/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include <cstring>
#include "acl/acl_op.h"
#include "register/op_impl_kernel_registry.h"
#include "graph/types.h"
#include "graph/operator_factory.h"
#include "graph/utils/type_utils.h"
#include "register/op_impl_registry.h"
#include "opdev/common_types.h"
#include "opdev/op_def.h"
#include "opdev/op_dfx.h"
#include "op_run_context.h"
#include "op_info_serialize.h"
#include "ini_parse.h"
#include "base/registry/op_impl_space_registry_v2.h"
#include "lib_path.h"

class OpInfoRecordUtest : public testing::Test {
protected:
    virtual void SetUp()
    {
        aclnnOpInfoRecord::Path confDir = aclnnOpInfoRecord::LibPath::Instance().GetInstallParentPath().Append("conf");
        ASSERT_TRUE(confDir.CreateDirectory(true));
        std::ifstream src(NNOPBASE_UT_DEPENDS_DIR "/conf/dump_tool_config.ini", std::ios::binary);
        ASSERT_TRUE(src.is_open());
        aclnnOpInfoRecord::Path confFile = confDir.Concat("dump_tool_config.ini");
        std::ofstream dst(confFile.GetString(), std::ios::binary);
        ASSERT_TRUE(dst.is_open());
        dst << src.rdbuf();
    }

    virtual void TearDown()
    {
        gert::DefaultOpImplSpaceRegistryV2::GetInstance().ClearSpaceRegistry();
        aclnnOpInfoRecord::Path confFile = aclnnOpInfoRecord::LibPath::Instance().GetInstallParentPath().Concat(
            "conf/dump_tool_config.ini");
        (void)std::remove(confFile.GetCString());
    }
};

TEST_F(OpInfoRecordUtest, Utest_OpInfoEmptyBinInfo)
{
    aclnnOpInfoRecord::OpCompilerOption opt("", 0);
    aclnnOpInfoRecord::OpKernelInfo kernelInfo("", 0);
    EXPECT_EQ(kernelInfo.isMc2, false);
    gert::TilingContext ctx;
    EXPECT_EQ(aclnnOpInfoRecord::OpInfoSerialize(&ctx, opt, &kernelInfo), -1);
    EXPECT_EQ(aclnnOpInfoRecord::OpInfoDump(), 0);
}

TEST_F(OpInfoRecordUtest, Utest_OpInfoSerialize)
{
    aclnnOpInfoRecord::OpCompilerOption opt("", 0);
    aclnnOpInfoRecord::OpKernelInfo kernelInfo(OP_API_COMMON_UT_SRC_DIR
                                               "/built-in/op_impl/ai_core/tbe/kernel/ascend910/axpy/"
                                               "Axpy_233851a3505389e43928a8bba133a74d_high_performance.json",
                                               0);
    op::Shape shape{33, 15, 1, 48};
    auto self = std::make_unique<aclTensor>(shape, op::DataType::DT_FLOAT, op::Format::FORMAT_ND, nullptr);
    auto other = std::make_unique<aclTensor>(shape, op::DataType::DT_FLOAT, op::Format::FORMAT_ND, nullptr);
    auto out = std::make_unique<aclTensor>(shape, op::DataType::DT_FLOAT, op::Format::FORMAT_ND, nullptr);
    float alpha = 13.37;
    auto input = OP_INPUT(self.get(), other.get());
    auto output = OP_OUTPUT(out.get());
    auto attr = OP_ATTR(alpha);
    auto ctx = op::MakeOpArgContext(input, output, attr);
    auto spaceRegistry = std::make_shared<gert::OpImplSpaceRegistryV2>();
    gert::DefaultOpImplSpaceRegistryV2::GetInstance().SetSpaceRegistry(spaceRegistry);
    uint32_t opType = op::GenOpTypeId("Axpy");
    gert::TilingContext* tilingCtx = op::internal::OpRunContextMgr::opRunCtx_.UpdateTilingCtx(
        opType, *ctx->GetOpArg(op::OpArgDef::OP_INPUT_ARG), *ctx->GetOpArg(op::OpArgDef::OP_OUTPUT_ARG),
        *ctx->GetOpArg(op::OpArgDef::OP_ATTR_ARG));
    ASSERT_NE(tilingCtx, nullptr);

    EXPECT_EQ(aclnnOpInfoRecord::OpInfoSerialize(tilingCtx, opt, &kernelInfo), 0);
    EXPECT_EQ(aclnnOpInfoRecord::OpInfoDump(), 0);
    op::DestroyOpArgContext(ctx);
}

