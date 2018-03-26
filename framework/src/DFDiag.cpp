#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "DFLog.hpp"
#include "DFDiag.hpp"
#include "DevMgr.hpp"

using namespace DriverFramework;

void DFDiag::listRegisteredDevices()
{
	unsigned int index = 0;
	const char *name;

	DF_LOG_INFO("Registered Devices:");

	while (DevMgr::getNextDeviceName(index, &name) == 0) {
		DF_LOG_INFO("   '%s'", name);
	}
}

void DFDiag::listRawDevices()
{
	int fd;
	char devname[15];

	DF_LOG_INFO("I2C devices:");

	for (unsigned int i = 0; i < 8; i++) {
#ifdef __DF_QURT
		sprintf(devname, "/dev/iic-%u", i);
#else
		sprintf(devname, "/dev/i2c-%u", i);
#endif
		fd = ::open(devname, O_RDONLY);

		if (fd != -1) {
			DF_LOG_INFO("  %s", devname);
			close(fd);

		} else if (errno == EACCES) {
			DF_LOG_INFO("  %s (WARNING access denied)", devname);
		}
	}

	DF_LOG_INFO("SPI devices:");

	for (unsigned int i = 0; i < 8; i++) {
		sprintf(devname, "/dev/spi-%u", i);
		fd = ::open(devname, O_RDONLY);

		if (fd != -1) {
			DF_LOG_INFO("  %s", devname);
			close(fd);
		}
	}
}
