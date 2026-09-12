/*
 * Copyright (c) 2025 RobotPilots-SZU
 * SPDX-License-Identifier: Apache-2.0
 *
 * 协议编解码实现。
 */
#include "message.hpp"

#include <pb_encode.h>
#include <pb_decode.h>

#include "osal.h"

namespace
{
constexpr const char *kTag = "protocol";

/* nanopb 的 errmsg 可能为空串，此处兜底以保证日志有意义。 */
const char *safe_errmsg(const char *msg)
{
    return (msg != nullptr && msg[0] != '\0') ? msg : "unknown nanopb error";
}
} /* namespace */

namespace protocol
{

ssize_t encode(const pb_msgdesc_t *fields, const void *src,
               uint8_t *buf, size_t buf_size)
{
    if (fields == nullptr || src == nullptr) {
        osal_log(OSAL_LOG_ERR, kTag, "encode: null fields/src");
        return -1;
    }
    if (buf == nullptr || buf_size == 0u) {
        osal_log(OSAL_LOG_ERR, kTag, "encode: invalid buffer");
        return -1;
    }

    pb_ostream_t stream = pb_ostream_from_buffer(buf, buf_size);

    if (!pb_encode(&stream, fields, src)) {
        osal_log(OSAL_LOG_ERR, kTag, safe_errmsg(PB_GET_ERROR(&stream)));
        return -1;
    }

    return (ssize_t)stream.bytes_written;
}

bool decode(const pb_msgdesc_t *fields, void *dst,
            const uint8_t *buf, size_t len)
{
    if (fields == nullptr || dst == nullptr) {
        osal_log(OSAL_LOG_ERR, kTag, "decode: null fields/dst");
        return false;
    }
    if (buf == nullptr && len != 0u) {
        osal_log(OSAL_LOG_ERR, kTag, "decode: null buffer with non-zero len");
        return false;
    }

    pb_istream_t stream = pb_istream_from_buffer(buf, len);

    if (!pb_decode(&stream, fields, dst)) {
        osal_log(OSAL_LOG_ERR, kTag, safe_errmsg(PB_GET_ERROR(&stream)));
        return false;
    }

    return true;
}

} /* namespace protocol */
