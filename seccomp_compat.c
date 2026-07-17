// Fallback implementations for libseccomp functionality that is missing in
// older libseccomp headers/libraries. Declarations, and the version checks
// that gate them, live in seccomp_compat.h.

#include "seccomp_compat.h"

#if SCMP_VER_MAJOR == 2 && SCMP_VER_MINOR < 4

// The libseccomp API level functions were added in v2.4.0.
const unsigned int seccomp_api_get(void)
{
	// libseccomp-golang requires libseccomp v2.3.1, at a minimum, which
	// supported API level 2. However, the kernel may not support API level
	// 2 constructs which are the seccomp() system call and the TSYNC
	// filter flag. Return the "reserved" value of 0 here to indicate that
	// proper API level support is not available in libseccomp.
	return 0;
}

int seccomp_api_set(unsigned int level)
{
	return -EOPNOTSUPP;
}

#endif // < 2.4.0


#if SCMP_VER_MAJOR == 2 && SCMP_VER_MINOR < 5

// The seccomp notify API functions were added in v2.5.0.

int seccomp_notify_alloc(struct seccomp_notif **req, struct seccomp_notif_resp **resp) {
	return -EOPNOTSUPP;
}
int seccomp_notify_fd(const scmp_filter_ctx ctx) {
	return -EOPNOTSUPP;
}
void seccomp_notify_free(struct seccomp_notif *req, struct seccomp_notif_resp *resp) {
}
int seccomp_notify_id_valid(int fd, uint64_t id) {
	return -EOPNOTSUPP;
}
int seccomp_notify_receive(int fd, struct seccomp_notif *req) {
	return -EOPNOTSUPP;
}
int seccomp_notify_respond(int fd, struct seccomp_notif_resp *resp) {
	return -EOPNOTSUPP;
}

#endif // < 2.5.0


#if SCMP_VER_MAJOR == 2 && SCMP_VER_MINOR < 6

// The following functions were added in libseccomp v2.6.0.

int seccomp_precompute(scmp_filter_ctx ctx) {
	return -EOPNOTSUPP;
}
int seccomp_export_bpf_mem(const scmp_filter_ctx ctx, void *buf, size_t *len)  {
	return -EOPNOTSUPP;
}
int seccomp_transaction_start(const scmp_filter_ctx ctx) {
	return -EOPNOTSUPP;
}
int seccomp_transaction_commit(const scmp_filter_ctx ctx) {
	return -EOPNOTSUPP;
}
void seccomp_transaction_reject(const scmp_filter_ctx ctx) {
}

#endif // < 2.6.0
