#include <errno.h>
#include "DriverFramework.hpp"
#include "WorkItems.hpp"

using namespace DriverFramework;

bool WorkMgr::m_initialized = false;

//-----------------------------------------------------------------------
// Static Functions
//-----------------------------------------------------------------------

// TODO FIXME: this seems conflicting with WorkHandle::isValid()
bool WorkMgr::isValidHandle(const WorkHandle &h)
{
	return ((h.m_handle >= 0) && WorkItems::isValidIndex(h.m_handle));
}

//-----------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------

int WorkMgr::initialize()
{
	DF_LOG_DEBUG("WorkMgr::initialize");

	if (m_initialized) {
		DF_LOG_ERR("WorkMgr already initialized");
		return -1;
	}

	m_initialized = true;

	return 0;
}

void WorkMgr::finalize()
{
	DF_LOG_DEBUG("WorkMgr::finalize");

	if (!m_initialized) {
		DF_LOG_ERR("WorkMgr not initialized, cannot finalize");
		return;
	}

	WorkItems::finalize();
}

void WorkMgr::getWorkHandle(WorkCallback cb, void *arg, uint32_t delay_usec, WorkHandle &wh)
{
	// Use -1 to flag that we don't know the index, otherwise we pass undefined.
	int handle = -1;

	int ret = WorkItems::getIndex(cb, arg, delay_usec, handle);

	if (ret == 0) {
		wh.m_errno = 0;
		wh.m_handle = (int)handle;

	} else {
		wh.m_errno = ret;
		wh.m_handle = -1;
	}
}

void WorkMgr::releaseWorkHandle(WorkHandle &wh)
{
	DF_LOG_DEBUG("WorkMgr::releaseWorkHandle");

	if (wh.m_handle == -1) {
		wh.m_errno = EBADF;
	}

	if (isValidHandle(wh)) {
		WorkItems::unschedule(wh.m_handle);
		wh.m_errno = 0;

	} else {
		wh.m_errno = EBADF;
	}

	wh.m_handle = -1;
}

void WorkMgr::setError(WorkHandle &h, int error)
{
	h.m_errno = error;
}

WorkHandle::~WorkHandle()
{
	WorkMgr::releaseWorkHandle(*this);
}
