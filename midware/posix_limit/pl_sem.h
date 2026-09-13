/**
 * pl: posix limit
 */

#ifndef PL_SEM_H
#define PL_SEM_H

#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef sem_t pl_sem_t;

static inline int pl_sem_init(pl_sem_t *sem, unsigned int value)
{
    return sem_init(sem, 0, value);
}

static inline int pl_sem_destroy(pl_sem_t *sem)
{
    return sem_destroy(sem);
}

static inline int pl_sem_wait(pl_sem_t *sem)
{
    return sem_wait(sem);
}

static inline int pl_sem_trywait(pl_sem_t *sem)
{
    return sem_trywait(sem);
}

static inline int pl_sem_post(pl_sem_t *sem)
{
    return sem_post(sem);
}

static inline int pl_sem_getvalue(pl_sem_t *sem, int *value)
{
    return sem_getvalue(sem, value);
}

#ifdef __cplusplus
}
#endif

#endif /* PL_SEM_H */
