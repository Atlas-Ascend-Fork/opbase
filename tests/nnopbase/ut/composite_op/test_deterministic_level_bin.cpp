/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "op_kernel.h"

#include <memory>
#include <vector>

#include "base/registry/op_impl_space_registry_v2.h"
#include "gtest/gtest.h"
#include "kernel_mgr.h"
#include "opdev/data_type_utils.h"
#include "opdev/make_op_executor.h"
#include "thread_local_context.h"

using namespace op;
using namespace op::internal;

OP_TYPE_REGISTER(TestLevelBinOp);
OP_TYPE_REGISTER(TestLevelSplitOp);
OP_TYPE_REGISTER(TestLevelSplitMode2Op);
OP_TYPE_REGISTER(TestLevelInvalidOp);
OP_TYPE_REGISTER(TestLevelDefaultOp);
OP_TYPE_REGISTER(TestLevelErrLevelsEmptyOp);
OP_TYPE_REGISTER(TestLevelErrLevelsTypeOp);
OP_TYPE_REGISTER(TestLevelErrLevelsElementOp);
OP_TYPE_REGISTER(TestLevelErrTrueZeroOp);
OP_TYPE_REGISTER(TestLevelErrTrueMixedOp);
OP_TYPE_REGISTER(TestLevelErrFalseNonzeroOp);
OP_TYPE_REGISTER(TestLevelErrDetTypeOp);
OP_TYPE_REGISTER(TestLevelErrLegacyDetTypeOp);
OP_TYPE_REGISTER(TestLevelErrNoListOp);
OP_TYPE_REGISTER(TestLevelErrBinlistTypeOp);
OP_TYPE_REGISTER(TestLevelErrLevelOutOfRangeOp);
OP_TYPE_REGISTER(TestLevelMixedEntryOp);
OP_TYPE_REGISTER(TestLevelDebugConsistencyOp);

namespace {
// 临时打开 debug kernel 开关，析构时恢复，避免影响其他用例
class DebugKernelSwitch {
public:
    DebugKernelSwitch() { op::internal::systemConfig.SetEnableDebugKernelFlag(true); }
    ~DebugKernelSwitch() { op::internal::systemConfig.SetEnableDebugKernelFlag(false); }
};
} // namespace

class DeterministicLevelBinUT : public testing::Test {
protected:
    static void SetUpTestCase()
    {
        setenv("ASCEND_OPP_PATH", OP_API_COMMON_UT_SRC_DIR, 1);
        TestLevelBinOpOpTypeId();
        TestLevelSplitOpOpTypeId();
        TestLevelSplitMode2OpOpTypeId();
        TestLevelInvalidOpOpTypeId();
        TestLevelDefaultOpOpTypeId();
        TestLevelErrLevelsEmptyOpOpTypeId();
        TestLevelErrLevelsTypeOpOpTypeId();
        TestLevelErrLevelsElementOpOpTypeId();
        TestLevelErrTrueZeroOpOpTypeId();
        TestLevelErrTrueMixedOpOpTypeId();
        TestLevelErrFalseNonzeroOpOpTypeId();
        TestLevelErrDetTypeOpOpTypeId();
        TestLevelErrLegacyDetTypeOpOpTypeId();
        TestLevelErrNoListOpOpTypeId();
        TestLevelErrBinlistTypeOpOpTypeId();
        TestLevelErrLevelOutOfRangeOpOpTypeId();
        TestLevelMixedEntryOpOpTypeId();
        TestLevelDebugConsistencyOpOpTypeId();

        // 建立独立的 OpImpl 注册空间，避免依赖其他用例先初始化 SpaceRegistry
        previousSpaceRegistry_ = gert::DefaultOpImplSpaceRegistryV2::GetInstance().GetSpaceRegistry();
        gert::DefaultOpImplSpaceRegistryV2::GetInstance().SetSpaceRegistry(
            std::make_shared<gert::OpImplSpaceRegistryV2>());
    }

    static void TearDownTestCase()
    {
        auto& instance = gert::DefaultOpImplSpaceRegistryV2::GetInstance();
        if (previousSpaceRegistry_ == nullptr) {
            instance.ClearSpaceRegistry();
        } else {
            instance.SetSpaceRegistry(previousSpaceRegistry_);
        }
    }

