#include "../h/chaos.h"

#ifdef BSD42
#include "param.h"
#include "systm.h"
#include "time.h"
#include "kernel.h"
#endif

#if defined(linux) && defined(__KERNEL__)
#include <linux/signal.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/time.h>
#else
#include <stdio.h>
#include <sys/time.h>
#endif

/*
 * Return the time according to the chaos TIME protocol, in a long.
 * No byte shuffling need be done here, just time conversion.
 */
void
ch_time(long *tp)
{
#if defined(linux) && defined(__KERNEL__)
	struct timeval time;
	do_gettimeofday(&time);
#endif

#if defined(linux) && !defined(__KERNEL__)
	struct timeval time;
	gettimeofday(&time, NULL);
#endif

#if defined(BSD42) || defined(linux)
	*tp = time.tv_sec;
#else
	*tp = time;
#endif
	*tp += 60L*60*24*((1970-1900)*365L + 1970/4 - 1900/4);
}

void
ch_uptime(long *tp)
{
#ifdef linux
#if !defined(__KERNEL__)
	*tp = 0;
#else
	*tp = jiffies;
#endif
#else
#ifdef BSD42
	*tp = (time.tv_sec - boottime.tv_sec) * 60L;
#else
	*tp = (time - bootime) * 60L;
#endif
#endif
}