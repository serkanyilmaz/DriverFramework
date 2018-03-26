#pragma once

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

namespace DriverFramework
{
uint64_t offsetTime();
}

#define DF_INFO 0

#ifdef __DF_QURT

#include <cstdarg>

extern "C" {

// declaration to make the compiler happy.  This symbol is part of the DSP static image.
	void HAP_debug(const char *msg, int level, const char *filename, int line);

};

static __inline void qurt_log_2(int level, const char *file, int line, const char *format, ...)
{
	char buf[256];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);
	HAP_debug(buf, level, file, line);
}

#define DF_LOG_INFO(FMT, ...) qurt_log_2(0, __FILE__, __LINE__, "%" PRIu64 " " FMT  "\n", offsetTime(), ##__VA_ARGS__)
#define DF_LOG_ERR(FMT, ...)  qurt_log_2(0, __FILE__, __LINE__, "%" PRIu64 " " FMT  "\n", offsetTime(), ##__VA_ARGS__)

#if DF_DEBUG
#define DF_LOG_DEBUG(FMT, ...)  qurt_log_2(1, __FILE__, __LINE__, "%" PRIu64 " " FMT  "\n", offsetTime(), ##__VA_ARGS__)
#else
#define DF_LOG_DEBUG(FMT, ...)
#endif

#elif defined(__DF_NUTTX)

// TODO: Substitute logging implementation here

// TODO: NuttX doesn't support the PRIuN defines
// TODO: The NuttX build doesn't link to offsetTime() yet
#define DF_LOG_INFO(FMT, ...) printf(FMT  "\n", ##__VA_ARGS__)
#define DF_LOG_ERR(FMT, ...)  printf(FMT "\n", ##__VA_ARGS__)

#if DF_DEBUG
#define DF_LOG_DEBUG(FMT, ...)  printf(FMT "\n", ##__VA_ARGS__)
#else
#define DF_LOG_DEBUG(FMT, ...)
#endif

#else

// TODO: Substitute logging implementation here
#define DF_LOG_INFO(FMT, ...) printf("%" PRIu64 " " FMT  "\n", offsetTime(), ##__VA_ARGS__)
#define DF_LOG_ERR(FMT, ...)  printf("%" PRIu64 " " FMT "\n", offsetTime(), ##__VA_ARGS__)

#if DF_DEBUG
#define DF_LOG_DEBUG(FMT, ...)  printf("%" PRIu64 " " FMT "\n", offsetTime(), ##__VA_ARGS__)
#else
#define DF_LOG_DEBUG(FMT, ...)
#endif

#endif

