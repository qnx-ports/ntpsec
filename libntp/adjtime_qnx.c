/*
 * adjtime() for QNX, built on ClockAdjust().
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * QNX has no adjtime().  ClockAdjust() slews CLOCK_REALTIME by adding
 * tick_nsec_inc nanoseconds to each of the next tick_count clock ticks.
 * The correction is spread over about one second, which is how often
 * ntpd calls adjtime(), at no more than 2% of the tick (QNX is a
 * real-time system, so the clock rate is not changed drastically).
 * A new call replaces any slew still in progress and returns what was
 * left of it in olddelta, as adjtime() does elsewhere.
 */
#include "config.h"

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/time.h>
#include <time.h>

#include "ntp_machine.h"

#define SLEW_LIMIT_DIVISOR 50	/* at most tick/50 extra per tick (2%) */

int
adjtime(const struct timeval *delta, struct timeval *olddelta)
{
	struct _clockadjust adj = { 0, 0 };
	struct _clockadjust old;
	int64_t ns = 0;

	if (delta != NULL)
		ns = (int64_t)delta->tv_sec * 1000000000 +
		    (int64_t)delta->tv_usec * 1000;

	if (ns != 0) {
		struct _clockperiod period;
		int64_t mag = llabs(ns);
		int64_t ticks_per_s, inc, limit;

		if (ClockPeriod(CLOCK_REALTIME, NULL, &period, 0) == -1)
			return -1;
		if (period.nsec == 0) {
			errno = EINVAL;
			return -1;
		}
		ticks_per_s = 1000000000 / period.nsec;
		if (ticks_per_s < 1)
			ticks_per_s = 1;
		inc = (mag + ticks_per_s - 1) / ticks_per_s;
		limit = period.nsec / SLEW_LIMIT_DIVISOR;
		if (limit < 1)
			limit = 1;
		if (inc > limit)
			inc = limit;
		adj.tick_nsec_inc = (_Int32t)(ns < 0 ? -inc : inc);
		adj.tick_count = (_Uint32t)((mag + inc / 2) / inc);
	}

	if (ClockAdjust(CLOCK_REALTIME, &adj, &old) == -1)
		return -1;

	if (olddelta != NULL) {
		int64_t left = old.tick_count == 0 ? 0 :
		    (int64_t)old.tick_count * old.tick_nsec_inc;

		olddelta->tv_sec = (time_t)(left / 1000000000);
		olddelta->tv_usec = (suseconds_t)((left % 1000000000) / 1000);
	}
	return 0;
}
