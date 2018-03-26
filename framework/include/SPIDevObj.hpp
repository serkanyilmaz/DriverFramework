#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "DevObj.hpp"

#pragma once

namespace DriverFramework
{

/* Defines for common frequencies */
#define SPI_FREQUENCY_320KHZ (320000)
#define SPI_FREQUENCY_1MHZ (1000000)
#define SPI_FREQUENCY_5MHZ (5000000)
#define SPI_FREQUENCY_10MHZ (10000000)
#define SPI_FREQUENCY_15MHZ (15000000)
#define SPI_FREQUENCY_20MHZ (20000000)

class SPIDevObj : public DevObj
{
public:
	SPIDevObj(const char *name, const char *dev_path, const char *dev_class_path, unsigned int sample_interval_usec) :
		DevObj(name, dev_path, dev_class_path, DeviceBusType_SPI, sample_interval_usec)
	{
		m_id.dev_id_s.bus = DeviceBusType_SPI;
	}

	virtual ~SPIDevObj();

	virtual int start();
	virtual int stop();

	static int readReg(DevHandle &h, uint8_t address, uint8_t &val);
	static int writeReg(DevHandle &h, uint8_t address, uint8_t val);
	static int writeRegVerified(DevHandle &h, uint8_t address, uint8_t val);
	static int bulkRead(DevHandle &h, uint8_t address, uint8_t *out_buffer, int length);
	static int setLoopbackMode(DevHandle &h, bool enable);

protected:
	int _readReg(uint8_t address, uint8_t &val);
	int _writeReg(uint8_t address, uint8_t *in_buffer, uint16_t length);
	int _writeReg(uint8_t address, uint8_t val);
	int _modifyReg(uint8_t address, uint8_t clearbits, uint8_t setbits);
	int _transfer(uint8_t *write_buffer, uint8_t *read_buffer, uint8_t len);

	int _bulkRead(uint8_t address, uint8_t *out_buffer, int length);
	int _setBusFrequency(uint32_t freq_hz);

	int m_fd = -1;
};

};
