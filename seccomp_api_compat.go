package seccomp

// -ldl is needed for the dlsym()-based fallback in seccomp_api_compat.h,
// used when this package is compiled against libseccomp headers older
// than v2.4.0. cgo directives can only be written as Go comments (they
// are parsed by the cgo tool itself, not the C compiler), so this can't
// live inside seccomp_api_compat.h; it's kept in its own file instead
// of seccomp_internal.go to stay next to what it's for.
//
// This flag is always passed, even when building against headers new
// enough that seccomp_api_compat.h isn't included, since cgo directives
// can't be conditioned on the C preprocessor macros that decide that.
// That's harmless: on any modern libc, -ldl either links a compatibility
// stub (the dl* functions have lived in libc itself since glibc 2.34) or
// a small, universally available library.

// #cgo LDFLAGS: -ldl
import "C"
