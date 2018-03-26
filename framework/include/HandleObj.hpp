#pragma once

#include "DisableCopy.hpp"

namespace DriverFramework
{

class HRTWorkQueue;
class WorkMgr;
class DevMgr;

class HandleObj : public DisableCopy
{
public:
	HandleObj() {}

	virtual ~HandleObj() {}

	bool isValid()
	{
#ifdef __DF_NUTTX
		return m_fd != -1;
#else
		return m_handle != nullptr;
#endif
	}

	int getError()
	{
		return m_errno;
	}

protected:
	friend DevMgr;

#ifdef __DF_NUTTX
	int 	m_fd = -1;
#else
	void 	*m_handle = nullptr;
#endif
	int 	m_errno = 0;
};

class IntHandleObj : public DisableCopy
{
public:
	IntHandleObj() {}

	virtual ~IntHandleObj() {}

	bool isValid()
	{
		return m_handle != -1;
	}

	int getError()
	{
		return m_errno;
	}

protected:
	friend HRTWorkQueue;
	friend WorkMgr;

	int	m_handle = -1;
	int 	m_errno = 0;
};
}
