#ifndef __PL_THREAD_H
#define __PL_THREAD_H

#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef pthread_t pl_thread_t;
typedef pthread_mutex_t pl_mutex_t;

typedef void *(*pl_thread_entry_t)(void *);

static inline int pl_thread_create(pl_thread_t *thread, void *(*entry)(void *), void *arg)
{
    return pthread_create(thread, NULL, entry, arg);
}

static inline int pl_thread_join(pl_thread_t thread, void **result)
{
    return pthread_join(thread, result);
}

static inline int pl_thread_detach(pl_thread_t thread)
{
    return pthread_detach(thread);
}

static inline void pl_thread_exit(void *result)
{
    pthread_exit(result);
}

static inline pl_thread_t pl_thread_self(void)
{
    return pthread_self();
}

static inline int pl_thread_equal(pl_thread_t first, pl_thread_t second)
{
    return pthread_equal(first, second);
}

static inline int pl_mutex_init(pl_mutex_t *mutex)
{
    return pthread_mutex_init(mutex, NULL);
}

static inline int pl_mutex_destroy(pl_mutex_t *mutex)
{
    return pthread_mutex_destroy(mutex);
}

static inline int pl_mutex_lock(pl_mutex_t *mutex)
{
    return pthread_mutex_lock(mutex);
}

static inline int pl_mutex_trylock(pl_mutex_t *mutex)
{
    return pthread_mutex_trylock(mutex);
}

static inline int pl_mutex_unlock(pl_mutex_t *mutex)
{
    return pthread_mutex_unlock(mutex);
}


#ifdef __cplusplus
}
#endif

#endif
