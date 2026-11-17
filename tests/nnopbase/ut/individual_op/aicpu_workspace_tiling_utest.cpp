/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <atomic>
#include <chrono>
#include <future>
#include <limits>
#include <thread>
#include <vector>

#include "gtest/gtest.h"
#include "aicpu_workspace_tiling.h"
#include "opdev/op_dfx.h"

using op::internal::AicpuWorkspaceTilingRegistry;

namespace {
constexpr size_t kRegistrationCount = 4096U;
constexpr size_t kThreadCount = 16U;
constexpr size_t kQueryCount = 1000U;
constexpr uint64_t kFirstWorkspaceSize = 32U;
constexpr uint64_t kOtherWorkspaceSize = 64U;
constexpr auto kQueryTimeout = std::chrono::seconds(2);

aclnnStatus FirstTiling(op::OpArgList&, op::OpArgList&, op::OpArgList&, uint64_t& workspaceSize)
{
    workspaceSize = kFirstWorkspaceSize;
    return ACLNN_SUCCESS;
}

aclnnStatus OtherTiling(op::OpArgList&, op::OpArgList&, op::OpArgList&, uint64_t& workspaceSize)
{
    workspaceSize = kOtherWorkspaceSize;
    return ACLNN_SUCCESS;
}

// No OpTypeId function is declared: registration must not require or allocate an operator ID.
REGISTER_AICPU_WORKSPACE_TILING(AicpuRegistryMacroProbe, FirstTiling);
} // namespace

TEST(AicpuWorkspaceTilingRegistryTest, MacroDoesNotAllocateId)
{
    EXPECT_EQ(op::OpTypeDict::ToOpType("AicpuRegistryMacroProbe"), 0U);
    const auto id = op::GenOpTypeId("AicpuRegistryMacroProbe");
    ASSERT_NE(id, 0U);
    EXPECT_EQ(AicpuWorkspaceTilingRegistry::GetInstance().Find(id), FirstTiling);
}

TEST(AicpuWorkspaceTilingRegistryTest, InvalidRegistration)
{
    AicpuWorkspaceTilingRegistry registry;
    EXPECT_FALSE(registry.RegisterByName(nullptr, FirstTiling));
    EXPECT_FALSE(registry.RegisterByName("", FirstTiling));
    EXPECT_FALSE(registry.RegisterByName("AicpuRegistryInvalid", nullptr));
    EXPECT_EQ(registry.Find(0U), nullptr);
    EXPECT_EQ(registry.Find(op::internal::MAX_OP_TYPE_COUNT), nullptr);
    EXPECT_EQ(registry.Find(std::numeric_limits<uint32_t>::max()), nullptr);
    EXPECT_EQ(registry.FindSlow(0U), nullptr);
    EXPECT_EQ(registry.FindSlow(op::internal::MAX_OP_TYPE_COUNT), nullptr);
    EXPECT_EQ(registry.FindSlow(std::numeric_limits<uint32_t>::max()), nullptr);
    EXPECT_TRUE(registry.namedFuncs_.empty());
}

TEST(AicpuWorkspaceTilingRegistryTest, MoreNamesThanIdCacheWithoutAllocatingIds)
{
    AicpuWorkspaceTilingRegistry registry;
    const auto countBefore = op::OpTypeDict::GetAllOpTypeSize();
    for (size_t index = 0U; index < kRegistrationCount; ++index) {
        const auto name = "AicpuRegistryBulk" + std::to_string(index);
        ASSERT_TRUE(registry.RegisterByName(name.c_str(), FirstTiling));
    }
    EXPECT_EQ(op::OpTypeDict::GetAllOpTypeSize(), countBefore);
    EXPECT_EQ(registry.namedFuncs_.size(), kRegistrationCount);
    const auto first = op::GenOpTypeId("AicpuRegistryBulk0");
    const auto lastName = "AicpuRegistryBulk" + std::to_string(kRegistrationCount - 1U);
    const auto last = op::GenOpTypeId(lastName.c_str());
    ASSERT_NE(first, 0U);
    ASSERT_NE(last, 0U);
    EXPECT_EQ(registry.Find(first), FirstTiling);
    EXPECT_EQ(registry.Find(last), FirstTiling);
}

TEST(AicpuWorkspaceTilingRegistryTest, DuplicateNameRetainsFirstCallback)
{
    AicpuWorkspaceTilingRegistry registry;
    ASSERT_TRUE(registry.RegisterByName("AicpuRegistryDuplicate", FirstTiling));
    for (size_t index = 0U; index < kQueryCount; ++index) {
        ASSERT_TRUE(registry.RegisterByName("AicpuRegistryDuplicate", FirstTiling));
    }
    EXPECT_TRUE(registry.RegisterByName("AicpuRegistryDuplicate", OtherTiling));
    EXPECT_EQ(registry.namedFuncs_.size(), 1U);
    const auto id = op::GenOpTypeId("AicpuRegistryDuplicate");
    ASSERT_NE(id, 0U);
    EXPECT_EQ(registry.Find(id), FirstTiling);
    EXPECT_TRUE(registry.RegisterByName("AicpuRegistryDuplicate", OtherTiling));
    EXPECT_EQ(registry.Find(id), FirstTiling);
}

