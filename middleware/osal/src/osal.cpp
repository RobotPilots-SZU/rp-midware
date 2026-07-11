/*
 * Copyright (c) 2025 RobotPilots-SZU
 * SPDX-License-Identifier: Apache-2.0
 *
 * OSAL core dispatch. Stores the injected backend and forwards calls,
 * degrading gracefully when a service is not provided.
 */
#include "osal.h"

namespace {
osal_backend_t g_be{};
}

extern "C" {

void osal_init(const osal_backend_t *backend)
{
    if (backend != nullptr) {
        g_be = *backend;
    }
}

/* ---- Log ---- */
void osal_log(osal_log_level_t lvl, const char *tag, const char *msg)
{
    if (g_be.log != nullptr) {
        g_be.log(lvl, tag, msg);
    }
}

/* ---- Time ---- */
uint32_t osal_uptime_ms(void)
{
    return (g_be.uptime_ms != nullptr) ? g_be.uptime_ms() : 0u;
}

/* ---- Mutex ---- */
osal_mutex_t osal_mutex_create(void)
{
    return (g_be.mutex_create != nullptr) ? g_be.mutex_create() : nullptr;
}

void osal_mutex_destroy(osal_mutex_t m)
{
    if (g_be.mutex_destroy != nullptr) {
        g_be.mutex_destroy(m);
    }
}

int osal_mutex_lock(osal_mutex_t m, uint32_t timeout_ms)
{
    return (g_be.mutex_lock != nullptr) ? g_be.mutex_lock(m, timeout_ms) : 0;
}

void osal_mutex_unlock(osal_mutex_t m)
{
    if (g_be.mutex_unlock != nullptr) {
        g_be.mutex_unlock(m);
    }
}

/* ---- Memory ---- */
void *osal_mem_alloc(size_t size)
{
    return (g_be.mem_alloc != nullptr) ? g_be.mem_alloc(size) : nullptr;
}

void osal_mem_free(void *ptr)
{
    if (g_be.mem_free != nullptr) {
        g_be.mem_free(ptr);
    }
}

} /* extern "C" */