    void TearDown() override
    {
        // 复位等级，避免影响其它用例
        GetThreadLocalContext().opConfigInfo_.deterministicLevel_ = 0;
        GetThreadLocalContext().opConfigInfo_.isDeterministicOn_ = false;
    }

    OpKernelBin* SelectBinByLevel(uint32_t opType, uint8_t level, aclDataType dtype = aclDataType::ACL_FLOAT16)
    {
        GetThreadLocalContext().opConfigInfo_.deterministicLevel_ = level;
        GetThreadLocalContext().opConfigInfo_.isDeterministicOn_ = (level >= 1);

        auto kernel = gKernelMgr.GetKernel(opType);
        if (kernel == nullptr) {
            ADD_FAILURE() << "kernel is nullptr";
            return nullptr;
        }
        EXPECT_EQ(gKernelMgr.AclOpKernelInit(opType), ACLNN_SUCCESS);

        const auto opDtype = op::ToOpDataType(dtype);
        auto x1 = std::make_unique<aclTensor>(op::Shape{2, 2}, opDtype, op::Format::FORMAT_ND, nullptr);
        auto x2 = std::make_unique<aclTensor>(op::Shape{2, 2}, opDtype, op::Format::FORMAT_ND, nullptr);
        auto y = std::make_unique<aclTensor>(op::Shape{2, 2}, opDtype, op::Format::FORMAT_ND, nullptr);
        auto ctx = op::MakeOpArgContext(OP_INPUT(x1.get(), x2.get()), OP_OUTPUT(y.get()));
        if (ctx == nullptr) {
            ADD_FAILURE() << "MakeOpArgContext returned nullptr";
            return nullptr;
        }
        auto bin = kernel->SelectBin(*ctx->GetOpArg(op::OP_INPUT_ARG), *ctx->GetOpArg(op::OP_OUTPUT_ARG),
                                     *ctx->GetOpArg(op::OP_ATTR_ARG));
        op::DestroyOpArgContext(ctx);
        return bin;
    }

    static std::shared_ptr<gert::OpImplSpaceRegistryV2> previousSpaceRegistry_;
};

std::shared_ptr<gert::OpImplSpaceRegistryV2> DeterministicLevelBinUT::previousSpaceRegistry_;

TEST_F(DeterministicLevelBinUT, UsesLevelBinListWhenPresent)
{
    // binListWithLoadPriority 存在时，只解析它：选中的应是 l0 产物，而不是 binList 的 old 产物
    auto bin = SelectBinByLevel(op::OpTypeDict::ToOpType("TestLevelBinOp"), 0);
    ASSERT_NE(bin, nullptr);
    EXPECT_NE(bin->binPath_.find("TestLevelBinOp_l0.o"), std::string::npos);
    EXPECT_EQ(bin->binPath_.find("TestLevelBinOp_old.o"), std::string::npos);
}

TEST_F(DeterministicLevelBinUT, SelectsBinByDeterministicLevel)
{
    // 多 dtype 组各自声明 [0]/[1,2]/[3]，注册期按 deterministicLevels 逐等级展开 key，
    // 运行期按 dtype + level 命中各自产物
    const uint32_t opType = op::OpTypeDict::ToOpType("TestLevelSplitOp");
    const std::vector<std::pair<aclDataType, std::vector<std::pair<uint8_t, std::string>>>> groups = {
        {aclDataType::ACL_FLOAT16,
         {{0, "TestLevelSplitOp_fp16_l0.o"},
          {1, "TestLevelSplitOp_fp16_l12.o"},
          {2, "TestLevelSplitOp_fp16_l12.o"},
          {3, "TestLevelSplitOp_fp16_l3.o"}}},
        {aclDataType::ACL_FLOAT,
         {{0, "TestLevelSplitOp_fp32_l0.o"},
          {1, "TestLevelSplitOp_fp32_l12.o"},
          {2, "TestLevelSplitOp_fp32_l12.o"},
          {3, "TestLevelSplitOp_fp32_l3.o"}}},
    };
    for (const auto& [dtype, cases] : groups) {
        for (const auto& [level, expectBin] : cases) {
            auto bin = SelectBinByLevel(opType, level, dtype);
            ASSERT_NE(bin, nullptr) << "dtype=" << static_cast<int>(dtype) << ", level=" << static_cast<int>(level);
            EXPECT_NE(bin->binPath_.find(expectBin), std::string::npos)
                << "dtype=" << static_cast<int>(dtype) << ", level=" << static_cast<int>(level);
        }
    }
}

