#include "DriverFramework.hpp"
#ifdef __DF_QURT
#include "dspal_time.h"
#endif
#include <time.h>

#if defined(__DF_APPLE_LEGACY)
#include <sys/time.h>
static int clock_gettime(int clk_id, struct timespec *t)
{
	struct timeval now;
	int rv = gettimeofday(&now, nullptr);

	if (rv) {
		return rv;
	}

	t->tv_sec = now.tv_sec;
	t->tv_nsec = now.tv_usec * 1000;

	return 0;
}
#endif

using namespace DriverFramework;

//-----------------------------------------------------------------------
// Global Functions
//-----------------------------------------------------------------------

int DriverFramework::absoluteTime(struct timespec &ts)
{
#if defined(__DF_NUTTX) || defined(__DF_APPLE_LEGACY)
	// CLOCK_MONOTONIC not available on NuttX or OSX
	return clock_gettime(0, &ts);
#else
	return clock_gettime(CLOCK_MONOTONIC, &ts);
#endif
}

int DriverFramework::absoluteTimeInFuture(uint64_t time_us, struct timespec &ts)
{
	int ret = absoluteTime(ts);

	if (ret == 0) {
		uint64_t nsecs = ts.tv_nsec + time_us * 1000;
		uint64_t secs = (nsecs / 1000000000);

		ts.tv_sec += secs;
		ts.tv_nsec = nsecs - secs * 1000000000;
	}

	return ret;
}
