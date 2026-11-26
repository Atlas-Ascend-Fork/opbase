/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef AICPU_LRU_CACHE_H
#define AICPU_LRU_CACHE_H

#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "opdev/aicpu/aicpu_task.h"

namespace op {
namespace internal {

struct IdleLruInfo {
    std::list<AicpuTask*>::iterator idleIt;
    size_t seed = 0U;
};
struct SpaceCacheData {
    size_t cachedTaskNum = 0U;
    std::list<AicpuTask*> idleLru;
    std::unordered_map<AicpuTask*, IdleLruInfo> taskInfo;
};
inline std::mutex gSpaceCacheMutex;
inline std::unordered_map<void*, SpaceCacheData>& SpaceCacheDataMap()
{
    static std::unordered_map<void*, SpaceCacheData> instance;
    return instance;
}

inline size_t GetCachedTaskNum(const void* space)
{
    const std::lock_guard<std::mutex> lk(gSpaceCacheMutex);
    const auto& map = SpaceCacheDataMap();
    const auto it = map.find(const_cast<void*>(space));
    return (it == map.end()) ? 0U : it->second.cachedTaskNum;
}

inline void IncCachedTaskNum(const void* space)
{
    const std::lock_guard<std::mutex> lk(gSpaceCacheMutex);
    ++SpaceCacheDataMap()[const_cast<void*>(space)].cachedTaskNum;
}

inline void RegisterCachedTask(void* space, AicpuTask* task, const size_t seed)
{
    const std::lock_guard<std::mutex> lk(gSpaceCacheMutex);
    auto& data = SpaceCacheDataMap()[space];
    const auto [it, inserted] = data.taskInfo.emplace(task, IdleLruInfo{});
    if (!inserted) {
        return;
    }
    data.idleLru.push_front(task);
    it->second.idleIt = data.idleLru.begin();
    it->second.seed = seed;
}

inline void ActivateCachedTask(AicpuTask* task, void* space)
{
    const std::lock_guard<std::mutex> lk(gSpaceCacheMutex);
    auto& map = SpaceCacheDataMap();
    const auto spaceIt = map.find(space);
    if (spaceIt == map.end()) {
        return;
    }
    auto& data = spaceIt->second;
    const auto it = data.taskInfo.find(task);
    if (it == data.taskInfo.end()) {
        return;
    }
    data.idleLru.splice(data.idleLru.begin(), data.idleLru, it->second.idleIt);
    it->second.idleIt = data.idleLru.begin();
}

inline void ClearSpaceCacheData(void* space)
{
    const std::lock_guard<std::mutex> lk(gSpaceCacheMutex);
    (void)SpaceCacheDataMap().erase(space);
}

struct TaskSpaceCtx {
    AicpuTaskSpace* space;
    std::unordered_map<size_t, std::vector<std::unique_ptr<AicpuTask>>>& hashMap;
    std::mutex& mutex;
    bool& hasInit;
    const bool isTf;
    const ge::UnknowShapeOpType unknownType;
    const std::string& opType;
    bool (*isIdle)(const AicpuTask*);
};

bool AicpuCreateTaskUnlocked(const TaskSpaceCtx& taskCache, aclOpExecutor* executor,
                             const FVector<std::string>& attrNames, op::OpArgContext* args,
                             const FVector<const aclTensor*>& inputs, std::unique_ptr<AicpuTask>& task);

std::unique_ptr<AicpuTask> AicpuEvictOneTask(const TaskSpaceCtx& taskCache);

AicpuTask* AicpuCommitTask(const TaskSpaceCtx& taskCache, std::unique_ptr<AicpuTask>& task, const size_t seed,
                           aclOpExecutor* executor, const FVector<const aclTensor*>& inputs, bool& needRegister,
                           std::unique_ptr<AicpuTask>& evictedOwner);

} // namespace internal
} // namespace op

#endif // AICPU_LRU_CACHE_H
