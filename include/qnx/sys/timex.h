/*
 * sys/timex.h for QNX, which has no kernel clock discipline.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * QNX provides neither ntp_adjtime() nor struct timex.  This header
 * supplies the RFC 1589 / NTP kernel API types and constants so the
 * code compiles unchanged, and an ntp_adjtime() that always fails with
 * ENOSYS, so ntpd runs its own (daemon) clock discipline loop through
 * adjtime() (libntp/adjtime_qnx.c) instead of the kernel PLL.
 */
#ifndef GUARD_QNX_SYS_TIMEX_H
#define GUARD_QNX_SYS_TIMEX_H

#include <errno.h>
#include <sys/time.h>

#define MOD_OFFSET	0x0001
#define MOD_FREQUENCY	0x0002
#define MOD_MAXERROR	0x0004
#define MOD_ESTERROR	0x0008
#define MOD_STATUS	0x0010
#define MOD_TIMECONST	0x0020
#define MOD_TAI		0x0080
#define MOD_NANO	0x2000

#define STA_PLL		0x0001
#define STA_PPSFREQ	0x0002
#define STA_PPSTIME	0x0004
#define STA_FLL		0x0008
#define STA_INS		0x0010
#define STA_DEL		0x0020
#define STA_UNSYNC	0x0040
#define STA_FREQHOLD	0x0080
#define STA_PPSSIGNAL	0x0100
#define STA_PPSJITTER	0x0200
#define STA_PPSWANDER	0x0400
#define STA_PPSERROR	0x0800
#define STA_CLOCKERR	0x1000
#define STA_NANO	0x2000
#define STA_MODE	0x4000
#define STA_CLK		0x8000

#define TIME_OK		0
#define TIME_INS	1
#define TIME_DEL	2
#define TIME_OOP	3
#define TIME_WAIT	4
#define TIME_ERROR	5

struct timex {
	unsigned int modes;
	long	offset;
	long	freq;
	long	maxerror;
	long	esterror;
	int	status;
	long	constant;
	long	precision;
	long	tolerance;
	long	ppsfreq;
	long	jitter;
	int	shift;
	long	stabil;
	long	jitcnt;
	long	calcnt;
	long	errcnt;
	long	stbcnt;
};

static inline int ntp_adjtime(struct timex *tx)
{
	(void)tx;
	errno = ENOSYS;
	return -1;
}

#endif /* GUARD_QNX_SYS_TIMEX_H */
