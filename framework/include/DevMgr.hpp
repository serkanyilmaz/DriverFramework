#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>
#include "HandleObj.hpp"
#include "DFList.hpp"

#pragma once

#define NO_VERIFY 1 // Use fast method to get Driver Obj by Handle

namespace DriverFramework
{

// Forward class declarations
class DevMgr;
class DevObj;

class DevHandle : public HandleObj
{
public:
	DevHandle() {}

	virtual ~DevHandle();

	int ioctl(unsigned long cmd, unsigned long arg);
	ssize_t read(void *buf, size_t len);
	ssize_t write(const void *buf, size_t len);

private:
	friend DevMgr;
};

typedef DFPointerList UpdateList;



// DevMgr Is initalized by DriverFramework::initialize()
class DevMgr
{
public:

#ifndef __DF_NUTTX
	static int registerDriver(DevObj *obj);
	static void unregisterDriver(DevObj *obj);

	static DevObj *getDevObjByID(union DeviceId id);

	template <typename T>
	static T *getDevObjByHandle(DevHandle &handle)
	{
		if (!m_initialized || !handle.isValid()) {
			return nullptr;
		}

#ifdef NO_VERIFY
		return reinterpret_cast<T *>(handle.m_handle);
#else
		return dynamic_cast<T *>(_getDevObjByHandle(handle));
#endif
	}

#endif

	static void getHandle(const char *dev_path, DevHandle &handle);
	static void releaseHandle(DevHandle &handle);

	static void setDevHandleError(DevHandle &h, int error);

	static int getNextDeviceName(unsigned int &index, const char **instancename);
private:
	friend class Framework;

	DevMgr();
	~DevMgr();

	static int initialize();
	static void finalize();

#ifndef __DF_NUTTX
	static DevObj *_getDevObjByHandle(DevHandle &handle);

	static bool m_initialized;
#endif
};

}
