#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "DevObj.hpp"

#pragma once

namespace DriverFramework
{

class I2CDevObj : public DevObj
{
public:
	I2CDevObj(const char *name, const char *dev_path, const char *dev_class_path, unsigned int sample_interval_usec) :
		DevObj(name, dev_path, dev_class_path, DeviceBusType_I2C, sample_interval_usec)
	{
		m_id.dev_id_s.bus = DeviceBusType_I2C;
	}

	virtual ~I2CDevObj() = default;

	virtual int start();
	virtual int stop();

	static int readReg(DevHandle &h, uint8_t address, uint8_t *out_buffer, size_t length);
	static int writeReg(DevHandle &h, uint8_t address, uint8_t *in_buffer, size_t length);

protected:
	int _readReg(uint8_t address, uint8_t *out_buffer, size_t length);
	int _writeReg(uint8_t address, uint8_t *out_buffer, size_t length);
	int _readReg16(uint16_t address, uint16_t *out_buffer, size_t length);
	int _writeReg16(uint16_t address, uint16_t *out_buffer, size_t length);

	// read from a register without ioctl
	int _simple_read(uint8_t *out_buffer, size_t length);

	int _setSlaveConfig(uint32_t slave_address, uint32_t bus_frequency_khz, uint32_t transfer_timeout_usec);

	int m_fd{ -1};
	unsigned _retries{0};
};

};
