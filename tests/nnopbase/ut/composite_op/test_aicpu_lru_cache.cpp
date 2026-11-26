/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <memory>
#include <thread>
#include <vector>

#define private public
#define protected public
#include "opdev/aicpu/aicpu_task.h"
#undef private
#undef protected

#include "aicpu_lru_cache.h"

using namespace op::internal;

namespace {
class AicpuLruCacheUt : public testing::Test {
protected:
    void SetUp() override { space = std::make_unique<AicpuTaskSpace>("UtLruOp", ge::DEPEND_IN_SHAPE, false); }

    void TearDown() override { ClearSpaceCacheData(space.get()); }

    std::unique_ptr<AicpuCCTask> MakeRegisteredTask()
    {
        auto task = std::make_unique<AicpuCCTask>("UtLruOp", ge::DEPEND_IN_SHAPE);
        task->SetSpace(space.get());
        RegisterCachedTask(space.get(), task.get(), 0U);
        IncCachedTaskNum(space.get());
        return task;
    }

    TaskSpaceCtx MakeCtx(AicpuTaskSpace& target)
    {
        return TaskSpaceCtx{
            &target,      target.hashMap_,     target.mutex_,  target.hasInit_,
            target.isTf_, target.unknownType_, target.opType_, [](const AicpuTask* t) { return !t->isVisit_; }};
    }

    void EvictViaSpace(AicpuTaskSpace& target)
    {
        const std::lock_guard<std::mutex> lk(target.mutex_);
        (void)AicpuEvictOneTask(MakeCtx(target));
    }

    SpaceCacheData& Data()
    {
        // UT内独占访问（单线程用例），直接加锁读取快照亦可；此处持锁返回引用仅侢单线程断言
        return SpaceCacheDataMap()[space.get()];
    }

