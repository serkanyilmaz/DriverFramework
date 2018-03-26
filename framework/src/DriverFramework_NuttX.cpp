#include "DriverFramework.hpp"
#include "DevMgr.hpp"
#include <pthread.h>

using namespace DriverFramework;

//-----------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------

static pthread_mutex_t g_framework_exit = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_framework_cond;

//-----------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------

/*************************************************************************
  Framework
*************************************************************************/
void Framework::shutdown()
{
	// Free the DevMgr resources
	DevMgr::finalize();

	// allow Framework to exit
	pthread_mutex_lock(&g_framework_exit);
	pthread_cond_signal(&g_framework_cond);
	pthread_mutex_unlock(&g_framework_exit);
}

int Framework::initialize()
{
	int ret = DevMgr::initialize();

	pthread_cond_init(&g_framework_cond, nullptr);
	return (ret < 0) ? -1 : 0;
}

void Framework::waitForShutdown()
{
	// Block until shutdown requested
	pthread_mutex_lock(&g_framework_exit);
	pthread_cond_wait(&g_framework_cond, &g_framework_exit);
	pthread_mutex_unlock(&g_framework_exit);
}
