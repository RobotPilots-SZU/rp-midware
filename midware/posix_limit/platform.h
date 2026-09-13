#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#if defined(PLATFORM_LINUX)

#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#elif defined(PLATFORM_ZEPHYR)

#include <zephyr/posix/pthread.h>
#include <zephyr/posix/semaphore.h>
#include <time.h>

#elif defined(PLATFORM_FREERTOS)

#include <FreeRTOS_POSIX/pthread.h>
#include <FreeRTOS_POSIX/semaphore.h>
#include <FreeRTOS_POSIX/time.h>

#else

#error "Unsupported RP POSIX platform"

#endif

#endif