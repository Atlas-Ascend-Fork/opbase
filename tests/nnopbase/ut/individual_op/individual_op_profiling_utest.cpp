/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <gtest/gtest.h>

#include <vector>

#include "depends/acl/aclrt_stub.h"
#include "depends/op/aclnn_bninference_d_kernel_stub.h"
#include "executor/indv_executor.h"
#include "individual_op_api.h"
#include "op_dfx_internal.h"
#include "depends/op/op_stub.h"
#include "depends/profiler/profiler_stub.h"
#include "utils/file_faker.h"

namespace {
const uint32_t kBnInferenceOpTypeId = op::GenOpTypeId("bninference_d_kernel");
struct ProfilingRecorder : public ProfilerStub {
    int32_t MsprofReportApi(uint32_t agingFlag, const MsprofApi* api) override
    {
        ++apiCount;
        if (api->type == MSPROF_REPORT_NODE_LAUNCH_TYPE) {
            ++launchCount;
        }
        return 0;
    }

    int32_t MsprofReportCompactInfo(uint32_t agingFlag, const VOID_PTR data, uint32_t length) override
    {
        ++compactCount;
        return 0;
    }

    int32_t MsprofReportAdditionalInfo(uint32_t agingFlag, const VOID_PTR data, uint32_t length) override
    {
        ++additionalCount;
        return 0;
    }

    bool MsprofCheckOpSwitch(uint32_t type, const char* op, size_t len) override
    {
        ++checkCount;
        return checkResult;
    }

    void Reset()
    {
        apiCount = 0U;
        launchCount = 0U;
        compactCount = 0U;
        additionalCount = 0U;
        checkCount = 0U;
    }

    uint32_t apiCount = 0U;
    uint32_t launchCount = 0U;
    uint32_t compactCount = 0U;
    uint32_t additionalCount = 0U;
    uint32_t checkCount = 0U;
    bool checkResult = false;
};

struct LaunchAttrRecorder : public AclrtStub {
    aclError aclrtLaunchKernelWithHostArgs(aclrtFuncHandle funcHandle, uint32_t blockDim, aclrtStream stream,
                                           aclrtLaunchKernelCfg* cfg, void* hostArgs, size_t argsSize,
                                           aclrtPlaceHolderInfo* placeHolderArray, size_t placeHolderNum) override
    {
        ++launchCount;
        if (cfg != nullptr) {
            for (uint32_t i = 0U; i < cfg->numAttrs; ++i) {
                if (cfg->attrs[i].id == ACL_RT_LAUNCH_KERNEL_ATTR_ENABLE_PROFILING &&
                    cfg->attrs[i].value.enableProfiling == 1U) {
                    ++profilingAttrCount;
                }
            }
        }
        return ACL_SUCCESS;
    }

    uint32_t launchCount = 0U;
    uint32_t profilingAttrCount = 0U;
};

struct CacheOpInfoRecorder : public AclrtStub {
    aclError aclrtGetStreamAttribute(aclrtStream stream, aclrtStreamAttr stmAttrType,
                                     aclrtStreamAttrValue* value) override
    {
        value->cacheOpInfoSwitch = cacheOpInfoEnabled ? 1U : 0U;
        return ACL_SUCCESS;
    }

    aclError aclrtCacheLastTaskOpInfo(const void* const infoPtr, size_t infoSize) override
    {
        ++cacheOpInfoCount;
        return ACL_SUCCESS;
    }

