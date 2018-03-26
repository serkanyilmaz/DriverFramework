
#pragma once

#include <stdint.h>
#include "SyncObj.hpp"
#include "I2CDevObj.hpp"

#define MAG_CLASS_PATH  "/dev/mag"

namespace DriverFramework
{

/**
 * The sensor independent data structure containing pressure values.
 */
struct mag_sensor_data {
	float field_x_ga;
	float field_y_ga;
	float field_z_ga;
	uint64_t read_counter;		/*! the total number of pressure sensor readings since the system was started */
	uint64_t last_read_time_usec; 	/*! time stamp indicating the time at which the pressure in this data structure was read */
	uint64_t error_counter;		/*! the total number of errors detected when reading the pressure, since the system was started */
};

void printMagValues(struct mag_sensor_data &data);

class MagSensor : public I2CDevObj
{
public:
	MagSensor(const char *device_path, unsigned int sample_interval_usec) :
		I2CDevObj("MagSensor", device_path, MAG_CLASS_PATH, sample_interval_usec)
	{}

	virtual ~MagSensor() = default;

protected:
	virtual void _measure() = 0;

	struct mag_sensor_data		m_sensor_data;
};

}; // namespace DriverFramework
