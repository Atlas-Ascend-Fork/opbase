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
#include <array>
#include <cstdlib>
#include <limits>
#include <memory>
#include <string>
#include <tuple>

#include "aicpu_json_load_manager.h"
#include "aicpu_lru_cache.h"
#include "aicpu_workspace_tiling.h"
#include "opdev/op_dfx.h"

namespace op::internal {
std::unique_ptr<AicpuTask> TakePendingOverflowTask(AicpuTask* task);
}

namespace {
constexpr uint64_t kRawWorkspaceBytes = 32U;
constexpr uint64_t kWorkspaceOffset = 64U;
constexpr size_t kUserWorkspaceBytes = 1024U;
constexpr uint64_t kRuntimeCacheBytes = 512U;
constexpr int64_t kInputElements = 4;
constexpr size_t kSmallCacheLimit = 2U;
uint64_t gTilingCalls = 0U;

aclnnStatus WorkspaceTiling(op::OpArgList&, op::OpArgList&, op::OpArgList&, uint64_t& size)
{
    ++gTilingCalls;
    size = kRawWorkspaceBytes;
    return ACLNN_SUCCESS;
}

aclnnStatus ZeroTiling(op::OpArgList&, op::OpArgList&, op::OpArgList&, uint64_t& size)
{
    size = 0U;
    return ACLNN_SUCCESS;
}

aclnnStatus FailedTiling(op::OpArgList&, op::OpArgList&, op::OpArgList&, uint64_t&) { return ACLNN_ERR_PARAM_INVALID; }

aclnnStatus OversizedTiling(op::OpArgList&, op::OpArgList&, op::OpArgList&, uint64_t& size)
{
    size = std::numeric_limits<uint64_t>::max();
    return ACLNN_SUCCESS;
}

bool HasCacheLimit(const char* expected)
{
    const auto* value = std::getenv("ACLNN_CACHE_LIMIT");
    return value != nullptr && std::string(value) == expected;
}

using namespace op::internal;

class AicpuWorkspaceTaskTest : public testing::Test {
protected:
    void SetUp() override
    {
        savedLoader_ = std::make_tuple(JsonLoadManger::hasAicpuLoadBin_, JsonLoadManger::aicpuBinHandle_,
                                       JsonLoadManger::hasTfLoadBin_, JsonLoadManger::tfBinHandle_,
                                       JsonLoadManger::socVersion_, JsonLoadManger::aicpuCustLoadFlag_);
        JsonLoadManger::hasAicpuLoadBin_ = true;
        JsonLoadManger::aicpuBinHandle_ = this;
        JsonLoadManger::hasTfLoadBin_ = true;
        JsonLoadManger::tfBinHandle_ = this;
        JsonLoadManger::socVersion_ = "workspace-test";
        JsonLoadManger::aicpuCustLoadFlag_ = true;
        executor_ = std::make_unique<aclOpExecutor>();
        args_.reset(op::GetOpArgContext(OP_INPUT(&input_), OP_OUTPUT(&output_)));
        space_ = std::make_unique<AicpuTaskSpace>("WorkspaceTaskTest");
    }

    void TearDown() override
    {
        args_.reset();
        executor_.reset();
        aclnnAicpuFinalize();
        std::tie(JsonLoadManger::hasAicpuLoadBin_, JsonLoadManger::aicpuBinHandle_, JsonLoadManger::hasTfLoadBin_,
                 JsonLoadManger::tfBinHandle_, JsonLoadManger::socVersion_,
                 JsonLoadManger::aicpuCustLoadFlag_) = savedLoader_;
    }

    AicpuTask* Create(bool workspace)
    {
        return space_->GetOrCreateTaskImpl(executor_.get(), attrNames_, args_.get(), workspace);
    }

    aclTensor* AddWorkspace()
    {
        auto* tensor = executor_->AllocTensor(op::Shape({kRawWorkspaceBytes}), op::DataType::DT_UINT8);
        if (tensor != nullptr) {
            auto* list = executor_->AllocTensorList(&tensor, 1U);
            if (list == nullptr) {
                return nullptr;
            }
            args_->AppendOpWorkspaceArg(list);
        }
        return tensor;
    }

