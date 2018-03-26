#include "DevObj.hpp"

#pragma once

namespace DriverFramework
{

class VirtDevObj : public DevObj
{
public:
	VirtDevObj(const char *name, const char *dev_path, const char *dev_class_path, unsigned int sample_interval) :
		DevObj(name, dev_path, dev_class_path, DeviceBusType_VIRT, sample_interval)
	{}

	virtual ~VirtDevObj() {}

protected:
	virtual void _measure() = 0;

private:

};

};
