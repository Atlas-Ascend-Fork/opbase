/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "aicpu_workspace_tiling.h"

#include <new>

namespace op::internal {
AicpuWorkspaceTilingRegistry::AicpuWorkspaceTilingRegistry()
{
    for (size_t index = 0U; index < tilingFuncs_.size(); ++index) {
        tilingFuncs_[index].store(nullptr, std::memory_order_relaxed);
        lookupDone_[index].store(false, std::memory_order_relaxed);
    }
}

AicpuWorkspaceTilingRegistry& AicpuWorkspaceTilingRegistry::GetInstance()
{
    static AicpuWorkspaceTilingRegistry registry;
    return registry;
}

bool AicpuWorkspaceTilingRegistry::RegisterByName(const char* opName, AicpuWorkspaceTilingFunc tiling)
{
    if (opName == nullptr || opName[0] == '\0' || tiling == nullptr) {
        OP_LOGE(ACLNN_ERR_PARAM_INVALID, "Invalid AICPU workspace tiling name or function.");
        return false;
    }
    try {
        const std::string name(opName);
        AicpuWorkspaceTilingFunc registeredTiling = nullptr;
        {
            const std::lock_guard<std::mutex> lock(mutex_);
            // Match CpuKernelRegister: the first registration owns the operator name.
            // Equivalent callbacks in separate DSOs may have different addresses.
            registeredTiling = namedFuncs_.try_emplace(name, tiling).first->second;
        }
        // Publish the name before resolving its ID, without nesting the registry and dictionary locks.
        // A concurrent first query can then resolve the function even if the ID is allocated after this lookup.
        const auto opType = OpTypeDict::ToOpType(name);
        if (opType != 0U && opType < tilingFuncs_.size()) {
            const std::lock_guard<std::mutex> lock(mutex_);
            tilingFuncs_[opType].store(registeredTiling, std::memory_order_release);
            lookupDone_[opType].store(true, std::memory_order_release);
        }
        return true;
    } catch (const std::bad_alloc&) {
        OP_LOGE(ACLNN_ERR_INNER, "Failed to allocate AICPU workspace tiling registration for op %s.", opName);
        return false;
    }
}

AicpuWorkspaceTilingFunc AicpuWorkspaceTilingRegistry::FindSlow(uint32_t opType)
{
    if (opType == 0U || opType >= tilingFuncs_.size() || opType >= OpTypeDict::GetAllOpTypeSize()) {
        return nullptr;
    }
    const auto name = OpTypeDict::ToString(opType);
    const std::lock_guard<std::mutex> lock(mutex_);
    const auto current = tilingFuncs_[opType].load(std::memory_order_relaxed);
    if (current != nullptr) {
        return current;
    }
    const auto found = namedFuncs_.find(name.GetString());
    const auto tiling = found == namedFuncs_.end() ? nullptr : found->second;
    tilingFuncs_[opType].store(tiling, std::memory_order_release);
    lookupDone_[opType].store(true, std::memory_order_release);
    return tiling;
}

bool RegisterAicpuWorkspaceTilingByName(const char* opName, AicpuWorkspaceTilingFunc tiling)
{
    return AicpuWorkspaceTilingRegistry::GetInstance().RegisterByName(opName, tiling);
}

} // namespace op::internal