    aclTensor input_{op::Shape({kInputElements}), op::DataType::DT_DOUBLE, op::Format::FORMAT_ND, nullptr};
    aclTensor output_{op::Shape({kInputElements}), op::DataType::DT_DOUBLE, op::Format::FORMAT_ND, nullptr};
    const op::FVector<std::string> attrNames_;
    std::unique_ptr<aclOpExecutor> executor_;
    std::unique_ptr<AicpuTaskSpace> space_;
    std::unique_ptr<op::OpArgContext, decltype(&op::DestroyOpArgContext)> args_{nullptr, &op::DestroyOpArgContext};
    std::tuple<bool, aclrtBinHandle, bool, aclrtBinHandle, std::string, bool> savedLoader_;
};

TEST_F(AicpuWorkspaceTaskTest, InitAndCacheSeparateExtensionLayouts)
{
    auto* legacy = Create(false);
    ASSERT_NE(legacy, nullptr);
    EXPECT_FALSE(legacy->extInfoHandle_->HasWorkspaceInfo());
    legacy->SetVisit(false);
    auto* workspace = Create(true);
    ASSERT_NE(workspace, nullptr);
    EXPECT_NE(workspace, legacy);
    ASSERT_TRUE(workspace->extInfoHandle_->HasWorkspaceInfo());
    workspace->SetVisit(false);
    EXPECT_EQ(Create(false), legacy);
    EXPECT_EQ(Create(true), workspace);
    EXPECT_EQ(GetCachedTaskNum(space_.get()), 2U);
}

TEST_F(AicpuWorkspaceTaskTest, LegacyEntryKeepsOriginalKey)
{
    auto* task = space_->GetOrCreateTask(executor_.get(), attrNames_, args_.get());
    ASSERT_NE(task, nullptr);
    task->SetVisit(false);
    op::FVector<const aclTensor*> inputs{&input_};
    EXPECT_EQ(space_->FindTask(executor_.get(), args_.get(), inputs), task);
    EXPECT_FALSE(task->extInfoHandle_->HasWorkspaceInfo());
    EXPECT_EQ(space_->FindTaskImpl(executor_.get(), args_.get(), inputs, true), nullptr);
}

TEST_F(AicpuWorkspaceTaskTest, WorkspaceRecordPreservesOpNameAndRejectsInvalidUpdates)
{
    auto* task = Create(true);
    ASSERT_NE(task, nullptr);
    const auto* first = PtrToPtr<uint8_t, AicpuExtInfo>(task->argsHandle_->GetExtInfoAddr());
    EXPECT_EQ(first->infoType, aicpu::FWKAdapter::FWK_ADPT_EXT_OP_NAME);
    EXPECT_NE(task->extInfoHandle_->UpdateWorkspaceInfo(0U, kWorkspaceOffset), ACLNN_SUCCESS);
    EXPECT_NE(task->extInfoHandle_->UpdateWorkspaceInfo(kRawWorkspaceBytes, 0U), ACLNN_SUCCESS);
    EXPECT_EQ(task->extInfoHandle_->UpdateWorkspaceInfo(kRawWorkspaceBytes, kWorkspaceOffset), ACLNN_SUCCESS);
}

TEST_F(AicpuWorkspaceTaskTest, TensorFlowKeepsOriginalExtensionLayout)
{
    AicpuTaskSpace tfSpace("WorkspaceTaskTestTf", ge::DEPEND_IN_SHAPE, true);
    auto* task = tfSpace.GetOrCreateTask(executor_.get(), attrNames_, args_.get());
    ASSERT_NE(task, nullptr);
    EXPECT_FALSE(task->extInfoHandle_->HasWorkspaceInfo());
    task->SetVisit(false);
    aclnnAicpuFinalize();
}

TEST_F(AicpuWorkspaceTaskTest, ReusedTaskRefreshesRawAddressAndLength)
{
    auto* tensor = AddWorkspace();
    ASSERT_NE(tensor, nullptr);
    auto* task = Create(true);
    ASSERT_NE(task, nullptr);
    std::array<uint8_t, kUserWorkspaceBytes> first{};
    std::array<uint8_t, kUserWorkspaceBytes> second{};
    executor_->UpdateTensorAddr(first.data(), first.size());
    executor_->workspaceDeviceAicpuTaskOffset_ = first.size();
    tensor->SetWorkspaceOffset(kWorkspaceOffset);
    tensor->SetStorageAddr(first.data());
    ASSERT_EQ(task->SetIoTensors(executor_.get(), args_.get()), ACLNN_SUCCESS);
    auto* record = static_cast<aicpu::FWKAdapter::WorkSpaceInfo*>(task->extInfoHandle_->workspace_);
    ASSERT_NE(record, nullptr);
    EXPECT_EQ(record->addr, PtrToValue(first.data() + kWorkspaceOffset));
    EXPECT_EQ(record->size, kRawWorkspaceBytes);
    EXPECT_NE(record->size, first.size());
    task->SetVisit(false);
    ASSERT_EQ(Create(true), task);
    tensor->SetStorageShape(op::Shape({kRawWorkspaceBytes * 2U}));
    tensor->SetViewShape(op::Shape({kRawWorkspaceBytes * 2U}));
    executor_->UpdateTensorAddr(second.data(), second.size());
    executor_->workspaceDeviceAicpuTaskOffset_ = second.size();
    tensor->SetWorkspaceOffset(kWorkspaceOffset);
    tensor->SetStorageAddr(second.data());
    ASSERT_EQ(task->SetIoTensors(executor_.get(), args_.get()), ACLNN_SUCCESS);
    EXPECT_EQ(record->addr, PtrToValue(second.data() + kWorkspaceOffset));
    EXPECT_EQ(record->size, kRawWorkspaceBytes * 2U);
}

TEST_F(AicpuWorkspaceTaskTest, RejectsWorkspaceAddressMismatch)
{
    auto* tensor = AddWorkspace();
    ASSERT_NE(tensor, nullptr);
    auto* task = Create(true);
    ASSERT_NE(task, nullptr);
    std::array<uint8_t, kUserWorkspaceBytes> bytes{};
    executor_->UpdateTensorAddr(bytes.data(), bytes.size());
    executor_->workspaceDeviceAicpuTaskOffset_ = bytes.size();
    tensor->SetWorkspaceOffset(kWorkspaceOffset);
    tensor->SetStorageAddr(bytes.data() + 1U);
    EXPECT_NE(task->SetIoTensors(executor_.get(), args_.get()), ACLNN_SUCCESS);
}

TEST_F(AicpuWorkspaceTaskTest, RejectsWorkspaceInRuntimeCacheTail)
{
    auto* tensor = AddWorkspace();
    ASSERT_NE(tensor, nullptr);
    auto* task = Create(true);
    ASSERT_NE(task, nullptr);
    std::array<uint8_t, kUserWorkspaceBytes> bytes{};
    executor_->UpdateTensorAddr(bytes.data(), bytes.size());
    executor_->workspaceDeviceAicpuTaskOffset_ = kWorkspaceOffset;
    tensor->SetWorkspaceOffset(kWorkspaceOffset);
    tensor->SetStorageAddr(bytes.data());
    EXPECT_NE(task->SetIoTensors(executor_.get(), args_.get()), ACLNN_SUCCESS);
}

TEST_F(AicpuWorkspaceTaskTest, LauncherPlansWorkspaceAndPreservesOtherBudgets)
{
    const auto opType = op::GenOpTypeId("WorkspaceLauncherTest");
    ASSERT_TRUE(RegisterAicpuWorkspaceTilingByName("WorkspaceLauncherTest", WorkspaceTiling));
    executor_->workspaceDeviceAicpuMem_ = kRuntimeCacheBytes;
    gTilingCalls = 0U;
    ASSERT_EQ(CreatAicpuKernelLauncher(opType, *space_, executor_.get(), attrNames_, args_.release()), ACLNN_SUCCESS);
    EXPECT_EQ(gTilingCalls, 1U);
    const auto total = executor_->GetWorkspaceSize();
    EXPECT_GE(total, kRawWorkspaceBytes + kRuntimeCacheBytes);
    EXPECT_EQ(executor_->workspaceDeviceAicpuMem_, kRuntimeCacheBytes);

    aclTensor baselineInput{op::Shape({kInputElements}), op::DataType::DT_DOUBLE, op::Format::FORMAT_ND, nullptr};
    aclTensor baselineOutput{op::Shape({kInputElements}), op::DataType::DT_DOUBLE, op::Format::FORMAT_ND, nullptr};
    aclOpExecutor baselineExecutor;
    baselineExecutor.workspaceDeviceAicpuMem_ = kRuntimeCacheBytes;
    const auto baselineType = op::GenOpTypeId("WorkspaceBudgetControl");
    auto* baselineArgs = op::GetOpArgContext(OP_INPUT(&baselineInput), OP_OUTPUT(&baselineOutput));
    ASSERT_EQ(CreatAicpuKernelLauncher(baselineType, *space_, &baselineExecutor, attrNames_, baselineArgs),
              ACLNN_SUCCESS);
    const auto baselineTotal = baselineExecutor.GetWorkspaceSize();
    EXPECT_GT(baselineTotal, kRuntimeCacheBytes);
    EXPECT_GE(total, baselineTotal + kRawWorkspaceBytes);
}

TEST_F(AicpuWorkspaceTaskTest, ExplicitWorkspaceSkipsTiling)
{
    const auto opType = op::GenOpTypeId("WorkspaceExplicitTest");
    ASSERT_TRUE(RegisterAicpuWorkspaceTilingByName("WorkspaceExplicitTest", WorkspaceTiling));
    ASSERT_NE(AddWorkspace(), nullptr);
    gTilingCalls = 0U;
    ASSERT_EQ(CreatAicpuKernelLauncher(opType, *space_, executor_.get(), attrNames_, args_.release()), ACLNN_SUCCESS);
    EXPECT_EQ(gTilingCalls, 0U);
}

TEST_F(AicpuWorkspaceTaskTest, ZeroTilingKeepsLegacyTask)
{
    const auto opType = op::GenOpTypeId("WorkspaceZeroTest");
    ASSERT_TRUE(RegisterAicpuWorkspaceTilingByName("WorkspaceZeroTest", ZeroTiling));
    ASSERT_EQ(CreatAicpuKernelLauncher(opType, *space_, executor_.get(), attrNames_, args_.release()), ACLNN_SUCCESS);
    ASSERT_EQ(GetCachedTaskNum(space_.get()), 1U);
    const auto& task = space_->hashMap_.begin()->second.front();
    EXPECT_FALSE(task->extInfoHandle_->HasWorkspaceInfo());
}

TEST_F(AicpuWorkspaceTaskTest, TilingFailureDoesNotCreateTask)
{
    const auto opType = op::GenOpTypeId("WorkspaceFailedTest");
    ASSERT_TRUE(RegisterAicpuWorkspaceTilingByName("WorkspaceFailedTest", FailedTiling));
    EXPECT_EQ(CreatAicpuKernelLauncher(opType, *space_, executor_.get(), attrNames_, args_.release()),
              ACLNN_ERR_PARAM_INVALID);
    EXPECT_EQ(GetCachedTaskNum(space_.get()), 0U);
}

TEST_F(AicpuWorkspaceTaskTest, OversizedTilingRejectedBeforeTensorPlanning)
{
    const auto opType = op::GenOpTypeId("WorkspaceOversizedTest");
    ASSERT_TRUE(RegisterAicpuWorkspaceTilingByName("WorkspaceOversizedTest", OversizedTiling));
    EXPECT_EQ(CreatAicpuKernelLauncher(opType, *space_, executor_.get(), attrNames_, args_.release()),
              ACLNN_ERR_PARAM_INVALID);
    EXPECT_EQ(GetCachedTaskNum(space_.get()), 0U);
}

TEST_F(AicpuWorkspaceTaskTest, UnregisteredOperatorKeepsLegacyTask)
{
    const auto opType = op::GenOpTypeId("WorkspaceUnregisteredTest");
    ASSERT_EQ(CreatAicpuKernelLauncher(opType, *space_, executor_.get(), attrNames_, args_.release()), ACLNN_SUCCESS);
    const auto& task = space_->hashMap_.begin()->second.front();
    EXPECT_FALSE(task->extInfoHandle_->HasWorkspaceInfo());
}

TEST_F(AicpuWorkspaceTaskTest, ShapeRangeCacheBudgetIncludesWorkspaceRecord)
{
    AicpuTaskSpace rangeSpace("WorkspaceTaskTestRange", ge::DEPEND_SHAPE_RANGE, false);
    auto* task = rangeSpace.GetOrCreateTaskImpl(executor_.get(), attrNames_, args_.get(), true);
    ASSERT_NE(task, nullptr);
    EXPECT_TRUE(task->extInfoHandle_->HasWorkspaceInfo());
    EXPECT_GT(task->deviceExtMemSize_, kRawWorkspaceBytes);
    EXPECT_GE(executor_->workspaceDeviceAicpuMem_, task->deviceExtMemSize_);
    task->SetVisit(false);
    aclnnAicpuFinalize();
}

TEST_F(AicpuWorkspaceTaskTest, FullCacheEvictsIdleTaskAndPreservesLayout)
{
    if (!HasCacheLimit("2")) {
        GTEST_SKIP() << "Run with ACLNN_CACHE_LIMIT=2 to exercise eviction.";
    }
    auto* legacy = Create(false);
    ASSERT_NE(legacy, nullptr);
    legacy->SetVisit(false);
    auto* busy = Create(true);
    ASSERT_NE(busy, nullptr);
    input_.SetDataType(op::DataType::DT_FLOAT);
    auto* replacement = Create(true);
    ASSERT_NE(replacement, nullptr);
    EXPECT_TRUE(replacement->extInfoHandle_->HasWorkspaceInfo());
    EXPECT_EQ(GetCachedTaskNum(space_.get()), kSmallCacheLimit);
    EXPECT_EQ(SpaceCacheDataMap()[space_.get()].taskInfo.count(legacy), 0U);
    EXPECT_EQ(SpaceCacheDataMap()[space_.get()].taskInfo.count(busy), 1U);
}

TEST_F(AicpuWorkspaceTaskTest, FullBusyCacheReturnsOwnedOverflowTask)
{
    if (!HasCacheLimit("2")) {
        GTEST_SKIP() << "Run with ACLNN_CACHE_LIMIT=2 to exercise a full busy cache.";
    }
    ASSERT_NE(Create(false), nullptr);
    ASSERT_NE(Create(true), nullptr);
    auto* overflow = Create(true);
    ASSERT_NE(overflow, nullptr);
    EXPECT_TRUE(overflow->extInfoHandle_->HasWorkspaceInfo());
    auto owner = TakePendingOverflowTask(overflow);
    EXPECT_EQ(owner.get(), overflow);
    EXPECT_EQ(GetCachedTaskNum(space_.get()), kSmallCacheLimit);
}

TEST_F(AicpuWorkspaceTaskTest, DisabledCacheReturnsOwnedWorkspaceTask)
{
    if (!HasCacheLimit("0")) {
        GTEST_SKIP() << "Run with ACLNN_CACHE_LIMIT=0 to exercise disabled caching.";
    }
    auto* task = Create(true);
    ASSERT_NE(task, nullptr);
    EXPECT_TRUE(task->extInfoHandle_->HasWorkspaceInfo());
    auto owner = TakePendingOverflowTask(task);
    EXPECT_EQ(owner.get(), task);
    EXPECT_EQ(GetCachedTaskNum(space_.get()), 0U);
}
} // namespace
