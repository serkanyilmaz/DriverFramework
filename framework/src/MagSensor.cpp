#include "MagSensor.hpp"

namespace DriverFramework
{

void printMagValues(struct mag_sensor_data &data)
{
	DF_LOG_INFO("Mag: [%.6f, %.6f, %.6f] Ga",
		    (double)data.field_x_ga,
		    (double)data.field_y_ga,
		    (double)data.field_z_ga);
}

}
