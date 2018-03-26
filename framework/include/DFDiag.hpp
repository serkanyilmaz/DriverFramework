#pragma once

#include "DFLog.hpp"

namespace DriverFramework
{

class DFDiag
{
public:
	static void listRawDevices();
	static void listRegisteredDevices();
private:
	DFDiag() {}
	~DFDiag() {}
};
};
