#ifndef _SYS_FUTEX_H_
#define _SYS_FUTEX_H_
#include <sys/syscall.h>
#include <linux/futex.h>
#include <time.h>
#include <unistd.h>

static long sys_futex(void *addr1, int op, int val1, struct timespec *timeout, void *addr2, int val3)
{
	return syscall(SYS_futex, addr1, op, val1, timeout, addr2, val3);
}
#endif