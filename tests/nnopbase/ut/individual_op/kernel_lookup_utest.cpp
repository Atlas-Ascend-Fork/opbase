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
#include <cstring>
#include <vector>
#include "executor/indv_bininfo.h"
#include "executor/indv_collector.h"
#include "executor/indv_executor.h"
#include "utils/file_faker.h"
#include "utils/indv_types.h"

#define private public
#include "executor/indv_args_pool.h"
#undef private

namespace {
constexpr size_t GEN_SIMPLIFIED_KEY_BUF_LEN = NNOPBASE_CUS_KEY_LEN;
constexpr size_t BASE_VERBKEY_PREFIX_LEN = 2U;
constexpr size_t DETERMINISTIC_VERBKEY_PREFIX_LEN = 3U;
constexpr size_t DYNKEY_BYTES_PER_TENSOR = 2U;
constexpr size_t CUSTOMIZED_STUB_INPUT_NUM = 3U;
constexpr size_t CUSTOMIZED_STUB_OUTPUT_NUM = 1U;
constexpr size_t DYNAMIC_TENSOR_DESC_NUM = 600U;

class NnopbaseKernelLookupUnitTest : public testing::Test {
protected:
    void SetUp() override
    {
        NnopbaseSetStubFiles(OP_API_COMMON_UT_SRC_DIR);
        if (binCollector_ == nullptr) {
            binCollector_ = new NnopbaseBinCollector;
            ASSERT_NE(binCollector_, nullptr);
            ASSERT_EQ(NnopbaseCollectorInit(binCollector_), OK);
            ASSERT_EQ(NnopbaseCollectorWork(binCollector_), OK);
        }
    }

    void TearDown() override { NnopbaseUnsetEnvAndClearFolder(); }

    NnopbaseExecutor* CreateExecutor(const char* opType)
    {
        auto* executor = new NnopbaseExecutor;
        EXPECT_NE(executor, nullptr);
        char inputDesc[] = {1, 1, 1};
        char outputDesc[] = {1};
        char attrDesc[] = {};
        EXPECT_EQ(NnopbaseExecutorInit(executor, {inputDesc, sizeof(inputDesc), outputDesc, sizeof(outputDesc),
                                                  attrDesc, sizeof(attrDesc)}),
                  OK);
        executor->space = new NnopbaseExecutorSpace;
        NnopbaseExecutorSetCollector(executor, binCollector_);
        EXPECT_EQ(NnopbaseExecutorSetRegInfo(executor, opType), OK);
        executor->ownArgs.enableCache = false;
        nnopbase::ArgsPool::GetInstance().CreateArgs(executor);
        std::vector<int64_t> shape = {1, 1, 1, 1, 1};
        aclTensor* tensor = aclCreateTensor(shape.data(), shape.size(), aclDataType::ACL_FLOAT, nullptr, 0,
                                            aclFormat::ACL_FORMAT_ND, shape.data(), shape.size(), nullptr);
        EXPECT_NE(tensor, nullptr);
        for (size_t i = 0; i < CUSTOMIZED_STUB_INPUT_NUM; i++) {
            EXPECT_EQ(NnopbaseExecutorUpdateTensorsIndex(&executor->ownArgs.inputs, i), OK);
            EXPECT_EQ(NnopbaseExecutorAddTensor(executor, tensor, i, true, false), OK);
        }
        EXPECT_EQ(NnopbaseExecutorUpdateTensorsIndex(&executor->ownArgs.outputs, 0), OK);
        EXPECT_EQ(NnopbaseExecutorAddTensor(executor, tensor, 0, false, false), OK);
        aclDestroyTensor(tensor);
        return executor;
    }

