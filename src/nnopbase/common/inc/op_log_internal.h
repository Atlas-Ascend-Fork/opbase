/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file op_log_internal.h
 * \brief 测试期可抑制的日志宏：仅 UT/ST 构建存在开关，生产构建等价于直接打印
 */
#ifndef OP_LOG_INTERNAL_H_
#define OP_LOG_INTERNAL_H_

#if defined(NNOPBASE_UT) || defined(NNOPBASE_ST)
#include <atomic>

namespace op {
namespace internal {
// UT/ST 压测用例可短时抑制该日志；默认开启，生产构建不包含此开关
inline std::atomic<bool>& TestSuppressibleLogEnabled()
{
    static std::atomic<bool> enabled{true};
    return enabled;
}
} // namespace internal
} // namespace op

// 与下方生产分支保持同步：两份定义仅差开关判断
#define OP_LOG_TEST_SUPPRESSIBLE(logFunc)                 \
    do {                                                  \
        if (op::internal::TestSuppressibleLogEnabled()) { \
            logFunc;                                      \
        }                                                 \
    } while (false)
#else
// 生产构建：等价于直接调用日志宏，无开关、无额外开销
#define OP_LOG_TEST_SUPPRESSIBLE(logFunc) \
    do {                                  \
        logFunc;                          \
    } while (false)
#endif

#endif // OP_LOG_INTERNAL_H_
