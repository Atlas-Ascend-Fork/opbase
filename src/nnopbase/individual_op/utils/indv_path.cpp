/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "indv_path.h"

namespace nnopbase {
bool IndvPath::ReplacePathSuffix(const std::string& path, const std::string& fromSuffix, const std::string& toSuffix,
                                 std::string& pathWithNewSuffix)
{
    if (fromSuffix.empty()) {
        return false;
    }

    const auto suffixPos = path.rfind(fromSuffix);
    if (suffixPos == std::string::npos || (suffixPos + fromSuffix.size()) != path.size()) {
        return false;
    }

    pathWithNewSuffix = path.substr(0U, suffixPos) + toSuffix;
    return true;
}

} // namespace nnopbase