    std::unique_ptr<AicpuTaskSpace> space;
};

// 空间不存在时旁路操作安全；无space的task释放仅清标记
TEST_F(AicpuLruCacheUt, UnknownSpaceSafety)
{
    int fakeSpace = 0;
    EXPECT_EQ(GetCachedTaskNum(&fakeSpace), 0U);
    AicpuCCTask task("UtLruOp", ge::DEPEND_IN_SHAPE);
    task.SetVisit(false);
    ActivateCachedTask(&task, &fakeSpace);
    ClearSpaceCacheData(&fakeSpace);
}

// 计数器增减与淘汰联动；占用中的task不可淘汰
TEST_F(AicpuLruCacheUt, CounterWithEvictSkipInUse)
{
    EXPECT_EQ(GetCachedTaskNum(space.get()), 0U);
    auto taskA = MakeRegisteredTask();
    auto taskB = MakeRegisteredTask();
    EXPECT_EQ(GetCachedTaskNum(space.get()), 2U);
    taskA->SetVisit(true);
    EvictViaSpace(*space);
    EXPECT_EQ(GetCachedTaskNum(space.get()), 1U);
    EXPECT_EQ(Data().taskInfo.count(taskA.get()), 1U);
    EXPECT_EQ(Data().taskInfo.count(taskB.get()), 0U);
}

// 注册即入链；释放只清标记不动链；激活刷新到链头
TEST_F(AicpuLruCacheUt, RegisterInChainReleaseKeepsPosition)
{
    auto task = MakeRegisteredTask();
    EXPECT_EQ(Data().idleLru.size(), 1U);
    EXPECT_EQ(Data().idleLru.front(), task.get());

    task->SetVisit(false);
    EXPECT_EQ(task->isVisit_, false);
    EXPECT_EQ(Data().idleLru.size(), 1U);

    auto taskB = MakeRegisteredTask();
    EXPECT_EQ(Data().idleLru.front(), taskB.get());
    ActivateCachedTask(task.get(), space.get());
    EXPECT_EQ(Data().idleLru.front(), task.get());
    EXPECT_EQ(Data().idleLru.size(), 2U);
}

// 重复释放幂等
TEST_F(AicpuLruCacheUt, DoubleReleaseGuard)
{
    auto task = MakeRegisteredTask();
    task->SetVisit(false);
    task->SetVisit(false);
    EXPECT_EQ(Data().idleLru.size(), 1U);
}

// 激活刷新位置到链头；淘汰从链尾扫，跳过占用
TEST_F(AicpuLruCacheUt, ActivateMovesToFrontAndEvictTail)
{
    auto taskA = MakeRegisteredTask();
    auto taskB = MakeRegisteredTask();
    EXPECT_EQ(Data().idleLru.front(), taskB.get());
    ActivateCachedTask(taskA.get(), space.get());
    EXPECT_EQ(Data().idleLru.front(), taskA.get());
    taskA->SetVisit(true);
    EvictViaSpace(*space);
    EXPECT_EQ(Data().taskInfo.count(taskB.get()), 0U);
    EXPECT_EQ(Data().taskInfo.count(taskA.get()), 1U);
}

// 未注册的task释放为no-op
TEST_F(AicpuLruCacheUt, ReleaseUnregisteredTaskNoop)
{
    auto task = std::make_unique<AicpuCCTask>("UtLruOp", ge::DEPEND_IN_SHAPE);
    task->SetSpace(space.get());
    task->SetVisit(false);
    EXPECT_EQ(Data().idleLru.size(), 0U);
    EXPECT_EQ(GetCachedTaskNum(space.get()), 0U);
}

// SetVisit(true)不触碰LRU
TEST_F(AicpuLruCacheUt, SetVisitTrueNoLruInteraction)
{
    auto task = MakeRegisteredTask();
    EXPECT_EQ(Data().idleLru.size(), 1U);
    task->SetVisit(true);
    EXPECT_EQ(task->isVisit_, true);
    EXPECT_EQ(Data().idleLru.size(), 1U);
    EXPECT_EQ(Data().idleLru.front(), task.get());
}

// space_为空时SetVisit(false)仅清标记
TEST_F(AicpuLruCacheUt, SetVisitNullSpaceSafe)
{
    auto task = std::make_unique<AicpuCCTask>("UtLruOp", ge::DEPEND_IN_SHAPE);
    task->SetVisit(true);
    task->SetVisit(false);
    EXPECT_EQ(task->isVisit_, false);
}

// 多space隔离
TEST_F(AicpuLruCacheUt, MultiSpaceIsolation)
{
    AicpuTaskSpace otherSpace("UtLruOtherOp", ge::DEPEND_IN_SHAPE, false);
    auto taskA = MakeRegisteredTask();
    auto taskB = std::make_unique<AicpuCCTask>("UtLruOtherOp", ge::DEPEND_IN_SHAPE);
    taskB->SetSpace(&otherSpace);
    RegisterCachedTask(&otherSpace, taskB.get(), 0U);
    IncCachedTaskNum(&otherSpace);

    taskA->SetVisit(true);
    taskB->SetVisit(false);
    EXPECT_EQ(GetCachedTaskNum(space.get()), 1U);
    EXPECT_EQ(GetCachedTaskNum(&otherSpace), 1U);

    EvictViaSpace(otherSpace);
    EXPECT_EQ(GetCachedTaskNum(&otherSpace), 0U);
    EXPECT_EQ(GetCachedTaskNum(space.get()), 1U);
    EXPECT_EQ(Data().taskInfo.count(taskA.get()), 1U);
    ClearSpaceCacheData(&otherSpace);
}

// Clear清除全部旁路状态；空间清除后SetVisit仅清标记，安全no-op
TEST_F(AicpuLruCacheUt, ClearRemovesAll)
{
    auto task = MakeRegisteredTask();
    task->SetVisit(false);
    ClearSpaceCacheData(space.get());
    EXPECT_EQ(GetCachedTaskNum(space.get()), 0U);
    task->SetVisit(false);
    EXPECT_EQ(task->isVisit_, false);
    const std::lock_guard<std::mutex> lk(space->mutex_);
    EXPECT_EQ(AicpuEvictOneTask(MakeCtx(*space)), nullptr);
}

// 重复注册幂等
TEST_F(AicpuLruCacheUt, RegisterIdempotent)
{
    auto task = std::make_unique<AicpuCCTask>("UtLruOp", ge::DEPEND_IN_SHAPE);
    task->SetSpace(space.get());
    RegisterCachedTask(space.get(), task.get(), 0U);
    RegisterCachedTask(space.get(), task.get(), 0U);
    EXPECT_EQ(Data().taskInfo.size(), 1U);
    EXPECT_EQ(Data().idleLru.size(), 1U);
}

// 并发压力：占用声明与释放(space锁内) vs 淘汰(space锁内扫描)，验证锁序与状态一致
TEST_F(AicpuLruCacheUt, ConcurrentReleaseEvictStress)
{
    constexpr size_t kTaskNum = 32U;
    constexpr int kWorkerPairs = 4;
    std::vector<std::unique_ptr<AicpuCCTask>> tasks;
    for (size_t i = 0U; i < kTaskNum; ++i) {
        tasks.push_back(MakeRegisteredTask());
    }

    std::atomic<bool> stop{false};
    std::vector<std::thread> pool;
    for (int w = 0; w < kWorkerPairs; ++w) {
        pool.emplace_back([&stop, &tasks, this]() {
            for (size_t i = 0U; !stop.load(std::memory_order_relaxed); ++i) {
                AicpuTask* const task = tasks[i % tasks.size()].get();
                {
                    const std::lock_guard<std::mutex> lk(space->mutex_);
                    task->SetVisit(true);
                    ActivateCachedTask(task, space.get());
                }
                task->SetVisit(false);
            }
        });
        pool.emplace_back([&stop, this]() {
            while (!stop.load(std::memory_order_relaxed)) {
                const std::lock_guard<std::mutex> lk(space->mutex_); // 对齐生产淘汰路径的持锁方式
                (void)AicpuEvictOneTask(MakeCtx(*space));
            }
        });
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    stop.store(true);
    for (auto& th : pool) {
        th.join();
    }

    // 收敛后校验不变式：链长==索引数、计数一致
    const std::lock_guard<std::mutex> lk(space->mutex_);
    const std::lock_guard<std::mutex> lk2(gSpaceCacheMutex);
    const SpaceCacheData& data = Data();
    EXPECT_EQ(data.idleLru.size(), data.taskInfo.size());
    EXPECT_EQ(data.cachedTaskNum, data.taskInfo.size());
    for (AicpuTask* const t : data.idleLru) {
        EXPECT_NE(data.taskInfo.find(t), data.taskInfo.end());
    }
}
} // namespace
