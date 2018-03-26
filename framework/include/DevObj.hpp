#include "DriverFramework.hpp"
#include "DFList.hpp"
#include "DevMgr.hpp"
#include "SyncObj.hpp"
#include "DisableCopy.hpp"
#include <stdint.h>

#pragma once

#if __DF_NUTTX
#error DevObj not supported in NuttX
#endif

#define DRIVER_MAX_INSTANCES 5

namespace DriverFramework
{

// Re-use Device ID types from PX4
enum DeviceBusType {
	DeviceBusType_UNKNOWN = 0,
	DeviceBusType_I2C     = 1,
	DeviceBusType_SPI     = 2,
	DeviceBusType_UAVCAN  = 3,
	DeviceBusType_VIRT    = 4,
};

/*
  broken out device elements. The bitfields are used to keep
  the overall value small enough to fit in a float accurately,
  which makes it possible to transport over the MAVLink
  parameter protocol without loss of information.
 */
struct DeviceStructure {
	enum DeviceBusType bus_type : 3;
	uint8_t bus: 5;    // which instance of the bus type
	uint8_t address;   // address on the bus (eg. I2C address)
	uint8_t devtype;   // device class specific device type
};

union DeviceId {
	struct DeviceStructure dev_id_s;
	uint32_t dev_id;
};

class DevObj : public DisableCopy
{
public:
	DevObj(const char *name, const char *dev_path, const char *dev_class_path, DeviceBusType bus_type,
	       unsigned int sample_interval_usec);

	virtual int init();

	virtual int start();

	virtual int stop();

	void setSampleInterval(unsigned int sample_interval_usecs);

	virtual ~DevObj();

	union DeviceId getId()
	{
		return m_id;
	}

	bool isRegistered()
	{
		return (m_driver_instance >= 0);
	}

	int getInstance()
	{
		return m_driver_instance;
	}

	virtual int devIOCTL(unsigned long request, unsigned long arg);

	virtual ssize_t devRead(void *buf, size_t count);

	virtual ssize_t devWrite(const void *buf, size_t count);

	// _measure() is the periodic callback that is called every
	// m_sample_interval_usecs
	virtual void _measure() = 0; // periodic callback

	static uint16_t swap16(uint16_t val) { return (val >> 8) | (val << 8); }

	const char 	 	*m_name;
	const char 	 	*m_dev_path;
	char 	 		*m_dev_class_path;
	char 	 		*m_dev_instance_path;
	unsigned int 		m_sample_interval_usecs;
	union DeviceId		m_id;

	WorkHandle 		m_work_handle;
	bool 			m_pub_blocked;

private:
	int addHandle(DevHandle &h);
	int removeHandle(DevHandle &h);

	friend DevMgr;

	static void measure(void *arg);

	// Disallow copy
	DevObj(const DevObj &);

	int 			m_driver_instance;	// m_driver_instance = -1 when unregistered
	DFPointerList		m_handles;
	SyncObj			m_handle_lock;
	unsigned 		m_refcount;
};

}
