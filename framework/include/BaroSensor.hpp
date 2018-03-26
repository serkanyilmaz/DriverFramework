#pragma once

#include <stdint.h>
#include "SyncObj.hpp"

#if defined(__DF_OCPOC)
#define __BARO_USE_SPI
#endif

#if defined(__BARO_USE_SPI)
#include "SPIDevObj.hpp"
#else
#include "I2CDevObj.hpp"
#endif

#define BARO_CLASS_PATH  "/dev/baro"

namespace DriverFramework
{

/**
 * The sensor independent data structure containing pressure values.
 */
struct baro_sensor_data {
	int32_t  t_fine; 		/*! used internally to calculate a temperature compensated pressure value. */
	float    pressure_pa; 		/*! current pressure in Pascals */
	float    temperature_c;		/*! current temperature in C at which the pressure was read */
	uint64_t read_counter;		/*! the total number of pressure sensor readings since the system was started */
	uint64_t last_read_time_usec; 	/*! time stamp indicating the time at which the pressure in this data structure was read */
	uint64_t error_counter;		/*! the total number of errors detected when reading the pressure, since the system was started */
};

void printPressureValues(struct baro_sensor_data &data);

#if defined(__BARO_USE_SPI)
class BaroSensor : public SPIDevObj
#else
class BaroSensor : public I2CDevObj
#endif
{
public:
	BaroSensor(const char *device_path, unsigned int sample_interval_usec) :
#if defined(__BARO_USE_SPI)
		SPIDevObj("BaroSensor", device_path, BARO_CLASS_PATH, sample_interval_usec)
#else
		I2CDevObj("BaroSensor", device_path, BARO_CLASS_PATH, sample_interval_usec)
#endif
	{}

	virtual ~BaroSensor() = default;

	void setAltimeter(float altimeter_setting_in_mbars)
	{
		m_altimeter_mbars = altimeter_setting_in_mbars;
	}

protected:
	virtual void _measure() = 0;

	struct baro_sensor_data	m_sensor_data {};
	float 				m_altimeter_mbars{0.0f};
};

}; // namespace DriverFramework
