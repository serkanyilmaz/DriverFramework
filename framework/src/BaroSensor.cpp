#include "BaroSensor.hpp"
namespace DriverFramework
{

void printPressureValues(struct baro_sensor_data &data)
{
	DF_LOG_INFO("Pressure: %.2f Pa, temperature: %.2f C", (double)data.pressure_pa, (double)data.temperature_c);
}

}