TEST_F(DeterministicLevelBinUT, CustomizedKeyContainsDeterministicLevel)
{
    // mode 2 自定义 key 字面量带 dt=<level>，运行期需拼出同样的 key 才能按等级命中产物
    const uint32_t opType = op::OpTypeDict::ToOpType("TestLevelSplitMode2Op");
    const std::vector<std::pair<uint8_t, std::string>> cases = {
        {0, "TestLevelSplitMode2Op_l0.o"},
        {1, "TestLevelSplitMode2Op_l12.o"},
        {2, "TestLevelSplitMode2Op_l12.o"},
        {3, "TestLevelSplitMode2Op_l3.o"},
    };
    for (const auto& [level, expectBin] : cases) {
        auto bin = SelectBinByLevel(opType, level);
        ASSERT_NE(bin, nullptr) << "level=" << static_cast<int>(level);
        EXPECT_NE(bin->binPath_.find(expectBin), std::string::npos) << "level=" << static_cast<int>(level);
    }
}

TEST_F(DeterministicLevelBinUT, AppliesDeterministicLevelDefaults)
{
    // deterministic / deterministicLevels 未配置时按固定规则补齐
    const uint32_t opType = op::OpTypeDict::ToOpType("TestLevelDefaultOp");

    // deterministic=true 未配置等级：只注册 level 1，level 0 应选不到
    auto bin = SelectBinByLevel(opType, 1, aclDataType::ACL_FLOAT16);
    ASSERT_NE(bin, nullptr);
    EXPECT_NE(bin->binPath_.find("TestLevelDefaultOp_fp16_l1.o"), std::string::npos);
    EXPECT_EQ(SelectBinByLevel(opType, 0, aclDataType::ACL_FLOAT16), nullptr);

    // deterministic=false 未配置等级：只注册 level 0
    bin = SelectBinByLevel(opType, 0, aclDataType::ACL_FLOAT);
    ASSERT_NE(bin, nullptr);
    EXPECT_NE(bin->binPath_.find("TestLevelDefaultOp_fp32_l0.o"), std::string::npos);

    // deterministic 未配置且等级未配置：非确定性 level 0
    bin = SelectBinByLevel(opType, 0, aclDataType::ACL_BF16);
    ASSERT_NE(bin, nullptr);
    EXPECT_NE(bin->binPath_.find("TestLevelDefaultOp_bf16_l0.o"), std::string::npos);

    // ignore 未配置等级：按 legacy 语义同时注册 level 0 与 level 1
    bin = SelectBinByLevel(opType, 0, aclDataType::ACL_INT32);
    ASSERT_NE(bin, nullptr);
    EXPECT_NE(bin->binPath_.find("TestLevelDefaultOp_ignore.o"), std::string::npos);
    bin = SelectBinByLevel(opType, 1, aclDataType::ACL_INT32);
    ASSERT_NE(bin, nullptr);
    EXPECT_NE(bin->binPath_.find("TestLevelDefaultOp_ignore.o"), std::string::npos);

    // 显式等级优先于补齐规则
    bin = SelectBinByLevel(opType, 3, aclDataType::ACL_INT64);
    ASSERT_NE(bin, nullptr);
    EXPECT_NE(bin->binPath_.find("TestLevelDefaultOp_int64_l3.o"), std::string::npos);

    // 未知字符串按 ignore 处理：注册 level 0 与 level 1
    bin = SelectBinByLevel(opType, 0, aclDataType::ACL_UINT8);
    ASSERT_NE(bin, nullptr);
    EXPECT_NE(bin->binPath_.find("TestLevelDefaultOp_unknown.o"), std::string::npos);
    bin = SelectBinByLevel(opType, 1, aclDataType::ACL_UINT8);
    ASSERT_NE(bin, nullptr);
    EXPECT_NE(bin->binPath_.find("TestLevelDefaultOp_unknown.o"), std::string::npos);

    // 空字符串按 ignore 处理：注册 level 0 与 level 1
    bin = SelectBinByLevel(opType, 0, aclDataType::ACL_BOOL);
    ASSERT_NE(bin, nullptr);
    EXPECT_NE(bin->binPath_.find("TestLevelDefaultOp_empty.o"), std::string::npos);
    bin = SelectBinByLevel(opType, 1, aclDataType::ACL_BOOL);
    ASSERT_NE(bin, nullptr);
    EXPECT_NE(bin->binPath_.find("TestLevelDefaultOp_empty.o"), std::string::npos);
}

