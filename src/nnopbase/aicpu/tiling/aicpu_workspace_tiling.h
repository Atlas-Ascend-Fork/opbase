/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef OP_INTERNAL_AICPU_WORKSPACE_TILING_H_
#define OP_INTERNAL_AICPU_WORKSPACE_TILING_H_

#include <array>
#include <atomic>
#include <map>
#include <mutex>
#include <string>

#include "opdev/aicpu/aicpu_task.h"
#include "kernel_utils.h"

namespace op::internal {
// Internal implementation only; this header is not installed with the CANN public headers.
class AicpuWorkspaceTilingRegistry final {
public:
    AicpuWorkspaceTilingRegistry();
    static AicpuWorkspaceTilingRegistry& GetInstance();
    // Duplicate names retain the first callback, including registrations from different DSOs.
    bool RegisterByName(const char* opName, AicpuWorkspaceTilingFunc tiling);

    AicpuWorkspaceTilingFunc Find(uint32_t opType)
    {
        if (opType == 0U || opType >= tilingFuncs_.size()) {
            return nullptr;
        }
        // The pointer has its own acquire; lookupDone_ only avoids repeated map misses.
        // A lookup overlapping registration may miss; order registration before use to require the new callback.
        const auto tiling = tilingFuncs_[opType].load(std::memory_order_acquire);
        if (tiling != nullptr || lookupDone_[opType].load(std::memory_order_acquire)) {
            return tiling;
        }
        return FindSlow(opType);
    }

private:
    AicpuWorkspaceTilingFunc FindSlow(uint32_t opType);

    // After construction, all cache writes and map accesses hold mutex_; cache reads do not acquire mutex_.
    // A published non-null callback is never replaced or reset. Late registration may replace a cached miss.
    // No OpTypeDict call may run while holding mutex_. Register the name before looking up its existing ID.
    std::array<std::atomic<AicpuWorkspaceTilingFunc>, MAX_OP_TYPE_COUNT> tilingFuncs_;
    std::array<std::atomic<bool>, MAX_OP_TYPE_COUNT> lookupDone_;
    std::mutex mutex_;
    std::map<std::string, AicpuWorkspaceTilingFunc, std::less<>> namedFuncs_;
};
} // namespace op::internal

#endif // OP_INTERNAL_AICPU_WORKSPACE_TILING_H_