    uint32_t cacheOpInfoCount = 0U;
    bool cacheOpInfoEnabled = false;
};

aclnnStatus RunBnOperator(aclrtStream stream = nullptr)
{
    std::vector<int64_t> shape = {1, 1, 1, 1, 1};
    aclTensor* tensor = aclCreateTensor(shape.data(), shape.size(), aclDataType::ACL_FLOAT, nullptr, 0,
                                        aclFormat::ACL_FORMAT_ND, shape.data(), shape.size(), nullptr);
    if (stream != nullptr) {
        uint64_t workspaceSize = 0U;
        aclOpExecutor* executor = nullptr;
        aclnnStatus directRet = aclnnBninferenceDKernelGetWorkspaceSize(tensor, tensor, tensor, tensor, &workspaceSize,
                                                                        &executor);
        if (directRet != OK) {
            aclDestroyTensor(tensor);
            return directRet;
        }
        void* workspace = workspaceSize != 0U ? malloc(workspaceSize) : nullptr;
        directRet = aclnnBninferenceDKernel(workspace, workspaceSize, executor, stream);
        if (workspace != nullptr) {
            free(workspace);
        }
        aclDestroyTensor(tensor);
        return directRet;
    }

    aclnnStatus ret = RunOp(aclnnBninferenceDKernel, tensor, tensor, tensor, tensor);
    aclDestroyTensor(tensor);
    return ret;
}

void EnableProfiling(bool reportFlag, bool kernelLaunchFlag, bool additionInfoFlag = true)
{
    op::internal::opProfilingSwitch.reportFlag = reportFlag;
    op::internal::opProfilingSwitch.kernelLaunchFlag = kernelLaunchFlag;
    op::internal::opProfilingSwitch.additionInfoFlag = additionInfoFlag;
}

int32_t StartProfiling(uint64_t profSwitch)
{
    MsprofCommandHandle handle{};
    handle.profSwitch = profSwitch;
    handle.type = PROF_COMMANDHANDLE_TYPE_START;
    return op::internal::ProfilingCallBack(PROF_CTRL_SWITCH, &handle, sizeof(handle));
}

int32_t StopProfiling()
{
    MsprofCommandHandle handle{};
    handle.type = PROF_COMMANDHANDLE_TYPE_STOP;
    return op::internal::ProfilingCallBack(PROF_CTRL_SWITCH, &handle, sizeof(handle));
}
} // namespace

class IndividualOpProfilingUt : public testing::Test {
protected:
    void SetUp() override
    {
        NnopbaseSetStubFiles(OP_API_COMMON_UT_SRC_DIR);
        originalProfiling_ = op::internal::opProfilingSwitch;
        op::internal::opProfilingSwitch = {};
    }

    void TearDown() override
    {
        if (specifiedOpProfilingStarted_) {
            StopProfiling();
        }
        op::internal::opProfilingSwitch = originalProfiling_;
        ProfilerStub::GetInstance()->UnInstall();
        AclrtStub::GetInstance()->UnInstall();
        NnopbaseUnsetEnvAndClearFolder();
    }

    op::internal::OpProfilingSwitch originalProfiling_;
    bool specifiedOpProfilingStarted_ = false;
    ProfilingRecorder* recorder_ = nullptr;

    void EnableSpecifiedOpProfiling(bool checkResult)
    {
        ASSERT_NE(recorder_, nullptr);
        recorder_->checkResult = checkResult;
        ASSERT_EQ(StartProfiling(PROF_TASK_TIME_MASK | PROF_TASK_TIME_L2_MASK | PROF_OP_MASK), 0);
        specifiedOpProfilingStarted_ = true;
    }
};

TEST_F(IndividualOpProfilingUt, DefaultProfilingWhenSpecifiedOpSwitchDisabled)
{
    ProfilingRecorder recorder;
    recorder.Reset();
    ProfilerStub::GetInstance()->Install(&recorder);
    EnableProfiling(true, false, true);

    aclrtStream stream = reinterpret_cast<aclrtStream>(new uint8_t[1]);
    aclnnStatus ret = RunBnOperator(stream);
    delete[] reinterpret_cast<uint8_t*>(stream);
    EXPECT_EQ(ret, OK);
    EXPECT_GT(recorder.apiCount, 0U);
    EXPECT_GT(recorder.launchCount, 0U);
    EXPECT_EQ(recorder.checkCount, 0U);
}

TEST_F(IndividualOpProfilingUt, HostProfilingFilteredWhenSpecifiedOpMisses)
{
    ProfilingRecorder recorder;
    recorder.Reset();
    ProfilerStub::GetInstance()->Install(&recorder);
    EnableProfiling(true, true, true);
    recorder_ = &recorder;

    EnableSpecifiedOpProfiling(false);

    aclrtStream stream = reinterpret_cast<aclrtStream>(new uint8_t[1]);
    aclnnStatus ret = RunBnOperator(stream);
    delete[] reinterpret_cast<uint8_t*>(stream);
    EXPECT_EQ(ret, OK);
    EXPECT_EQ(recorder.apiCount, 0U);
    EXPECT_EQ(recorder.launchCount, 0U);
    EXPECT_EQ(recorder.compactCount, 0U);
    EXPECT_EQ(recorder.additionalCount, 0U);
}

TEST_F(IndividualOpProfilingUt, LaunchProfilingKeptWhenOnlyReportFlagEnabled)
{
    ProfilingRecorder recorder;
    recorder.Reset();
    ProfilerStub::GetInstance()->Install(&recorder);
    EnableProfiling(true, false, true);

    aclrtStream stream = reinterpret_cast<aclrtStream>(new uint8_t[1]);
    aclnnStatus ret = RunBnOperator(stream);
    delete[] reinterpret_cast<uint8_t*>(stream);
    EXPECT_EQ(ret, OK);
    EXPECT_GT(recorder.launchCount, 0U);
}