// 场景：四参调用写入tiling与runtime键
TEST_F(OpInfoRecordUtest, Utest_OpInfoSerializeWithRuntimeInfo)
{
    aclnnOpInfoRecord::OpCompilerOption opt("", 0);
    aclnnOpInfoRecord::OpKernelInfo kernelInfo(OP_API_COMMON_UT_SRC_DIR
                                               "/built-in/op_impl/ai_core/tbe/kernel/ascend910/axpy/"
                                               "Axpy_233851a3505389e43928a8bba133a74d_high_performance.json",
                                               0);
    op::Shape shape{33, 15, 1, 48};
    auto self = std::make_unique<aclTensor>(shape, op::DataType::DT_FLOAT, op::Format::FORMAT_ND, nullptr);
    auto other = std::make_unique<aclTensor>(shape, op::DataType::DT_FLOAT, op::Format::FORMAT_ND, nullptr);
    auto out = std::make_unique<aclTensor>(shape, op::DataType::DT_FLOAT, op::Format::FORMAT_ND, nullptr);
    float alpha = 13.37;
    auto input = OP_INPUT(self.get(), other.get());
    auto output = OP_OUTPUT(out.get());
    auto attr = OP_ATTR(alpha);
    auto ctx = op::MakeOpArgContext(input, output, attr);
    auto spaceRegistry = std::make_shared<gert::OpImplSpaceRegistryV2>();
    gert::DefaultOpImplSpaceRegistryV2::GetInstance().SetSpaceRegistry(spaceRegistry);
    uint32_t opType = op::GenOpTypeId("Axpy");
    gert::TilingContext* tilingCtx = op::internal::OpRunContextMgr::opRunCtx_.UpdateTilingCtx(
        opType, *ctx->GetOpArg(op::OpArgDef::OP_INPUT_ARG), *ctx->GetOpArg(op::OpArgDef::OP_OUTPUT_ARG),
        *ctx->GetOpArg(op::OpArgDef::OP_ATTR_ARG));
    ASSERT_NE(tilingCtx, nullptr);

    // 写入已知tiling结果
    ASSERT_EQ(tilingCtx->SetTilingKey(123456UL), ge::GRAPH_SUCCESS);
    ASSERT_EQ(tilingCtx->SetSimdNumBlocks(8U), ge::GRAPH_SUCCESS);
    ASSERT_EQ(tilingCtx->SetNeedAtomic(true), ge::GRAPH_SUCCESS);
    ASSERT_EQ(tilingCtx->SetScheduleMode(2U), ge::GRAPH_SUCCESS);
    ASSERT_EQ(tilingCtx->SetDynUBufSize(4096U), ge::GRAPH_SUCCESS);
    // 负值验证int32有符号序列化
    constexpr int32_t tilingCondValue = -7;
    ASSERT_EQ(tilingCtx->SetTilingCond(tilingCondValue), ge::GRAPH_SUCCESS);
    auto* rawTilingData = tilingCtx->GetRawTilingData();
    ASSERT_NE(rawTilingData, nullptr);
    ASSERT_NE(rawTilingData->GetData(), nullptr);
    rawTilingData->SetDataSize(8U);
    const uint8_t tilingBytes[8] = {0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00};
    ASSERT_EQ(memcpy_s(rawTilingData->GetData(), rawTilingData->GetCapacity(), tilingBytes, sizeof(tilingBytes)), EOK);
    auto* wsSizes = tilingCtx->GetWorkspaceSizes(2U);
    ASSERT_NE(wsSizes, nullptr);
    wsSizes[0] = 1024U;
    wsSizes[1] = 4096U;

    aclnnOpInfoRecord::RuntimeInfo runtimeInfo;
    runtimeInfo.kernelName = "Axpy_233851a3505389e43928a8bba133a74d_high_performance";
    constexpr int64_t tilingArgsIndexValue = 3; // 已知args槽位
    runtimeInfo.tilingArgsIndex = tilingArgsIndexValue;

    // 四参调用写入tiling与runtime键
    EXPECT_EQ(aclnnOpInfoRecord::OpInfoSerialize(tilingCtx, opt, &kernelInfo, &runtimeInfo), 0);
    EXPECT_EQ(aclnnOpInfoRecord::OpInfoDump(), 0);
    op::DestroyOpArgContext(ctx);
}

