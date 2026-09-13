/**
 * pl: posix limit
 */

#ifndef __PL_TIME_H__
#define __PL_TIME_H__

#include "platform.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct timespec pl_timespec_t;

static inline int pl_clock_gettime(clockid_t clock_id, pl_timespec_t *time)
{
    return clock_gettime(clock_id, time);
}

static inline int pl_sleep(const pl_timespec_t *request, pl_timespec_t *remain)
{
    return nanosleep(request, remain);
}

static inline uint64_t pl_time_now_ms(void)
{
    pl_timespec_t ts;

    if (pl_clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        return 0;
    }

    return (uint64_t)ts.tv_sec * 1000u
         + (uint64_t)ts.tv_nsec / 1000000u;
}

static inline int pl_sleep_ms(uint32_t timeout_ms)
{
    pl_timespec_t request = {
        .tv_sec = timeout_ms / 1000u,
        .tv_nsec = (long)(timeout_ms % 1000u) * 1000000L,
    };

    return pl_sleep(&request, 0);
}

#ifdef __cplusplus
}
#endif
#endif /* __PL_TIME_H__ */
