#pragma once

#include <pthread.h>

namespace DriverFramework
{

int initMutex(pthread_mutex_t &mutex);

class SyncObj
{
public:
	SyncObj();
	~SyncObj() = default;

	void lock();
	void unlock();

	// Returns 0 on success, ETIMEDOUT on timeout
	// Use timeout_us = 0 for blocking wait
	int waitOnSignal(unsigned long timeout_us);

	void signal();

private:
	pthread_mutex_t m_lock{};
	pthread_cond_t	m_new_data_cond{};
};

}