TEST(AicpuWorkspaceTilingRegistryTest, LateRegistrationUpdatesCachedMiss)
{
    AicpuWorkspaceTilingRegistry registry;
    const auto id = op::GenOpTypeId("AicpuRegistryLate");
    ASSERT_NE(id, 0U);
    EXPECT_EQ(registry.Find(id), nullptr);
    EXPECT_TRUE(registry.lookupDone_[id].load());
    ASSERT_TRUE(registry.RegisterByName("AicpuRegistryLate", FirstTiling));
    EXPECT_EQ(registry.Find(id), FirstTiling);
}

TEST(AicpuWorkspaceTilingRegistryTest, RegistrationBeforeAndAfterIdAllocation)
{
    AicpuWorkspaceTilingRegistry registry;
    ASSERT_TRUE(registry.RegisterByName("AicpuRegistryBeforeId", FirstTiling));
    EXPECT_EQ(op::OpTypeDict::ToOpType("AicpuRegistryBeforeId"), 0U);
    const auto beforeId = op::GenOpTypeId("AicpuRegistryBeforeId");
    const auto afterId = op::GenOpTypeId("AicpuRegistryAfterId");
    ASSERT_NE(beforeId, 0U);
    ASSERT_NE(afterId, 0U);
    ASSERT_TRUE(registry.RegisterByName("AicpuRegistryAfterId", OtherTiling));
    EXPECT_EQ(registry.Find(beforeId), FirstTiling);
    EXPECT_EQ(registry.Find(afterId), OtherTiling);
}

TEST(AicpuWorkspaceTilingRegistryTest, CachedQueriesDoNotAcquireRegistryMutex)
{
    AicpuWorkspaceTilingRegistry registry;
    const auto registered = op::GenOpTypeId("AicpuRegistryCachedHit");
    const auto missing = op::GenOpTypeId("AicpuRegistryCachedMiss");
    ASSERT_NE(registered, 0U);
    ASSERT_NE(missing, 0U);
    ASSERT_TRUE(registry.RegisterByName("AicpuRegistryCachedHit", FirstTiling));
    EXPECT_EQ(registry.Find(registered), FirstTiling);
    EXPECT_EQ(registry.Find(missing), nullptr);
    std::unique_lock<std::mutex> lock(registry.mutex_);
    auto query = std::async(std::launch::async, [&registry, registered, missing]() {
        return registry.Find(registered) == FirstTiling && registry.Find(missing) == nullptr;
    });
    const auto status = query.wait_for(kQueryTimeout);
    lock.unlock();
    EXPECT_EQ(status, std::future_status::ready);
    EXPECT_TRUE(query.get());
}

