#pragma once

namespace DriverFramework
{

class DisableCopy
{
public:
	DisableCopy() {}
	virtual ~DisableCopy() {}

private:
	DisableCopy(const DisableCopy &);
	DisableCopy &operator=(const DisableCopy &);
};

}
