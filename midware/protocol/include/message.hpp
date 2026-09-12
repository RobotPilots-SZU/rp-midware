/*
 * Copyright (c) 2025 RobotPilots-SZU
 * SPDX-License-Identifier: Apache-2.0
 *
 * 基于 nanopb 运行时的协议消息编解码封装。
 */
#ifndef PROTOCOL_MESSAGE_HPP
#define PROTOCOL_MESSAGE_HPP

#include <cstddef>
#include <cstdint>
#include <sys/types.h>

#include <pb.h>

namespace protocol
{

/**
 * @brief 将 nanopb 消息编码进调用方提供的缓冲区
 *
 * @param fields   生成的 `<Msg>_fields` 描述符（非空）
 * @param src      已填充的 `<Msg>` 结构体指针（非空）
 * @param buf      目标缓冲区（非空）
 * @param buf_size 缓冲区容量（字节，须 > 0）
 * @return >= 0    成功，返回写入的字节数
 * @return -1       失败（空参数、空缓冲区，或 nanopb 报告的编码/溢出错误），
 *                  失败原因通过 OSAL 日志上报。POSIX write() 风格约定。
 */
ssize_t encode(const pb_msgdesc_t *fields, const void *src,
               uint8_t *buf, size_t buf_size);

/**
 * @brief 将缓冲区中的数据解码为 nanopb 消息结构体
 *
 * @param fields 生成的 `<Msg>_fields` 描述符（非空）
 * @param dst    待填充的 `<Msg>` 结构体指针（非空）
 * @param buf    源缓冲区（非空；可承载零长度消息）
 * @param len    缓冲区中有效字节数
 * @return true  成功
 * @return false 失败（空参数，或 nanopb 报告的解码错误），
 *               失败原因通过 OSAL 日志上报。
 */
bool decode(const pb_msgdesc_t *fields, void *dst,
            const uint8_t *buf, size_t len);

/*
 * 面向定长数组缓冲区的便捷重载。
 * 容量由数组类型自动推导，调用处无需重复填写大小：
 *   protocol::encode(Msg_fields, &m, buf);
 *
 * 注意：decode 的数组重载会按整个数组长度 N 解码，仅适用于
 * “缓冲区正好装满一条消息” 的场景。若只收到 n 个有效字节（如网络包），
 * 请改用显式长度版本 decode(fields, &out, buf, n)。
 */

/**
 * @brief encode 的定长数组便捷重载，容量 N 编译期自动推导
 *
 * @tparam N     数组容量（自动推导）
 * @param fields 生成的 `<Msg>_fields` 描述符（非空）
 * @param src    已填充的 `<Msg>` 结构体指针（非空）
 * @param buf    目标数组缓冲区
 * @return >= 0  成功，返回写入的字节数；失败返回 -1
 */
template <size_t N>
inline ssize_t encode(const pb_msgdesc_t *fields, const void *src,
                      uint8_t (&buf)[N])
{
    return encode(fields, src, buf, N);
}

/**
 * @brief decode 的定长数组便捷重载，按整个数组长度 N 解码
 *
 * @tparam N     数组容量（自动推导）
 * @param fields 生成的 `<Msg>_fields` 描述符（非空）
 * @param dst    待填充的 `<Msg>` 结构体指针（非空）
 * @param buf    源数组缓冲区（须正好装满一条消息）
 * @return true  成功；失败返回 false
 */
template <size_t N>
inline bool decode(const pb_msgdesc_t *fields, void *dst,
                   const uint8_t (&buf)[N])
{
    return decode(fields, dst, buf, N);
}

} /* namespace protocol */

#endif /* PROTOCOL_MESSAGE_HPP */
