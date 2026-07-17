// This file provides fallback definitions of seccomp_api_get() and
// seccomp_api_set(), which were added to libseccomp in v2.4.0. It is
// only included (from seccomp_internal.go) when this package is being
// compiled against libseccomp headers older than that, which don't
// declare these two functions at all.
//
// The libseccomp shared library actually loaded at runtime may still
// be v2.4.0 or newer (and thus provide real implementations of these
// functions) even though the headers used to compile this package are
// older -- for example, this package built against v2.3.3 headers, but
// run against a system libseccomp v2.5.5 or newer. A naive fallback
// that just always returns "unsupported" would permanently hide the
// real API level support in that case: cgo places the whole preamble
// (which is where this file gets included) into the same translation
// unit as the code that calls these functions, so a statically
// compiled-in fallback would be resolved directly at compile time and
// would always run, regardless of what's available at runtime.
//
// So instead, look the real symbols up in whatever shared library
// provides them at runtime via dlsym(), and only fall back to the
// "unsupported" behavior if that lookup fails. The lookup itself is
// only ever done once, in a constructor that runs at load time (before
// any Go code, so before any goroutines or extra OS threads exist),
// which lets it be a simple unsynchronized global write instead of a
// dlsym() call on every invocation guarded by a lock or an atomic
// check-and-set.

#include <dlfcn.h>
#include <errno.h>
#include <stddef.h>

typedef unsigned int (*seccomp_api_get_func)(void);
typedef int (*seccomp_api_set_func)(unsigned int);

static seccomp_api_get_func real_seccomp_api_get;
static seccomp_api_set_func real_seccomp_api_set;

__attribute__((constructor))
static void resolve_real_seccomp_api_funcs(void)
{
	real_seccomp_api_get = (seccomp_api_get_func)dlsym(RTLD_DEFAULT, "seccomp_api_get");
	real_seccomp_api_set = (seccomp_api_set_func)dlsym(RTLD_DEFAULT, "seccomp_api_set");
}

static unsigned int seccomp_api_get(void)
{
	if (real_seccomp_api_get != NULL)
		return real_seccomp_api_get();

	// libseccomp-golang requires libseccomp v2.2.0, at a minimum, which
	// supported API level 2. However, the kernel may not support API level
	// 2 constructs which are the seccomp() system call and the TSYNC
	// filter flag. Return the "reserved" value of 0 here to indicate that
	// proper API level support is not available in libseccomp.
	return 0;
}

static int seccomp_api_set(unsigned int level)
{
	if (real_seccomp_api_set != NULL)
		return real_seccomp_api_set(level);

	return -EOPNOTSUPP;
}
