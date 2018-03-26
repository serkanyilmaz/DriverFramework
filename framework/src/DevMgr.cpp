#include <stdio.h>
#include <string.h>
#include "DriverFramework.hpp"
#include "DFList.hpp"
#include "SyncObj.hpp"
#include "DevObj.hpp"
#include "DevMgr.hpp"

#include <stdlib.h>
#include <errno.h>

using namespace DriverFramework;

static SyncObj *g_lock_dev_mgr = nullptr;

bool DevMgr::m_initialized = false;

#define NO_VERIFY 1 // Use fast method to get DevObj

// TODO add missing locking and reimplement with map
// This is meant just to work for now. It hs not been optimized yet.

static DFPointerList g_driver_list;

int DevMgr::initialize()
{
	g_lock_dev_mgr = new SyncObj();

	if (g_lock_dev_mgr == nullptr) {
		return -3;
	}

	g_lock_dev_mgr->lock();
	m_initialized = true;
	g_lock_dev_mgr->unlock();
	return 0;
}

void DevMgr::finalize()
{
	g_lock_dev_mgr->lock();
	m_initialized = false;

	g_driver_list.clear();

	g_lock_dev_mgr->unlock();
	delete g_lock_dev_mgr;
	g_lock_dev_mgr = nullptr;
}

#if DRIVER_MAX_INSTANCES > 9
#error must redo algorithm below
#endif

int DevMgr::registerDriver(DevObj *obj)
{
	DF_LOG_DEBUG("DevMgr::registerDriver %s", obj->m_name);

	if (obj->m_dev_path == nullptr) {
		return -2;
	}

	bool registered = false;
	g_lock_dev_mgr->lock();
	int ret = 0;

	if (obj->m_dev_class_path != nullptr) {
		for (unsigned int i = 0; i < DRIVER_MAX_INSTANCES; i++) {
			bool found = false;
			char tmp_path[strlen(obj->m_dev_class_path) + 3];
			snprintf(tmp_path, sizeof(tmp_path), "%s%d", obj->m_dev_class_path, i);
			DFPointerList::Index idx = nullptr;
			idx = g_driver_list.next(idx);

			while (idx != nullptr) {
				DevObj *list_obj = reinterpret_cast<DevObj *>(g_driver_list.get(idx));

				if ((list_obj->m_dev_instance_path) && (strcmp(tmp_path, list_obj->m_dev_instance_path) == 0)) {
					found = true;
					break;
				}

				idx = g_driver_list.next(idx);
			}

			if (!found)  {
				if (obj->m_dev_instance_path) {
					free(obj->m_dev_instance_path);
					obj->m_dev_instance_path = nullptr;
				}

				obj->m_dev_instance_path = strdup(tmp_path);
				g_driver_list.pushBack(obj);
				DF_LOG_DEBUG("Added driver %p %s", obj, obj->m_dev_instance_path);
				registered = true;
				break;
			}
		}

		if (!registered) {
			// Error - no available dev class instance
			ret = -3;
		}

	} else {
		// Some drivers do not specify a base class, or hardcode a specific instance
		g_driver_list.pushBack(obj);
		DF_LOG_DEBUG("Added driver %p %s", obj, obj->m_dev_path);
	}

	g_lock_dev_mgr->unlock();
	return ret;
}

void DevMgr::unregisterDriver(DevObj *obj)
{
	DF_LOG_DEBUG("DevMgr::unregisterDriver %s", obj->m_name);

	if (!g_lock_dev_mgr) {
		return;
	}

	g_lock_dev_mgr->lock();
	DFPointerList::Index idx = nullptr;
	idx = g_driver_list.next(idx);

	while (idx != nullptr) {
		DevObj *list_obj = reinterpret_cast<DevObj *>(g_driver_list.get(idx));

		if (list_obj == obj) {
			g_driver_list.erase(idx);
			break;
		}

		idx = g_driver_list.next(idx);
	}

	g_lock_dev_mgr->unlock();
}

DevObj *DevMgr::getDevObjByID(union DeviceId id)
{
	g_lock_dev_mgr->lock();
	DFPointerList::Index idx = nullptr;
	idx = g_driver_list.next(idx);

	while (idx != nullptr) {
		DevObj *list_obj = reinterpret_cast<DevObj *>(g_driver_list.get(idx));

		if (list_obj->getId().dev_id == id.dev_id) {
			break;
		}

		idx = g_driver_list.next(idx);
	}

	g_lock_dev_mgr->unlock();
	return (idx != nullptr) ? reinterpret_cast<DevObj *>(g_driver_list.get(idx)) : nullptr;
}

