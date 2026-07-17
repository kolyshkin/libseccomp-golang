#ifndef SECCOMP_COMPAT_H
#define SECCOMP_COMPAT_H

#include <errno.h>
#include <stdlib.h>
#include <seccomp.h>

// Minimally required version during compile time.
#if (SCMP_VER_MAJOR < 2) || \
    (SCMP_VER_MAJOR == 2 && SCMP_VER_MINOR < 3) || \
    (SCMP_VER_MAJOR == 2 && SCMP_VER_MINOR == 3 && SCMP_VER_MICRO < 1)
#error This package requires libseccomp >= v2.3.1
#endif


// Macros that are missing from older libseccomp headers. Being macros, they
// only need an #ifndef guard, so no version tracking is needed for them.

#define ARCH_BAD ~0

#ifndef SCMP_ARCH_PPC
#define SCMP_ARCH_PPC ARCH_BAD
#endif

#ifndef SCMP_ARCH_PPC64
#define SCMP_ARCH_PPC64 ARCH_BAD
#endif

#ifndef SCMP_ARCH_PPC64LE
#define SCMP_ARCH_PPC64LE ARCH_BAD
#endif

#ifndef SCMP_ARCH_S390
#define SCMP_ARCH_S390 ARCH_BAD
#endif

#ifndef SCMP_ARCH_S390X
#define SCMP_ARCH_S390X ARCH_BAD
#endif

#ifndef SCMP_ARCH_PARISC
#define SCMP_ARCH_PARISC ARCH_BAD
#endif

#ifndef SCMP_ARCH_PARISC64
#define SCMP_ARCH_PARISC64 ARCH_BAD
#endif

#ifndef SCMP_ARCH_RISCV64
#define SCMP_ARCH_RISCV64 ARCH_BAD
#endif

#ifndef SCMP_ARCH_LOONGARCH64
#define SCMP_ARCH_LOONGARCH64 ARCH_BAD
#endif

#ifndef SCMP_ARCH_M68K
#define SCMP_ARCH_M68K ARCH_BAD
#endif

#ifndef SCMP_ARCH_SH
#define SCMP_ARCH_SH ARCH_BAD
#endif

#ifndef SCMP_ARCH_SHEB
#define SCMP_ARCH_SHEB ARCH_BAD
#endif

#ifndef SCMP_ACT_LOG
#define SCMP_ACT_LOG 0x7ffc0000U
#endif

#ifndef SCMP_ACT_KILL_PROCESS
#define SCMP_ACT_KILL_PROCESS 0x80000000U
#endif

#ifndef SCMP_ACT_KILL_THREAD
#define SCMP_ACT_KILL_THREAD	0x00000000U
#endif

#ifndef SCMP_ACT_NOTIFY
#define SCMP_ACT_NOTIFY 0x7fc00000U
#endif


#if SCMP_VER_MAJOR == 2 && SCMP_VER_MINOR < 4

// SCMP_FLTATR_CTL_LOG was added in v2.4.0.
#define SCMP_FLTATR_CTL_LOG _SCMP_FLTATR_MIN

// The libseccomp API level functions were added in v2.4.0.
// Fallback implementations live in seccomp_compat.c.
const unsigned int seccomp_api_get(void);
int seccomp_api_set(unsigned int level);

#endif // < 2.4.0


#if SCMP_VER_MAJOR == 2 && SCMP_VER_MINOR < 5

// The following SCMP_FLTATR_*  were added in libseccomp v2.5.0.
#define SCMP_FLTATR_CTL_SSB      _SCMP_FLTATR_MIN
#define SCMP_FLTATR_CTL_OPTIMIZE _SCMP_FLTATR_MIN
#define SCMP_FLTATR_API_SYSRAWRC _SCMP_FLTATR_MIN

// The seccomp notify API structs and functions were added in v2.5.0.
// Fallback implementations live in seccomp_compat.c.

struct seccomp_data {
	int nr;
	__u32 arch;
	__u64 instruction_pointer;
	__u64 args[6];
};

struct seccomp_notif {
	__u64 id;
	__u32 pid;
	__u32 flags;
	struct seccomp_data data;
};

struct seccomp_notif_resp {
	__u64 id;
	__s64 val;
	__s32 error;
	__u32 flags;
};

int seccomp_notify_alloc(struct seccomp_notif **req, struct seccomp_notif_resp **resp);
int seccomp_notify_fd(const scmp_filter_ctx ctx);
void seccomp_notify_free(struct seccomp_notif *req, struct seccomp_notif_resp *resp);
int seccomp_notify_id_valid(int fd, uint64_t id);
int seccomp_notify_receive(int fd, struct seccomp_notif *req);
int seccomp_notify_respond(int fd, struct seccomp_notif_resp *resp);

#endif // < 2.5.0


#if SCMP_VER_MAJOR == 2 && SCMP_VER_MINOR < 6

#define SCMP_FLTATR_CTL_WAITKILL _SCMP_FLTATR_MIN

// The following functions were added in libseccomp v2.6.0.
// Fallback implementations live in seccomp_compat.c.

int seccomp_precompute(scmp_filter_ctx ctx);
int seccomp_export_bpf_mem(const scmp_filter_ctx ctx, void *buf, size_t *len);
int seccomp_transaction_start(const scmp_filter_ctx ctx);
int seccomp_transaction_commit(const scmp_filter_ctx ctx);
void seccomp_transaction_reject(const scmp_filter_ctx ctx);

#endif // < 2.6.0

#endif // SECCOMP_COMPAT_H