TEST_F(IndividualOpProfilingUt, Level2ProfilingFollowsOperatorFilterWhenL0Enabled)
{
    ProfilingRecorder recorder;
    recorder.Reset();
    ProfilerStub::GetInstance()->Install(&recorder);
    EnableProfiling(false, true, false);
    op::internal::opProfilingSwitch.level2ProfilingFlag = true;
    recorder_ = &recorder;
    EnableSpecifiedOpProfiling(true);

    void* executorSpace = nullptr;
    ASSERT_EQ(NnopbaseCreateExecutorSpace(&executorSpace), OK);
    const char* opType = "bninference_d_kernel";
    char inputDesc[] = {1, 1, 1};
    char outputDesc[] = {1};
    char attrDesc[] = {1};
    void* executor = NnopbaseGetExecutor(executorSpace, opType, inputDesc, sizeof(inputDesc), outputDesc,
                                         sizeof(outputDesc), attrDesc, sizeof(attrDesc));
    ASSERT_NE(executor, nullptr);

    std::vector<int64_t> shape = {1, 1, 1, 1, 1};
    aclTensor* tensor = aclCreateTensor(shape.data(), shape.size(), aclDataType::ACL_FLOAT, nullptr, 0,
                                        aclFormat::ACL_FORMAT_ND, shape.data(), shape.size(), nullptr);
    ASSERT_NE(tensor, nullptr);
    ASSERT_EQ(NnopbaseAddInput(executor, tensor, 0), OK);
    ASSERT_EQ(NnopbaseAddInput(executor, tensor, 1), OK);
    ASSERT_EQ(NnopbaseAddInput(executor, tensor, 2), OK);
    ASSERT_EQ(NnopbaseAddOutput(executor, tensor, 0), OK);
    int64_t attrValue = 1;
    ASSERT_EQ(NnopbaseAddAttrWithDtype(executor, &attrValue, sizeof(attrValue), 0, kNnopbaseInt), OK);

    uint64_t workspaceLen = 0U;
    ASSERT_EQ(NnopbaseRunForWorkspace(executor, &workspaceLen), OK);
    NnopbaseExecutorReportProfiling(reinterpret_cast<NnopbaseExecutor*>(executor), 1U, MSPROF_GE_TASK_TYPE_AI_CORE, 1U,
                                    nullptr);
    EXPECT_GT(recorder.apiCount, 0U);
    EXPECT_GT(recorder.compactCount, 0U);

    NnopbaseExecutorGcSpace(executorSpace);
    aclDestroyTensor(tensor);
}

TEST_F(IndividualOpProfilingUt, LaunchProfilingAttrAddedWhenOperatorSelected)
{
    ProfilingRecorder profiler;
    profiler.Reset();
    LaunchAttrRecorder aclrt;
    ProfilerStub::GetInstance()->Install(&profiler);
    AclrtStub::GetInstance()->Install(&aclrt);
    EnableProfiling(true, true, true);
    recorder_ = &profiler;

    EnableSpecifiedOpProfiling(true);

    aclrtStream stream = reinterpret_cast<aclrtStream>(new uint8_t[1]);
    aclnnStatus ret = RunBnOperator(stream);
    delete[] reinterpret_cast<uint8_t*>(stream);
    EXPECT_EQ(ret, OK);
    EXPECT_GT(aclrt.launchCount, 0U);
    EXPECT_GT(aclrt.profilingAttrCount, 0U);
}

TEST_F(IndividualOpProfilingUt, AclGraphCacheOpInfoIndependentFromOpProfiling)
{
    ProfilingRecorder profiler;
    CacheOpInfoRecorder aclrt;
    aclrt.cacheOpInfoEnabled = true;
    ProfilerStub::GetInstance()->Install(&profiler);
    AclrtStub::GetInstance()->Install(&aclrt);
    EnableProfiling(true, true, true);
    recorder_ = &profiler;

    EnableSpecifiedOpProfiling(false);
    EXPECT_FALSE(op::internal::CheckSpecifiedOpProfiling(kBnInferenceOpTypeId));

    aclrtStream stream = reinterpret_cast<aclrtStream>(new uint8_t[1]);
    aclnnStatus ret = RunBnOperator(stream);
    delete[] reinterpret_cast<uint8_t*>(stream);
    EXPECT_EQ(ret, OK);
    EXPECT_GT(aclrt.cacheOpInfoCount, 0U);
}
