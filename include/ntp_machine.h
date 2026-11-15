/*
 * ntp_machine.h
 *
 * Collect all machine dependent idiosyncrasies in one place.
 *
 */

#ifndef GUARD_NTP_MACHINE_H
#define GUARD_NTP_MACHINE_H

#include <sys/time.h>
#include <time.h>

#ifndef CLOCK_REALTIME
/*
 * Pacify platforms that don't have a real clock_gettime(2),
 * notably Mac OS X.
 */
#define CLOCK_REALTIME	0
#define CLOCK_MONOTONIC	1
typedef int clockid_t;
int clock_gettime(clockid_t clock_id, struct timespec *tp);
#endif

#ifdef __QNX__
/* QNX <time.h> has its own, signed, time64_t; ntp_types.h defines an unsigned one. */
#define time64_t ntp_time64_t
/* QNX has no adjtime(); libntp/adjtime_qnx.c implements it with ClockAdjust(). */
int adjtime(const struct timeval *, struct timeval *);
#endif

int ntp_set_tod (struct timespec *tvs);

#endif	/* GUARD_NTP_MACHINE_H */
