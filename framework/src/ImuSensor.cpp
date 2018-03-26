#include "ImuSensor.hpp"

namespace DriverFramework
{

void printImuValues(struct imu_sensor_data &data)
{
	DF_LOG_INFO("IMU: accel: [%.2f, %.2f, %.2f] m/s^2",
		    (double)data.accel_m_s2_x,
		    (double)data.accel_m_s2_y,
		    (double)data.accel_m_s2_z);
	DF_LOG_INFO("     gyro:  [%.2f, %.2f, %.2f] rad/s",
		    (double)data.gyro_rad_s_x,
		    (double)data.gyro_rad_s_y,
		    (double)data.gyro_rad_s_z);

	DF_LOG_INFO("     mag:  [%.6f, %.6f, %.6f] ga",
		    (double)data.mag_ga_x,
		    (double)data.mag_ga_y,
		    (double)data.mag_ga_z);

	DF_LOG_INFO("     temp:  %.2f C",
		    (double)data.temp_c);
}

}
