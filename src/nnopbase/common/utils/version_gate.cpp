/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "version_gate.h"
#include "acl/acl_rt.h"
#include "opdev/op_log.h"

namespace op {
namespace internal {
#ifndef PRODUCT_SIDE_IS_DEVICE
struct QueryResult {
    aclError ret;
    int32_t version;
};

QueryResult QueryOnce(const Pkg pkg)
{
    int32_t version = -1;
    const char* pkgName = (pkg == Pkg::RUNTIME) ? "runtime" : "metadef";
    const aclError ret = aclsysGetVersionNum(const_cast<char*>(pkgName), &version);
    return QueryResult{ret, version};
}

#if defined(NNOPBASE_UT) || defined(NNOPBASE_ST)
struct QueryCache {
    QueryResult result{};
    bool initialized = false;
};

QueryCache gRuntimeQueryCache{};
QueryCache gMetaDefQueryCache{};
#endif

const QueryResult& GetRuntimeQueryResult()
{
#if defined(NNOPBASE_UT) || defined(NNOPBASE_ST)
    if (!gRuntimeQueryCache.initialized) {
        gRuntimeQueryCache.result = QueryOnce(Pkg::RUNTIME);
        gRuntimeQueryCache.initialized = true;
    }
    return gRuntimeQueryCache.result;
#else
    static const QueryResult result = QueryOnce(Pkg::RUNTIME);
    return result;
#endif
}

const QueryResult& GetMetaDefQueryResult()
{
#if defined(NNOPBASE_UT) || defined(NNOPBASE_ST)
    if (!gMetaDefQueryCache.initialized) {
        gMetaDefQueryCache.result = QueryOnce(Pkg::METADEF);
        gMetaDefQueryCache.initialized = true;
    }
    return gMetaDefQueryCache.result;
#else
    static const QueryResult result = QueryOnce(Pkg::METADEF);
    return result;
#endif
}

#if defined(NNOPBASE_UT) || defined(NNOPBASE_ST)
void ResetVersionQueryCacheForTest()
{
    gRuntimeQueryCache = QueryCache{};
    gMetaDefQueryCache = QueryCache{};
}
#endif

FeatureState CheckVersionOnce(const char* featureName, std::initializer_list<VersionGate> requirements)
{
    for (const VersionGate& requirement : requirements) {
        const QueryResult& queryResult = (requirement.pkg == Pkg::RUNTIME) ? GetRuntimeQueryResult() :
                                                                             GetMetaDefQueryResult();
        if (queryResult.ret != ACL_SUCCESS) {
            OP_LOGE(ACLNN_ERR_INNER, "Feature %s is unsupported: query version failed, ret is %d.", featureName,
                    static_cast<int32_t>(queryResult.ret));
            return FeatureState::QUERY_FAILED;
        }

        if (queryResult.version < requirement.minVersion) {
            OP_LOGW("Feature %s is unsupported: real version is %d, required is %d.", featureName, queryResult.version,
                    requirement.minVersion);
            return FeatureState::RUNTIME_UNSUPPORTED;
        }
    }
    return FeatureState::SUPPORTED;
}
#endif

FeatureState CheckVersion(const char* featureName, std::initializer_list<VersionGate> requirements)
{
#ifdef PRODUCT_SIDE_IS_DEVICE
    (void)featureName;
    (void)requirements;
    return FeatureState::DEVICE_BUILD;
#else
    return CheckVersionOnce(featureName, requirements);
#endif
}

} // namespace internal
} // namespace op
