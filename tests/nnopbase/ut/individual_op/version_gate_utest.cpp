/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "gtest/gtest.h"

#include "version_gate.h"

extern int32_t g_aclsysVersionNum;

#ifdef NNOPBASE_UT
namespace op::internal {
void ResetVersionQueryCacheForTest();
}
#endif

namespace {
using namespace op::internal;

class VersionGateTest : public testing::Test {
protected:
    void SetUp() override
    {
        g_aclsysVersionNum = 90200000;
        (void)CheckVersion("ut_prime_runtime", {RuntimeAtLeast(1)});
        (void)CheckVersion("ut_prime_metadef", {MetaDefAtLeast(1)});
        ResetVersionQueryCacheForTest();
        g_aclsysVersionNum = 0;
    }

    void TearDown() override
    {
        ResetVersionQueryCacheForTest();
        g_aclsysVersionNum = 90200000;
    }
};

TEST_F(VersionGateTest, CheckRuntimeAndMetaDefVersions)
{
    EXPECT_EQ(CheckVersion("ut_runtime", {RuntimeAtLeast(1)}), FeatureState::RUNTIME_UNSUPPORTED);
    EXPECT_EQ(CheckVersion("ut_metadef", {MetaDefAtLeast(1)}), FeatureState::RUNTIME_UNSUPPORTED);

    EXPECT_EQ(CheckVersion("ut_high_runtime", {RuntimeAtLeast(99900000)}), FeatureState::RUNTIME_UNSUPPORTED);
    EXPECT_EQ(CheckVersion("ut_high_metadef", {MetaDefAtLeast(99900000)}), FeatureState::RUNTIME_UNSUPPORTED);
}

TEST_F(VersionGateTest, ResetVersionQueryCacheRequeriesChangedVersion)
{
    g_aclsysVersionNum = 90200000;
    ResetVersionQueryCacheForTest();
    EXPECT_EQ(CheckVersion("ut_runtime_after_reset", {RuntimeAtLeast(90200000)}), FeatureState::SUPPORTED);
    EXPECT_EQ(CheckVersion("ut_metadef_after_reset", {MetaDefAtLeast(90200000)}), FeatureState::SUPPORTED);

    g_aclsysVersionNum = 0;
    ResetVersionQueryCacheForTest();
    EXPECT_EQ(CheckVersion("ut_runtime_after_second_reset", {RuntimeAtLeast(1)}), FeatureState::RUNTIME_UNSUPPORTED);
    EXPECT_EQ(CheckVersion("ut_metadef_after_second_reset", {MetaDefAtLeast(1)}), FeatureState::RUNTIME_UNSUPPORTED);
}

TEST_F(VersionGateTest, CompileMacrosUseSeparatePackages)
{
#if NNOPBASE_RUNTIME_VERSION_AT_LEAST(90000000)
    EXPECT_TRUE(NNOPBASE_RUNTIME_VERSION_AT_LEAST(90000000));
#else
    EXPECT_FALSE(NNOPBASE_RUNTIME_VERSION_AT_LEAST(90000000));
#endif

#if NNOPBASE_METADEF_VERSION_AT_LEAST(90000000)
    EXPECT_TRUE(NNOPBASE_METADEF_VERSION_AT_LEAST(90000000));
#else
    EXPECT_FALSE(NNOPBASE_METADEF_VERSION_AT_LEAST(90000000));
#endif
    EXPECT_FALSE(NNOPBASE_RUNTIME_VERSION_AT_LEAST(99900000));
    EXPECT_FALSE(NNOPBASE_METADEF_VERSION_AT_LEAST(99900000));
}
} // namespace
