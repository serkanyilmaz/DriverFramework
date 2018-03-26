#pragma once

#include <stdint.h>
#include <time.h>
#ifdef __DF_QURT
#include <dspal_time.h>
#endif
#include "HandleObj.hpp"

namespace DriverFramework
{

// Types
class WorkHandle : public IntHandleObj
{
public:
	WorkHandle() {}

	virtual ~WorkHandle();
	friend WorkMgr;
};

typedef void (*WorkCallback)(void *arg);

// Get the offset time from startup
uint64_t offsetTime();

// convert offset time to absolute time
struct timespec offsetTimeToAbsoluteTime(uint64_t offset_time);

#ifdef DF_ENABLE_BACKTRACE
// Used to show a backtrace while running
void backtrace();
#endif

class Framework;

class WorkMgr
{
public:
	// Interface functions
	static void getWorkHandle(WorkCallback cb, void *arg, uint32_t delay_usec, WorkHandle &handle);
	static void releaseWorkHandle(WorkHandle &handle);
	static int schedule(WorkHandle &handle);
	static void setError(WorkHandle &h, int error);

private:
	friend class Framework;

	static bool isValidHandle(const WorkHandle &h);
	static int initialize();
	static void finalize();

	static bool m_initialized;
};

}

