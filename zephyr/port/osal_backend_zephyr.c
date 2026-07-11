/*
 * Copyright (c) 2025 RobotPilots-SZU
 * SPDX-License-Identifier: Apache-2.0
 *
 * OSAL core backend for Zephyr. This is one of the few places (together with
 * the bus adapters) that includes <zephyr/*.h>. It implements the
 * osal_backend_t services and injects them via osal_init().
 */
#include "osal.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(osal, CONFIG_MIDDLEWARE_OSAL_LOG_LEVEL);

/* ---- Log ---- */
static void be_log(osal_log_level_t lvl, const char *tag, const char *msg)
{
    switch (lvl) {
    case OSAL_LOG_ERR: LOG_ERR("[%s] %s", tag ? tag : "", msg ? msg : ""); break;
    case OSAL_LOG_WRN: LOG_WRN("[%s] %s", tag ? tag : "", msg ? msg : ""); break;
    case OSAL_LOG_INF: LOG_INF("[%s] %s", tag ? tag : "", msg ? msg : ""); break;
    case OSAL_LOG_DBG: LOG_DBG("[%s] %s", tag ? tag : "", msg ? msg : ""); break;
    default:           LOG_INF("[%s] %s", tag ? tag : "", msg ? msg : ""); break;
    }
}

/* ---- Time ---- */
static uint32_t be_uptime_ms(void)
{
    return (uint32_t)k_uptime_get();
}

/* ---- Mutex ----
 * Handle is a pointer to a heap-allocated struct k_mutex.
 */
static osal_mutex_t be_mutex_create(void)
{
    struct k_mutex *m = k_malloc(sizeof(struct k_mutex));
    if (m == NULL) {
        return NULL;
    }
    k_mutex_init(m);
    return (osal_mutex_t)m;
}

static void be_mutex_destroy(osal_mutex_t m)
{
    if (m != NULL) {
        k_free(m);
    }
}

static int be_mutex_lock(osal_mutex_t m, uint32_t timeout_ms)
{
    k_timeout_t t;

    if (m == NULL) {
        return -1;
    }
    if (timeout_ms == OSAL_WAIT_FOREVER) {
        t = K_FOREVER;
    } else if (timeout_ms == 0u) {
        t = K_NO_WAIT;
    } else {
        t = K_MSEC(timeout_ms);
    }
    return k_mutex_lock((struct k_mutex *)m, t);
}

static void be_mutex_unlock(osal_mutex_t m)
{
    if (m != NULL) {
        k_mutex_unlock((struct k_mutex *)m);
    }
}

/* ---- Memory ---- */
static void *be_mem_alloc(size_t size)
{
    return k_malloc(size);
}

static void be_mem_free(void *ptr)
{
    k_free(ptr);
}

/* ---- Injection ---- */
static const osal_backend_t osal_zephyr_backend = {
    .log           = be_log,
    .uptime_ms     = be_uptime_ms,
    .mutex_create  = be_mutex_create,
    .mutex_destroy = be_mutex_destroy,
    .mutex_lock    = be_mutex_lock,
    .mutex_unlock  = be_mutex_unlock,
    .mem_alloc     = be_mem_alloc,
    .mem_free      = be_mem_free,
};

void osal_backend_zephyr_init(void)
{
    osal_init(&osal_zephyr_backend);
}