    void DestroyExecutor(NnopbaseExecutor* executor)
    {
        if (executor == nullptr) {
            return;
        }
        auto* args = (executor->args == &executor->ownArgs) ? nullptr : executor->args;
        NnopbaseExecutorGcSpace(executor->space);
        NnopbaseExecutorDeInit(executor);
        delete args;
        delete executor;
    }

private:
    static NnopbaseBinCollector* binCollector_;
};

NnopbaseBinCollector* NnopbaseKernelLookupUnitTest::binCollector_ = nullptr;

TEST_F(NnopbaseKernelLookupUnitTest, DynamicKeyUsesDeterministicLevel)
{
    auto* executor = CreateExecutor("bninference_d_kernel");
    executor->deterministicLevel = 3U;
    ASSERT_FALSE(NnopbaseExecutorGetDynamicBinInfo(executor));
    ASSERT_EQ(executor->args->binInfo, nullptr);
    const size_t opTypeLen = strlen(executor->opType);
    const size_t tensorCount = executor->ownArgs.inputs.paramDescs.count + executor->ownArgs.outputs.paramDescs.count;
    ASSERT_EQ(executor->binInfoKey.len, opTypeLen + BASE_VERBKEY_PREFIX_LEN + tensorCount * DYNKEY_BYTES_PER_TENSOR);
    DestroyExecutor(executor);
}

TEST_F(NnopbaseKernelLookupUnitTest, DynamicKeyFallsBackToBaseLayout)
{
    auto* executor = CreateExecutor("bninference_d_kernel");
    executor->deterministicLevel = 0U;
    ASSERT_TRUE(NnopbaseExecutorGetDynamicBinInfo(executor));
    ASSERT_NE(executor->args->binInfo, nullptr);
    const size_t opTypeLen = strlen(executor->opType);
    const size_t tensorCount = executor->ownArgs.inputs.paramDescs.count + executor->ownArgs.outputs.paramDescs.count;
    ASSERT_EQ(executor->binInfoKey.len, opTypeLen + BASE_VERBKEY_PREFIX_LEN + tensorCount * DYNKEY_BYTES_PER_TENSOR);
    DestroyExecutor(executor);
}

TEST_F(NnopbaseKernelLookupUnitTest, DynamicKeyPreallocatesTensorDescriptorBuffer)
{
    std::vector<char> inputDesc(DYNAMIC_TENSOR_DESC_NUM, 1U);
    char outputDesc[] = {1};
    char attrDesc[] = {};
    NnopbaseOpInfo opInfo{inputDesc.data(), static_cast<uint32_t>(inputDesc.size()),
                          outputDesc,       sizeof(outputDesc),
                          attrDesc,         sizeof(attrDesc)};

    auto* executor = new NnopbaseExecutor;
    EXPECT_EQ(NnopbaseExecutorInit(executor, opInfo), OK);
    executor->space = new NnopbaseExecutorSpace;
    NnopbaseExecutorSetCollector(executor, binCollector_);
    EXPECT_EQ(NnopbaseExecutorSetRegInfo(executor, "bninference_d_kernel"), OK);
    executor->ownArgs.enableCache = false;
    nnopbase::ArgsPool::GetInstance().CreateArgs(executor);

    std::vector<int64_t> shape = {1, 1, 1, 1, 1};
    aclTensor* tensor = aclCreateTensor(shape.data(), shape.size(), aclDataType::ACL_FLOAT, nullptr, 0,
                                        aclFormat::ACL_FORMAT_ND, shape.data(), shape.size(), nullptr);
    EXPECT_NE(tensor, nullptr);
    for (uint32_t i = 0U; i < DYNAMIC_TENSOR_DESC_NUM; i++) {
        EXPECT_EQ(NnopbaseExecutorUpdateTensorsIndex(&executor->ownArgs.inputs, i), OK);
        EXPECT_EQ(NnopbaseExecutorAddTensor(executor, tensor, i, true, false), OK);
    }
    EXPECT_EQ(NnopbaseExecutorUpdateTensorsIndex(&executor->ownArgs.outputs, 0), OK);
    EXPECT_EQ(NnopbaseExecutorAddTensor(executor, tensor, 0, false, false), OK);
    aclDestroyTensor(tensor);

    executor->deterministicLevel = 3U;
    const size_t tailLen = (DYNAMIC_TENSOR_DESC_NUM + 1U) * DYNKEY_BYTES_PER_TENSOR;
    ASSERT_FALSE(NnopbaseExecutorGetDynamicBinInfo(executor));
    ASSERT_EQ(executor->args->binInfo, nullptr);
    ASSERT_EQ(executor->binInfoKey.len, strlen(executor->opType) + BASE_VERBKEY_PREFIX_LEN + tailLen);

    DestroyExecutor(executor);
}

TEST_F(NnopbaseKernelLookupUnitTest, DynamicLookupMissClearsPreviousBinInfo)
{
    auto* executor = CreateExecutor("bninference_d_kernel");
    executor->deterministicLevel = 0U;
    ASSERT_TRUE(NnopbaseExecutorGetDynamicBinInfo(executor));
    ASSERT_NE(executor->args->binInfo, nullptr);

    executor->deterministicLevel = 3U;
    ASSERT_FALSE(NnopbaseExecutorGetDynamicBinInfo(executor));
    ASSERT_EQ(executor->args->binInfo, nullptr);
    DestroyExecutor(executor);
}

TEST_F(NnopbaseKernelLookupUnitTest, DynamicDeterministicKeyHitsFirstLayout)
{
    auto* executor = CreateExecutor("DtFirstHitLookupOp");
    executor->deterministicLevel = 3U;
    ASSERT_TRUE(NnopbaseExecutorGetDynamicBinInfo(executor));
    ASSERT_NE(executor->args->binInfo, nullptr);
    const size_t opTypeLen = strlen(executor->opType);
    const size_t tensorCount = executor->ownArgs.inputs.paramDescs.count + executor->ownArgs.outputs.paramDescs.count;
    ASSERT_EQ(executor->binInfoKey.len,
              opTypeLen + DETERMINISTIC_VERBKEY_PREFIX_LEN + tensorCount * DYNKEY_BYTES_PER_TENSOR);
    DestroyExecutor(executor);
}

TEST_F(NnopbaseKernelLookupUnitTest, DynamicBaseKeyFallsBackAfterDeterministicMiss)
{
    auto* executor = CreateExecutor("DtFallbackLookupOp");
    executor->deterministicLevel = 3U;
    ASSERT_TRUE(NnopbaseExecutorGetDynamicBinInfo(executor));
    ASSERT_NE(executor->args->binInfo, nullptr);
    const size_t opTypeLen = strlen(executor->opType);
    const size_t tensorCount = executor->ownArgs.inputs.paramDescs.count + executor->ownArgs.outputs.paramDescs.count;
    ASSERT_EQ(executor->binInfoKey.len, opTypeLen + BASE_VERBKEY_PREFIX_LEN + tensorCount * DYNKEY_BYTES_PER_TENSOR);
    DestroyExecutor(executor);
}

TEST_F(NnopbaseKernelLookupUnitTest, DynamicLayoutSwitchKeepsBufferAndClearsStaleTail)
{
    auto* executor = CreateExecutor("DtFallbackLookupOp");
    executor->deterministicLevel = 3U;
    ASSERT_TRUE(NnopbaseExecutorGetDynamicBinInfo(executor));

    const size_t opTypeLen = strlen(executor->opType);
    const size_t tensorCount = executor->ownArgs.inputs.paramDescs.count + executor->ownArgs.outputs.paramDescs.count;
    const size_t baseLen = opTypeLen + BASE_VERBKEY_PREFIX_LEN + tensorCount * DYNKEY_BYTES_PER_TENSOR;
    const size_t capacity = executor->binInfoKey.verbose.size();

    ASSERT_EQ(executor->binInfoKey.len, baseLen);
    ASSERT_EQ(executor->binInfoKey.bufLen, capacity);
    // The extra byte used only by deterministic layout is cleared after fallback.
    ASSERT_EQ(executor->binInfoKey.verbose[baseLen], 0U);
    DestroyExecutor(executor);
}

TEST_F(NnopbaseKernelLookupUnitTest, CustomizedKeyFallsBackToBaseLayout)
{
    auto* executor = CreateExecutor("StubCustomOp");
    executor->deterministicLevel = 0U;
    ASSERT_TRUE(NnopbaseExecutorGetDynamicBinInfo(executor));
    ASSERT_NE(executor->args->binInfo, nullptr);
    const size_t opTypeLen = strlen(executor->opType);
    ASSERT_EQ(executor->binInfoKey.len, opTypeLen + BASE_VERBKEY_PREFIX_LEN + strlen("diy,99"));
    DestroyExecutor(executor);
}

TEST_F(NnopbaseKernelLookupUnitTest, CustomizedFallbackToDynamicKey)
{
    auto* executor = CreateExecutor("StubMixKeyOp");
    ASSERT_TRUE(NnopbaseExecutorGetDynamicBinInfo(executor));
    ASSERT_NE(executor->args->binInfo, nullptr);
    const size_t opTypeLen = strlen(executor->opType);
    const size_t tensorCount = CUSTOMIZED_STUB_INPUT_NUM + CUSTOMIZED_STUB_OUTPUT_NUM;
    ASSERT_EQ(executor->binInfoKey.len, opTypeLen + BASE_VERBKEY_PREFIX_LEN + tensorCount * DYNKEY_BYTES_PER_TENSOR);
    DestroyExecutor(executor);
}

TEST_F(NnopbaseKernelLookupUnitTest, CustomizedKeyGeneratesSimplifiedKeyOnce)
{
    auto* executor = CreateExecutor("StubMixKeyOp");
    static uint32_t genCallCount = 0U;
    auto* regInfo = executor->regInfo;
    GenSimplifiedKeyFun savedGen = regInfo->genSimplifiedKey;
    regInfo->genSimplifiedKey = [](gert::TilingContext*, ge::char_t* key) -> ge::graphStatus {
        ++genCallCount;
        return strcpy_s(key, GEN_SIMPLIFIED_KEY_BUF_LEN, "diy,99");
    };
    ASSERT_TRUE(NnopbaseExecutorGetDynamicBinInfo(executor));
    ASSERT_EQ(genCallCount, 1U);
    regInfo->genSimplifiedKey = savedGen;
    DestroyExecutor(executor);
}
} // namespace