// 场景：三参调用保持原json行为
TEST_F(OpInfoRecordUtest, Utest_OpInfoSerializeLegacyWithoutRuntimeInfo)
{
    aclnnOpInfoRecord::OpCompilerOption opt("", 0);
    aclnnOpInfoRecord::OpKernelInfo kernelInfo(OP_API_COMMON_UT_SRC_DIR
                                               "/built-in/op_impl/ai_core/tbe/kernel/ascend910/axpy/"
                                               "Axpy_233851a3505389e43928a8bba133a74d_high_performance.json",
                                               0);
    op::Shape shape{33, 15, 1, 48};
    auto self = std::make_unique<aclTensor>(shape, op::DataType::DT_FLOAT, op::Format::FORMAT_ND, nullptr);
    auto other = std::make_unique<aclTensor>(shape, op::DataType::DT_FLOAT, op::Format::FORMAT_ND, nullptr);
    auto out = std::make_unique<aclTensor>(shape, op::DataType::DT_FLOAT, op::Format::FORMAT_ND, nullptr);
    float alpha = 13.37;
    auto input = OP_INPUT(self.get(), other.get());
    auto output = OP_OUTPUT(out.get());
    auto attr = OP_ATTR(alpha);
    auto ctx = op::MakeOpArgContext(input, output, attr);
    auto spaceRegistry = std::make_shared<gert::OpImplSpaceRegistryV2>();
    gert::DefaultOpImplSpaceRegistryV2::GetInstance().SetSpaceRegistry(spaceRegistry);
    uint32_t opType = op::GenOpTypeId("Axpy");
    gert::TilingContext* tilingCtx = op::internal::OpRunContextMgr::opRunCtx_.UpdateTilingCtx(
        opType, *ctx->GetOpArg(op::OpArgDef::OP_INPUT_ARG), *ctx->GetOpArg(op::OpArgDef::OP_OUTPUT_ARG),
        *ctx->GetOpArg(op::OpArgDef::OP_ATTR_ARG));
    ASSERT_NE(tilingCtx, nullptr);

    // 已有tiling结果用于验证三参调用不写入新键
    ASSERT_EQ(tilingCtx->SetTilingKey(123456UL), ge::GRAPH_SUCCESS);
    ASSERT_EQ(tilingCtx->SetSimdNumBlocks(8U), ge::GRAPH_SUCCESS);

    // 三参调用保持原json行为
    EXPECT_EQ(aclnnOpInfoRecord::OpInfoSerialize(tilingCtx, opt, &kernelInfo), 0);
    EXPECT_EQ(aclnnOpInfoRecord::OpInfoDump(), 0);
    op::DestroyOpArgContext(ctx);
}

// 场景：kernelName为空时整键不写入
TEST_F(OpInfoRecordUtest, Utest_OpInfoSerializeEmptyKernelName)
{
    aclnnOpInfoRecord::OpCompilerOption opt("", 0);
    aclnnOpInfoRecord::OpKernelInfo kernelInfo(OP_API_COMMON_UT_SRC_DIR
                                               "/built-in/op_impl/ai_core/tbe/kernel/ascend910/axpy/"
                                               "Axpy_233851a3505389e43928a8bba133a74d_high_performance.json",
                                               0);
    op::Shape shape{33, 15, 1, 48};
    auto self = std::make_unique<aclTensor>(shape, op::DataType::DT_FLOAT, op::Format::FORMAT_ND, nullptr);
    auto other = std::make_unique<aclTensor>(shape, op::DataType::DT_FLOAT, op::Format::FORMAT_ND, nullptr);
    auto out = std::make_unique<aclTensor>(shape, op::DataType::DT_FLOAT, op::Format::FORMAT_ND, nullptr);
    float alpha = 13.37;
    auto input = OP_INPUT(self.get(), other.get());
    auto output = OP_OUTPUT(out.get());
    auto attr = OP_ATTR(alpha);
    auto ctx = op::MakeOpArgContext(input, output, attr);
    auto spaceRegistry = std::make_shared<gert::OpImplSpaceRegistryV2>();
    gert::DefaultOpImplSpaceRegistryV2::GetInstance().SetSpaceRegistry(spaceRegistry);
    uint32_t opType = op::GenOpTypeId("Axpy");
    gert::TilingContext* tilingCtx = op::internal::OpRunContextMgr::opRunCtx_.UpdateTilingCtx(
        opType, *ctx->GetOpArg(op::OpArgDef::OP_INPUT_ARG), *ctx->GetOpArg(op::OpArgDef::OP_OUTPUT_ARG),
        *ctx->GetOpArg(op::OpArgDef::OP_ATTR_ARG));
    ASSERT_NE(tilingCtx, nullptr);

    ASSERT_EQ(tilingCtx->SetTilingKey(42UL), ge::GRAPH_SUCCESS);

    aclnnOpInfoRecord::RuntimeInfo runtimeInfo; // kernelName默认为空
    runtimeInfo.tilingArgsIndex = -1;

    EXPECT_EQ(aclnnOpInfoRecord::OpInfoSerialize(tilingCtx, opt, &kernelInfo, &runtimeInfo), 0);
    EXPECT_EQ(aclnnOpInfoRecord::OpInfoDump(), 0);
    op::DestroyOpArgContext(ctx);
}

