/*
 * Copyright (c) 2025 RobotPilots-SZU
 * SPDX-License-Identifier: Apache-2.0
 *
 * OSAL core services (C). OS-agnostic: no <zephyr/*.h> here.
 *
 * Middleware and module layers depend only on this interface. A platform
 * (Zephyr, or a PC stub for testing) provides an implementation and injects
 * it via osal_init() at start-up. Usable from both C and C++.
 */
#ifndef OSAL_H
#define OSAL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */
/* Log                                                                */
/* ------------------------------------------------------------------ */
typedef enum {
    OSAL_LOG_ERR = 0,
    OSAL_LOG_WRN,
    OSAL_LOG_INF,
    OSAL_LOG_DBG,
} osal_log_level_t;

/* ------------------------------------------------------------------ */
/* Mutex (opaque handle, concrete type owned by the backend)          */
/* ------------------------------------------------------------------ */
typedef void *osal_mutex_t;

/* Wait-forever sentinel for osal_mutex_lock(). */
#define OSAL_WAIT_FOREVER  ((uint32_t)0xFFFFFFFFu)

/* ------------------------------------------------------------------ */
/* Backend interface: implemented by the platform, injected once.     */
/* Any function pointer may be NULL; the wrappers degrade gracefully  */
/* (no-op / safe default) when a service is absent.                   */
/* ------------------------------------------------------------------ */
typedef struct {
    /* Log */
    void         (*log)(osal_log_level_t lvl, const char *tag, const char *msg);

    /* Time */
    uint32_t     (*uptime_ms)(void);

    /* Mutex */
    osal_mutex_t (*mutex_create)(void);
    void         (*mutex_destroy)(osal_mutex_t m);
    int          (*mutex_lock)(osal_mutex_t m, uint32_t timeout_ms);
    void         (*mutex_unlock)(osal_mutex_t m);

    /* Memory */
    void        *(*mem_alloc)(size_t size);
    void         (*mem_free)(void *ptr);
} osal_backend_t;

/* Inject the backend implementation. Call once at start-up. */
void osal_init(const osal_backend_t *backend);

/* ------------------------------------------------------------------ */
/* Public convenience API                                             */
/* ------------------------------------------------------------------ */

/* Log */
void     osal_log(osal_log_level_t lvl, const char *tag, const char *msg);

/* Time: milliseconds since boot (0 if unsupported). */
uint32_t osal_uptime_ms(void);

/* Mutex */
osal_mutex_t osal_mutex_create(void);
void         osal_mutex_destroy(osal_mutex_t m);
/* timeout_ms: 0 = try, OSAL_WAIT_FOREVER = block. Returns 0 on success. */
int          osal_mutex_lock(osal_mutex_t m, uint32_t timeout_ms);
void         osal_mutex_unlock(osal_mutex_t m);

/* Memory */
void    *osal_mem_alloc(size_t size);
void     osal_mem_free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif /* OSAL_H */