TEST_F(DeterministicLevelBinUT, RejectsInvalidBinListConfig)
{
    // 结构性错误发生在条目循环之前，加载期直接失败
    const std::vector<std::string> opNames = {
        "TestLevelInvalidOp",        // binListWithLoadPriority 非数组
        "TestLevelErrNoListOp",      // binListWithLoadPriority 与 binList 都不存在
        "TestLevelErrBinlistTypeOp", // binList 非数组
    };
    for (const auto& opName : opNames) {
        const uint32_t opType = op::OpTypeDict::ToOpType(opName);
        ASSERT_NE(opType, 0u) << opName;
        EXPECT_NE(gKernelMgr.AclOpKernelInit(opType), ACLNN_SUCCESS) << opName;
    }
}

TEST_F(DeterministicLevelBinUT, SkipsInvalidDeterministicEntry)
{
    // 条目级配置错误只打 ERROR 并跳过该条目，算子初始化仍成功；无有效条目时 SelectBin 返回空
    const std::vector<std::string> opNames = {
        "TestLevelErrLevelsEmptyOp",     // deterministicLevels: []
        "TestLevelErrLevelsTypeOp",      // deterministicLevels: 3（非数组）
        "TestLevelErrLevelsElementOp",   // deterministicLevels: [1, "2"]
        "TestLevelErrTrueZeroOp",        // true + [0]
        "TestLevelErrTrueMixedOp",       // true + [0, 1]
        "TestLevelErrFalseNonzeroOp",    // false + [1]
        "TestLevelErrDetTypeOp",         // deterministic: true（布尔）
        "TestLevelErrLegacyDetTypeOp",   // binList 路径 deterministic: 1（数字）
        "TestLevelErrLevelOutOfRangeOp", // deterministicLevels: [10]，超出单字符范围
    };
    for (const auto& opName : opNames) {
        const uint32_t opType = op::OpTypeDict::ToOpType(opName);
        ASSERT_NE(opType, 0u) << opName;
        EXPECT_EQ(gKernelMgr.AclOpKernelInit(opType), ACLNN_SUCCESS) << opName;
        EXPECT_EQ(SelectBinByLevel(opType, 0, aclDataType::ACL_FLOAT16), nullptr) << opName;
    }
}

TEST_F(DeterministicLevelBinUT, SkipsInvalidEntryButKeepsValidEntries)
{
    // 同一配置中非法条目只跳过自身，合法条目仍正常注册并命中
    const uint32_t opType = op::OpTypeDict::ToOpType("TestLevelMixedEntryOp");
    ASSERT_NE(opType, 0u);
    EXPECT_EQ(gKernelMgr.AclOpKernelInit(opType), ACLNN_SUCCESS);

    auto bin = SelectBinByLevel(opType, 0, aclDataType::ACL_FLOAT);
    ASSERT_NE(bin, nullptr);
    EXPECT_NE(bin->binPath_.find("TestLevelMixedEntryOp_valid.o"), std::string::npos);

    // 非法条目的 dtype（float16）不应被注册
    EXPECT_EQ(SelectBinByLevel(opType, 0, aclDataType::ACL_FLOAT16), nullptr);
}

TEST_F(DeterministicLevelBinUT, SkipsDebugConfigWithDifferentBinListNode)
{
    // 正式配置使用 binListWithLoadPriority、debug 配置只有 binList：debug 配置应被跳过，正式 bin 仍可命中
    DebugKernelSwitch debugKernelSwitch;
    const uint32_t opType = op::OpTypeDict::ToOpType("TestLevelDebugConsistencyOp");
    ASSERT_NE(opType, 0u);
    EXPECT_EQ(gKernelMgr.AclOpKernelInit(opType), ACLNN_SUCCESS);

    auto bin = SelectBinByLevel(opType, 0, aclDataType::ACL_FLOAT16);
    ASSERT_NE(bin, nullptr);
    EXPECT_NE(bin->binPath_.find("TestLevelDebugConsistencyOp_formal.o"), std::string::npos);
}
