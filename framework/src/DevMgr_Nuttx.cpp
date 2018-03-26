#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "DriverFramework.hpp"
#include "DevMgr.hpp"

#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <poll.h>
#include <sys/ioctl.h>

using namespace DriverFramework;

int DevMgr::initialize(void)
{
	return 0;
}

void DevMgr::finalize(void)
{
}

void DevMgr::getHandle(const char *dev_path, DevHandle &h)
{
	if (dev_path == nullptr) {
		h.m_errno = EINVAL;
		return;
	}

	h.m_errno = EBADF;

	if (h.m_fd >= 0) {
		::close(h.m_fd);
	}

	h.m_fd = ::open(dev_path, O_RDONLY);

	if (h.m_fd < 0) {
		h.m_errno = errno;
	}
}

void DevMgr::releaseHandle(DevHandle &h)
{
	::close(h.m_fd);
	h.m_fd = -1;
	h.m_errno = 0;
}

void DevMgr::setDevHandleError(DevHandle &h, int error)
{
	h.m_errno = error;
}

int DevMgr::getNextDeviceName(unsigned int &index, const char **dev_path)
{
	unsigned idx = 0;

	/* list directory */
	DIR *d = ::opendir("/dev");

	if (d) {
		struct dirent	*direntry;

		char devname[50];

		while ((direntry = readdir(d)) != nullptr) {
			if (idx == index) {
				snprintf(devname, sizeof(devname), "/dev/%s", direntry->d_name);
				*dev_path = direntry->d_name;
				++index;
				return 0;
			}
		}
	}

	return -1;
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

int DevHandle::ioctl(unsigned long cmd, unsigned long arg)
{
	return ::ioctl(m_fd, cmd, arg);
}

ssize_t DevHandle::read(void *buf, size_t len)
{
	return ::read(m_fd, buf, len);
}

ssize_t DevHandle::write(const void *buf, size_t len)
{
	return ::write(m_fd, buf, len);
}