DevObj *DevMgr::_getDevObjByHandle(DevHandle &h)
{
	g_lock_dev_mgr->lock();
	DFPointerList::Index idx = nullptr;
	idx = g_driver_list.next(idx);

	while (idx != nullptr) {
		DevObj *list_obj = reinterpret_cast<DevObj *>(g_driver_list.get(idx));

		if (h.m_handle == list_obj) {
			break;
		}

		idx = g_driver_list.next(idx);
	}

	g_lock_dev_mgr->unlock();
	return (idx == nullptr) ? nullptr : reinterpret_cast<DevObj *>(g_driver_list.get(idx));
}

void DevMgr::getHandle(const char *dev_path, DevHandle &h)
{
	if (dev_path == nullptr) {
		h.m_errno = EINVAL;
		return;
	}

	h.m_errno = EBADF;

	g_lock_dev_mgr->lock();
	DFPointerList::Index idx = nullptr;
	idx = g_driver_list.next(idx);

	while (idx != nullptr) {
		DevObj *list_obj = reinterpret_cast<DevObj *>(g_driver_list.get(idx));

		// The dev path may be a class instance path or a dev name
		if ((strcmp(dev_path, list_obj->m_dev_path) == 0) ||
		    (list_obj->m_dev_instance_path && (strcmp(dev_path, list_obj->m_dev_instance_path) == 0))) {

			// Device is registered
			g_lock_dev_mgr->unlock();
			list_obj->addHandle(h);
			g_lock_dev_mgr->lock();
			h.m_handle = list_obj;
			h.m_errno = 0;
			break;
		}

		idx = g_driver_list.next(idx);
	}

	g_lock_dev_mgr->unlock();
}

void DevMgr::releaseHandle(DevHandle &h)
{
	DevObj *driver = DevMgr::getDevObjByHandle<DevObj>(h);

	if (driver) {
		driver->removeHandle(h);
	}

	g_lock_dev_mgr->lock();
	h.m_handle = nullptr;
	h.m_errno = 0;
	g_lock_dev_mgr->unlock();
}

void DevMgr::setDevHandleError(DevHandle &h, int error)
{
	h.m_errno = error;
}

int DevMgr::getNextDeviceName(unsigned int &index, const char **instancename)
{
	unsigned int i = 0;
	int ret = -1;
	g_lock_dev_mgr->lock();

	// First go through actual dev names
	DFPointerList::Index idx = nullptr;
	idx = g_driver_list.next(idx);

	while (idx != nullptr) {
		if (i == index) {
			DevObj *list_obj = reinterpret_cast<DevObj *>(g_driver_list.get(idx));
			*instancename = list_obj->m_dev_instance_path;
			index += 1;
			ret = 0;
			break;
		}

		idx = g_driver_list.next(idx);
		++i;
	}

	g_lock_dev_mgr->unlock();
	return ret;
}

//------------------------------------------------------------------------
// DevHandle
//------------------------------------------------------------------------

DevHandle::~DevHandle()
{
	if (isValid()) {
		DevMgr::releaseHandle(*this);
	}
}

#ifndef __DF_NUTTX

int DevHandle::ioctl(unsigned long cmd, unsigned long arg)
{
	if (m_handle) {
		return reinterpret_cast<DevObj *>(m_handle)->devIOCTL(cmd, arg);
	}

	return -1;
}

ssize_t DevHandle::read(void *buf, size_t len)
{
	if (m_handle) {
		return reinterpret_cast<DevObj *>(m_handle)->devRead(buf, len);
	}

	return -1;
}

ssize_t DevHandle::write(const void *buf, size_t len)
{
	if (m_handle) {
		return reinterpret_cast<DevObj *>(m_handle)->devWrite(buf, len);
	}

	return -1;
}

#else

int DevHandle::ioctl(unsigned long cmd, unsigned long arg)
{
	return ::ioctl(m_fd, cmd, (void *)arg);
}

ssize_t DevHandle::read(void *buf, size_t len)
{
	return ::read(m_fd, buf, len);
}

ssize_t DevHandle::write(const void *buf, size_t len)
{
	return ::write(m_fd, buf, len);
}

#endif