TEST_F(OpInfoRecordUtest, Utest_OpInfoSerialize_without_supportInfo)
{
    aclnnOpInfoRecord::OpCompilerOption opt("", 0);
    aclnnOpInfoRecord::OpKernelInfo kernelInfo(
        OP_API_COMMON_UT_SRC_DIR "/built-in/op_impl/ai_core/tbe/kernel/config/ascend910/add_n.json", 0);
    gert::TilingContext ctx;
    EXPECT_EQ(aclnnOpInfoRecord::OpInfoSerialize(&ctx, opt, &kernelInfo), -1);
    EXPECT_EQ(aclnnOpInfoRecord::OpInfoDump(), 0);
}

TEST_F(OpInfoRecordUtest, Utest_OpInfoPathEqual)
{
    std::string dependPath = NNOPBASE_UT_DEPENDS_DIR "/";
    aclnnOpInfoRecord::Path pathA = aclnnOpInfoRecord::Path(dependPath);
    aclnnOpInfoRecord::Path pathB = aclnnOpInfoRecord::Path(dependPath);
    std::string configPath = OP_API_COMMON_UT_SRC_DIR "/built-in/op_impl/ai_core/tbe/kernel/config/";
    aclnnOpInfoRecord::Path pathC = aclnnOpInfoRecord::Path(configPath);
    EXPECT_EQ(pathA == pathB, true);
    EXPECT_EQ(pathA == pathC, false);
}

// 场景：不设置tilingCond时透传默认值
TEST_F(OpInfoRecordUtest, Utest_OpInfoSerializeTilingCondDefault)
{
    aclnnOpInfoRecord::OpCompilerOption opt("", 0);
    aclnnOpInfoRecord::OpKernelInfo kernelInfo(OP_API_COMMON_UT_SRC_DIR
                                               "/built-in/op_impl/ai_core/tbe/kernel/ascend910/axpy/"
                                               "Axpy_233851a3505389e43928a8bba133a74d_high_performance.json",
                                               0);
    op::Shape shape{33, 15, 1, 48};
    auto self = std::make_unique<aclTensor>(shape, op::DataType::DT_FLOAT, op::Format::FORMAT_ND, nullptr);
    auto other = std::make_unique<aclTensor>(shape, op::DataType::DT_FLOAT, op::Format::FORMAT_ND, nullptr);
    auto out = std::make_unique<aclTensor>(shape, op::DataType::DT_FLOAT, op::Format::FORMAT_ND, nullptr);
    float alpha = 13.37;
    auto input = OP_INPUT(self.get(), other.get());
    auto output = OP_OUTPUT(out.get());
    auto attr = OP_ATTR(alpha);
    auto ctx = op::MakeOpArgContext(input, output, attr);
    auto spaceRegistry = std::make_shared<gert::OpImplSpaceRegistryV2>();
    gert::DefaultOpImplSpaceRegistryV2::GetInstance().SetSpaceRegistry(spaceRegistry);
    uint32_t opType = op::GenOpTypeId("Axpy");
    gert::TilingContext* tilingCtx = op::internal::OpRunContextMgr::opRunCtx_.UpdateTilingCtx(
        opType, *ctx->GetOpArg(op::OpArgDef::OP_INPUT_ARG), *ctx->GetOpArg(op::OpArgDef::OP_OUTPUT_ARG),
        *ctx->GetOpArg(op::OpArgDef::OP_ATTR_ARG));
    ASSERT_NE(tilingCtx, nullptr);

    aclnnOpInfoRecord::RuntimeInfo runtimeInfo;

    EXPECT_EQ(aclnnOpInfoRecord::OpInfoSerialize(tilingCtx, opt, &kernelInfo, &runtimeInfo), 0);
    EXPECT_EQ(aclnnOpInfoRecord::OpInfoDump(), 0);
    op::DestroyOpArgContext(ctx);
}
