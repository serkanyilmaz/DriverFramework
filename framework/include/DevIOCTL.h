#include <sys/ioctl.h>

#pragma once

/*
 * ioctl() definitions
 */

#ifdef __DF_NUTTX
#define _DF_IOC(x,y) _IOC(x,y)
#else
#define _DF_IOC(x,y) _IO(x,y)
#endif

#define _DEVICEIOCBASE          (0x100)
#define _DEVICEIOC(_n)          (_DF_IOC(_DEVICEIOCBASE, _n))

/** ask device to stop publishing */
#define DEVIOCSPUBBLOCK _DEVICEIOC(0)

/** check publication block status */
#define DEVIOCGPUBBLOCK _DEVICEIOC(1)

/**
 * Return device ID, to enable matching of configuration parameters
 * (such as compass offsets) to specific sensors
 */
#define DEVIOCGDEVICEID _DEVICEIOC(2)

