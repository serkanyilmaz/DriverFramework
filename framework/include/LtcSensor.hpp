
/***************************************************
 * modified by Christoph Tobler <christoph@px4.io>
 ***************************************************/

#pragma once

#include <stdint.h>
#include "SyncObj.hpp"
#include "I2CDevObj.hpp"

#define LTC2946_CLASS_PATH  "/dev/ltc"

#define LTC2946_MEASURE_INTERVAL_US 20000  // 1000000/50Hz

namespace DriverFramework
{

/** * The sensor independent data structure containing LTC2946 values. */
struct ltc2946_sensor_data {
	// 5V sensor
	float board_voltage_V; // [V]
	float board_current_A; // [A]
	// Battery sensor
	float battery_voltage_V; // [V]
	float battery_current_A; // [A]

	float remain;
	uint64_t read_counter; /*! the total number of sensor readings since the system was started */
	uint64_t error_counter;
};

class LtcSensor : public I2CDevObj
{
public:
	LtcSensor(const char *device_path, unsigned int sample_interval_usec) :
		I2CDevObj("LtcSensor", device_path, LTC2946_CLASS_PATH, LTC2946_MEASURE_INTERVAL_US)
	{}

	~LtcSensor() {}

	static int getSensorData(DevHandle &h, struct ltc2946_sensor_data &out_data, bool is_new_data_required)
	{
		LtcSensor *me = DevMgr::getDevObjByHandle<LtcSensor>(h);
		int ret = -1;

		if (me != nullptr) {
			me->m_synchronize.lock();

			if (is_new_data_required) {
				me->m_synchronize.waitOnSignal(0);
			}

			out_data = me->m_sensor_data;
			me->m_synchronize.unlock();
			ret = 0;
		}

		return ret;
	}


protected:
	virtual void _measure() = 0;

	struct ltc2946_sensor_data		m_sensor_data;
	SyncObj 			m_synchronize;
};

}; // namespace DriverFramework