TEST(AicpuWorkspaceTilingRegistryTest, ConcurrentRegistrationAndFirstQueries)
{
    AicpuWorkspaceTilingRegistry registry;
    const auto id = op::GenOpTypeId("AicpuRegistryConcurrent");
    ASSERT_NE(id, 0U);
    std::atomic<bool> start{false};
    std::atomic<size_t> errors{0U};
    std::vector<std::thread> threads;
    for (size_t index = 0U; index < kThreadCount; ++index) {
        threads.emplace_back([&registry, &start, &errors, id]() {
            while (!start.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            for (size_t query = 0U; query < kQueryCount; ++query) {
                const bool registered = registry.RegisterByName("AicpuRegistryConcurrent", FirstTiling);
                if (!registered || registry.Find(id) != FirstTiling) {
                    errors.fetch_add(1U, std::memory_order_relaxed);
                }
            }
        });
    }
    start.store(true, std::memory_order_release);
    for (auto& thread : threads) {
        thread.join();
    }
    EXPECT_EQ(errors.load(), 0U);
    EXPECT_EQ(registry.namedFuncs_.size(), 1U);
}

TEST(AicpuWorkspaceTilingRegistryTest, ConcurrentMissAndLateRegistration)
{
    AicpuWorkspaceTilingRegistry registry;
    const auto id = op::GenOpTypeId("AicpuRegistryConcurrentLate");
    ASSERT_NE(id, 0U);
    std::atomic<bool> start{false};
    std::atomic<size_t> errors{0U};
    std::atomic<bool> registered{false};
    std::vector<std::thread> threads;
    for (size_t index = 0U; index < kThreadCount; ++index) {
        threads.emplace_back([&registry, &start, &errors, &registered, id]() {
            while (!start.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            for (size_t query = 0U; query < kQueryCount; ++query) {
                const auto tiling = registry.Find(id);
                if (tiling != nullptr && tiling != FirstTiling) {
                    errors.fetch_add(1U, std::memory_order_relaxed);
                }
            }
            while (!registered.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            for (size_t query = 0U; query < kQueryCount; ++query) {
                if (registry.Find(id) != FirstTiling) {
                    errors.fetch_add(1U, std::memory_order_relaxed);
                }
            }
        });
    }
    start.store(true, std::memory_order_release);
    EXPECT_TRUE(registry.RegisterByName("AicpuRegistryConcurrentLate", FirstTiling));
    registered.store(true, std::memory_order_release);
    for (auto& thread : threads) {
        thread.join();
    }
    EXPECT_EQ(errors.load(), 0U);
    EXPECT_EQ(registry.Find(id), FirstTiling);
}

TEST(AicpuWorkspaceTilingRegistryTest, RegistrationOwnsName)
{
    AicpuWorkspaceTilingRegistry registry;
    std::string name = "AicpuRegistryOwnedName";
    ASSERT_TRUE(registry.RegisterByName(name.c_str(), FirstTiling));
    name.clear();
    const auto id = op::GenOpTypeId("AicpuRegistryOwnedName");
    ASSERT_NE(id, 0U);
    EXPECT_EQ(registry.Find(id), FirstTiling);
}

TEST(AicpuWorkspaceTilingRegistryTest, ConcurrentDuplicateRegistrationRetainsOneCallback)
{
    AicpuWorkspaceTilingRegistry registry;
    const auto id = op::GenOpTypeId("AicpuRegistryConcurrentConflict");
    ASSERT_NE(id, 0U);
    std::atomic<bool> start{false};
    std::atomic<size_t> firstSuccess{0U};
    std::atomic<size_t> otherSuccess{0U};
    std::vector<std::thread> threads;
    for (size_t index = 0U; index < kThreadCount; ++index) {
        threads.emplace_back([&registry, &start, &firstSuccess, &otherSuccess, index]() {
            while (!start.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            const bool first = index % 2U == 0U;
            if (registry.RegisterByName("AicpuRegistryConcurrentConflict", first ? FirstTiling : OtherTiling)) {
                (first ? firstSuccess : otherSuccess).fetch_add(1U, std::memory_order_relaxed);
            }
        });
    }
    start.store(true, std::memory_order_release);
    for (auto& thread : threads) {
        thread.join();
    }
    const auto winner = registry.Find(id);
    ASSERT_TRUE(winner == FirstTiling || winner == OtherTiling);
    EXPECT_EQ(firstSuccess.load(), kThreadCount / 2U);
    EXPECT_EQ(otherSuccess.load(), kThreadCount / 2U);
    EXPECT_EQ(registry.namedFuncs_.at("AicpuRegistryConcurrentConflict"), winner);
    EXPECT_EQ(registry.tilingFuncs_[id].load(), winner);
    for (size_t index = 0U; index < kQueryCount; ++index) {
        EXPECT_EQ(registry.Find(id), winner);
    }
    EXPECT_EQ(registry.namedFuncs_.size(), 1U);
}

TEST(AicpuWorkspaceTilingRegistryTest, DuplicateRetainsFirstCallbackAfterCachedMiss)
{
    AicpuWorkspaceTilingRegistry registry;
    const auto id = op::GenOpTypeId("AicpuRegistryDuplicateAfterMiss");
    ASSERT_NE(id, 0U);
    EXPECT_EQ(registry.Find(id), nullptr);
    ASSERT_TRUE(registry.RegisterByName("AicpuRegistryDuplicateAfterMiss", FirstTiling));
    ASSERT_TRUE(registry.RegisterByName("AicpuRegistryDuplicateAfterMiss", OtherTiling));
    const auto callback = registry.Find(id);
    ASSERT_EQ(callback, FirstTiling);
    op::OpArgList args{};
    uint64_t workspaceSize = 0U;
    EXPECT_EQ(callback(args, args, args, workspaceSize), ACLNN_SUCCESS);
    EXPECT_EQ(workspaceSize, kFirstWorkspaceSize);
}

TEST(AicpuWorkspaceTilingRegistryTest, DuplicateRegistrationReverseOrder)
{
    AicpuWorkspaceTilingRegistry registry;
    const auto id = op::GenOpTypeId("AicpuRegistryDuplicateReverseOrder");
    ASSERT_NE(id, 0U);
    ASSERT_TRUE(registry.RegisterByName("AicpuRegistryDuplicateReverseOrder", OtherTiling));
    ASSERT_TRUE(registry.RegisterByName("AicpuRegistryDuplicateReverseOrder", FirstTiling));
    const auto callback = registry.Find(id);
    ASSERT_EQ(callback, OtherTiling);
    EXPECT_EQ(registry.namedFuncs_.size(), 1U);
    EXPECT_EQ(registry.tilingFuncs_[id].load(), callback);
    op::OpArgList args{};
    uint64_t workspaceSize = 0U;
    EXPECT_EQ(callback(args, args, args, workspaceSize), ACLNN_SUCCESS);
    EXPECT_EQ(workspaceSize, kOtherWorkspaceSize);
}
