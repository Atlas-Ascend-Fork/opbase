/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef NNOPBASE_COMMON_INC_VERSION_GATE_H_
#define NNOPBASE_COMMON_INC_VERSION_GATE_H_

#include <cstdint>
#include <initializer_list>

#ifndef PRODUCT_SIDE_IS_DEVICE
#include "version/runtime_version.h"
#include "version/metadef_version.h"
#endif

namespace op {
namespace internal {

enum class Pkg : uint8_t { RUNTIME, METADEF };

struct VersionGate {
    Pkg pkg;
    int32_t minVersion;
};

constexpr VersionGate RuntimeAtLeast(const int32_t minVersion) { return VersionGate{Pkg::RUNTIME, minVersion}; }

constexpr VersionGate MetaDefAtLeast(const int32_t minVersion) { return VersionGate{Pkg::METADEF, minVersion}; }

enum class FeatureState : uint8_t { SUPPORTED, DEVICE_BUILD, RUNTIME_UNSUPPORTED, QUERY_FAILED };

FeatureState CheckVersion(const char* featureName, std::initializer_list<VersionGate> requirements);

} // namespace internal
} // namespace op

/*
 * The compile-time checks are intentionally separated. A feature which depends on both packages shall combine
 * them with && at its own call site, so each macro has a single responsibility.
 */
#ifndef PRODUCT_SIDE_IS_DEVICE
#ifdef RUNTIME_VERSION_NUM
#define NNOPBASE_RUNTIME_VERSION_AT_LEAST(version) ((RUNTIME_VERSION_NUM) >= (version))
#else
#define NNOPBASE_RUNTIME_VERSION_AT_LEAST(version) 0
#endif
#ifdef METADEF_VERSION_NUM
#define NNOPBASE_METADEF_VERSION_AT_LEAST(version) ((METADEF_VERSION_NUM) >= (version))
#else
#define NNOPBASE_METADEF_VERSION_AT_LEAST(version) 0
#endif
#else
#define NNOPBASE_RUNTIME_VERSION_AT_LEAST(version) 0
#define NNOPBASE_METADEF_VERSION_AT_LEAST(version) 0
#endif

// Feature version macros shared by composite_op and individual_op.
#define NNOPBASE_CUSTOM_EXCEPTION_CALLBACK_MIN_RUNTIME_VERSION 90000000
#define NNOPBASE_CUSTOM_EXCEPTION_CALLBACK_MIN_METADEF_VERSION 90000000
#define NNOPBASE_STREAM_NO_BLOCKING_MIN_RUNTIME_VERSION 90200000
#define NNOPBASE_PCIE_THROUGH_MIN_RUNTIME_VERSION 90200000
#define NNOPBASE_OP_PROFILING_MIN_RUNTIME_VERSION 90200000

#endif // NNOPBASE_COMMON_INC_VERSION_GATE_H_
