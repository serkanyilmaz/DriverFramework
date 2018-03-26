#pragma once

#include "DFLog.hpp"

#include <stdint.h>
#include <time.h>
#include "HandleObj.hpp"

#ifndef __DF_NUTTX
#include "WorkMgr.hpp"
#endif

#ifdef __DF_LINUX
// Show backtrace on error
#define DF_ENABLE_BACKTRACE 1
#endif

namespace DriverFramework
{

/**
 * Get the absolute time off the system realtime clock
 *
 * @param ts the realtime time
 *
 * @return 0 if successful, nonzero else
 */
int absoluteTime(struct timespec &ts);

// convert offset time to absolute time
int absoluteTimeInFuture(uint64_t time_us, struct timespec &ts);

/**
 * Get the absolute time off the system monotonic clock
 *
 * @param ts the realtime time
 *
 * @return 0 if successful, nonzero else
 */
int clockGetMonotonic(struct timespec &ts);


#ifdef DF_ENABLE_BACKTRACE
// Used to show a backtrace while running
void backtrace();
#endif

class Framework
{
public:
	// Initialize the driver framework
	// This function must be called before any of the functions below
	static int initialize();

	// Terminate the driver framework
	static void shutdown();

	// Block until shutdown requested
	static void waitForShutdown();
};

}
