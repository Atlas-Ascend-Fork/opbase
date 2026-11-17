/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef OP_API_OP_API_COMMON_INC_OPDEV_INTERNAL_KERNEL_CONTEXT_HOLDER_H
#define OP_API_OP_API_COMMON_INC_OPDEV_INTERNAL_KERNEL_CONTEXT_HOLDER_H
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <vector>

#include "exe_graph/runtime/kernel_run_context.h"
#include "kernel_arg.h"
#include "kernel_utils.h"
#include "op_ctx_def.h"
#include "opdev/op_errno.h"
#include "opdev/op_dfx.h"
#include "nnopbase_error_msg.h"
#include "utils/extern_math_util.h"

namespace op::internal {
class KernelContextHolder {
public:
    KernelContextHolder()
    {
        BuildComputeNodeInfo();
        BuildOpInOutArg();
    };

    ~KernelContextHolder();

    KernelContextHolder(const KernelContextHolder&) = delete;
    KernelContextHolder(KernelContextHolder&&) = delete;
    KernelContextHolder& operator=(const KernelContextHolder&) = delete;
    KernelContextHolder& operator=(KernelContextHolder&&) = delete;

    void UpdateOutputArgIr(OpArgList& output) const
    {
        size_t outputIdx = 0;
        output.VisitByNoReturn([this, &outputIdx](size_t idx, OpArg& arg) { UpdateOutputArgIr(idx, arg, outputIdx); });
    }

    aclnnStatus UpdateComputeNodeInfo(const char* opType, OpArgList& input, OpArgList& output, OpArgList& attr)
    {
        ResetComputeNodeInfo(opType, input.count, output.count);
        UpdateCompileDescOffset(input.count);
        CHECK_RET_CODE(input.VisitBy([this](size_t idx, OpArg& arg) { return UpdateInputArg(idx, arg); }),
                       "UpdateInputArg failed.");
        CHECK_RET_CODE(output.VisitBy([this](size_t idx, OpArg& arg) { return UpdateOutputArg(idx, arg); }),
                       "UpdateOutputArg failed.");
        FinalizeComputeNodeInfo(attr.count);
        void* attrPtr = attrDataStart_;
        CHECK_RET_CODE(
            attr.VisitBy([this, &attrPtr](size_t idx, OpArg& arg) { return UpdateAttrArg(idx, arg, attrPtr); }),
            "Update Attr Arg failed");
        // 输出锚点紧随属性数据存放，需与属性区一起纳入容量核算，否则锚点写入会越过已分配区域。
        const size_t usedAttrBytes = PtrOffset(attrDataStart_, attrPtr);
        size_t outputAnchorBytes = 0U;
        size_t offsetTableBytes = 0U;
        size_t requiredAttrBytes = 0U;
        if (unlikely(ge::MulOverflow(sizeof(AnchorInstanceInfo), irOutputNum_, outputAnchorBytes) ||
                     ge::MulOverflow(sizeof(size_t), attrNum_, offsetTableBytes) ||
                     ge::AddOverflow(usedAttrBytes, outputAnchorBytes, requiredAttrBytes) ||
                     ge::AddOverflow(requiredAttrBytes, offsetTableBytes, requiredAttrBytes))) {
            OP_LOGE(ACLNN_ERR_INNER, "Attr region size overflow: used %zu bytes, output num %zu, attr num %zu.",
                    usedAttrBytes, irOutputNum_, attrNum_);
            return ACLNN_ERR_INNER;
        }
        CHECK_RET_CODE(EnsureAttrCapacity(requiredAttrBytes), "EnsureAttrCapacity failed.");

        attrPtr = PtrShift(attrDataStart_, usedAttrBytes);
        computeNodeInfo_->attr_size_ = PtrOffset(attrDef_, attrPtr);
        outputAnchorInfo_ = PtrCastTo<AnchorInstanceInfo>(attrPtr);
        UpdateOutputArgIr(output);
        return ACLNN_SUCCESS;
    }

    void UpdateKernelExtendInfo(const char* kernelType, const char* kernelName);
    void ResetComputeNodeInfo(const char* opType, size_t irInputNum, size_t irOutputNum);
    void UpdateCompileDescOffset(size_t irInputNum);
    void FinalizeComputeNodeInfo(size_t attrNum);

    template <size_t N>
    void AppendAttr(const std::array<size_t, N>& attrSize, std::array<void*, N>& attrAddr) const
    {
        void* attrPtr = attrDataStart_;
        for (size_t i = 0; i < N; i++) {
            attrDef_->offset[i] = PtrOffset(attrDef_, attrPtr);
            attrAddr[i] = attrPtr;
            attrPtr = PtrShift(attrPtr, attrSize[i]);
        }
    };

    size_t inputNum_{0};
    size_t outputNum_{0};
    AsyncAnyValue* opInArg_{nullptr};
    void* attrDataStart_{nullptr};

    ComputeNodeInfo* computeNodeInfo_{nullptr};
    KernelExtendInfo kernelExtendInfo_;

    // Dynamic capacity management
    // Initial capacity set to MAX_OP_ARG_NUM and ATTR_CAPACITY
    // Expansion is triggered only when capacity exceeds these constant values
    size_t opInArgCapacity_{MAX_OP_ARG_NUM};
    size_t computeNodeInfoCapacity_{MAX_OP_ARG_NUM};
    size_t attrCapacity_{ATTR_CAPACITY};

private:
    void BuildComputeNodeInfo();
    void BuildOpInOutArg();

    aclnnStatus EnsureArgCapacity(size_t requiredCapacity);
    aclnnStatus EnsureComputeNodeInfoCapacity(size_t requiredCapacity);
    aclnnStatus EnsureAttrCapacity(size_t requiredSize);
    aclnnStatus PrepareAttrWrite(size_t idx, void*& attrPtr, size_t needBytes, size_t* remainBytes = nullptr);

    void UpdateAttrDefOffset(size_t inoutNum, size_t attrNum);

    aclnnStatus UpdateInputArg(size_t idx, OpArg& arg);
    aclnnStatus UpdateInputArg(size_t idx, const aclTensor* tensor);
    aclnnStatus UpdateInputArg(size_t idx, const aclTensorList* tensorList);
    aclnnStatus UpdateOutputArg(size_t idx, OpArg& arg);
    aclnnStatus UpdateOutputArg(size_t idx, const aclTensor* tensor);
    aclnnStatus UpdateOutputArg(size_t idx, const aclTensorList* tensorList);

    void UpdateOutputArgIr(size_t idx, OpArg& arg, size_t& seq) const;
    void UpdateOutputArgIr(size_t idx, const aclTensor* tensor, size_t& seq) const;
    void UpdateOutputArgIr(size_t idx, const aclTensorList* tensorList, size_t& seq) const;

    inline aclnnStatus UpdateAttrArg(size_t idx, op::DataType value, void*& attrPtr)
    {
        OP_LOGD("Update Attr DataType: [%zu], %d", idx, value);
        CHECK_RET_CODE(PrepareAttrWrite(idx, attrPtr, sizeof(int64_t)), "PrepareAttrWrite failed.");
        *static_cast<int64_t*>(attrPtr) = 0L;
        *static_cast<DataType*>(attrPtr) = value;
        attrPtr = PtrShift(attrPtr, sizeof(int64_t));
        return ACLNN_SUCCESS;
    }

    inline aclnnStatus UpdateAttrArg(size_t idx, int64_t value, void*& attrPtr)
    {
#ifdef DEBUG
        std::stringstream ss;
        ss << value;
        OP_LOGD("Update Attr arg Scalar: [%zu] %s", idx, ss.str().c_str());
#else
        OP_LOGD("Update Attr arg Scalar: [%zu]", idx);
#endif
        CHECK_RET_CODE(PrepareAttrWrite(idx, attrPtr, sizeof(int64_t)), "PrepareAttrWrite failed.");
        *static_cast<int64_t*>(attrPtr) = value;
        attrPtr = PtrShift(attrPtr, sizeof(uint64_t));
        return ACLNN_SUCCESS;
    }

    inline aclnnStatus UpdateAttrArg(size_t idx, double value, void*& attrPtr)
    {
#ifdef DEBUG
        std::stringstream ss;
        ss << value;
        OP_LOGD("Update Attr arg Scalar: [%zu] %s", idx, ss.str().c_str());
#else
        OP_LOGD("Update Attr arg Scalar: [%zu]", idx);
#endif
        CHECK_RET_CODE(PrepareAttrWrite(idx, attrPtr, sizeof(double)), "PrepareAttrWrite failed.");
        *static_cast<double*>(attrPtr) = value;
        attrPtr = PtrShift(attrPtr, sizeof(double));
        return ACLNN_SUCCESS;
    }

    inline aclnnStatus UpdateAttrArg(size_t idx, float value, void*& attrPtr)
    {
#ifdef DEBUG
        std::stringstream ss;
        ss << value;
        OP_LOGD("Update Attr arg Scalar: [%zu] %s", idx, ss.str().c_str());
#else
        OP_LOGD("Update Attr arg Scalar: [%zu]", idx);
#endif
        CHECK_RET_CODE(PrepareAttrWrite(idx, attrPtr, sizeof(int64_t)), "PrepareAttrWrite failed.");
        *static_cast<int64_t*>(attrPtr) = 0L;
        *static_cast<float*>(attrPtr) = value;
        attrPtr = PtrShift(attrPtr, sizeof(int64_t));
        return ACLNN_SUCCESS;
    }

    aclnnStatus UpdateAttrArg(size_t idx, char* value, void*& attrPtr)
    {
        if (value == nullptr) {
            OP_LOGW("Update Attr NULL char*: [%zu]", idx);
            CHECK_RET_CODE(PrepareAttrWrite(idx, attrPtr, sizeof(int64_t)), "PrepareAttrWrite failed.");
            *static_cast<char*>(attrPtr) = '\0';
            attrPtr = PtrShift(attrPtr, sizeof(int64_t));
        } else {
            const size_t slen = strlen(value);
            if (slen >= ATTR_STRING_LENGTH_WARN_THRESHOLD) {
                OP_LOGW("Attr string at index %zu is too long: length %zu bytes, warning threshold %zu bytes.", idx,
                        slen, ATTR_STRING_LENGTH_WARN_THRESHOLD);
            }
            const size_t needBytes = AlignSize(slen + 1, sizeof(size_t));
            size_t remainBytes = 0U;
            CHECK_RET_CODE(PrepareAttrWrite(idx, attrPtr, needBytes, &remainBytes), "PrepareAttrWrite failed.");
            OP_CHECK(memcpy_s(attrPtr, remainBytes, value, slen + 1) == EOK,
                     OP_LOGW("Failed to memcpy in update attr arg for char."),
                     ;);
            attrPtr = PtrShift(attrPtr, needBytes);
#if defined(NNOPBASE_UT) || defined(NNOPBASE_ST)
            OP_LOGD(
                "Update Attr char*: [%zu], length: %zu, need bytes: %zu, remaining bytes before output anchors: %zu.",
                idx, slen, needBytes, remainBytes - needBytes);
#else
            OP_LOGD("Update Attr char*: [%zu], length: %zu, need bytes: %zu, remaining bytes before output anchors: "
                    "%zu. %s",
                    idx, slen, needBytes, remainBytes - needBytes, value);
#endif
        }
        return ACLNN_SUCCESS;
    }

    aclnnStatus UpdateAttrArg(size_t idx, aclScalar* value, void*& attrPtr)
    {
        if (value == nullptr) {
            OP_LOGW("Update Attr NULL aclScalar*: [%zu]", idx);
            CHECK_RET_CODE(PrepareAttrWrite(idx, attrPtr, sizeof(int64_t)), "PrepareAttrWrite failed.");
            *static_cast<int64_t*>(attrPtr) = 0;
            attrPtr = PtrShift(attrPtr, sizeof(int64_t));
        } else {
            const size_t scalarBytes = value->Size();
            size_t remainBytes = 0U;
            CHECK_RET_CODE(PrepareAttrWrite(idx, attrPtr, scalarBytes, &remainBytes), "PrepareAttrWrite failed.");
            OP_CHECK(memcpy_s(attrPtr, remainBytes, value->GetData(), scalarBytes) == EOK,
                     OP_LOGW("Failed to memcpy in update attr arg for aclScalar."),
                     ;);
            attrPtr = PtrShift(attrPtr, scalarBytes);
            OP_LOGD("Update Attr aclScalar*: [%zu], need bytes: %zu, remaining bytes before output anchors: %zu.", idx,
                    scalarBytes, remainBytes - scalarBytes);
        }
        return ACLNN_SUCCESS;
    }

    template <typename ArrayT>
    aclnnStatus UpdateAttrArrayArg(size_t idx, const ArrayT* array, const char* arrayTypeName, void*& attrPtr)
    {
        if (array == nullptr || array->Size() == 0) {
            OP_LOGW("Update Attr NULL %s*: [%zu]", arrayTypeName, idx);
            CHECK_RET_CODE(PrepareAttrWrite(idx, attrPtr, sizeof(gert::ContinuousVector)), "PrepareAttrWrite failed.");
            auto* attrBlock = static_cast<gert::ContinuousVector*>(attrPtr);
            attrBlock->Init(0);
            attrPtr = PtrShift(attrPtr, sizeof(gert::ContinuousVector));
            return ACLNN_SUCCESS;
        }

        const size_t elemCount = array->Size();
        size_t dataBytes = 0U;
        OP_CHECK(!ge::MulOverflow(elemCount, sizeof(*array->GetData()), dataBytes),
                 OP_LOGE(ACLNN_ERR_INNER, "Attr array size overflow: [%zu], elem count %zu.", idx, elemCount),
                 return ACLNN_ERR_INNER);

        auto* attrBlock = static_cast<gert::ContinuousVector*>(attrPtr);
        const size_t headerBytes = PtrOffset(attrBlock, attrBlock->GetData());
        size_t needBytes = 0U;
        OP_CHECK(!ge::AddOverflow(headerBytes, dataBytes, needBytes),
                 OP_LOGE(ACLNN_ERR_INNER, "Attr array size overflow: [%zu].", idx), return ACLNN_ERR_INNER);

        size_t remainBytes = 0U;
        CHECK_RET_CODE(PrepareAttrWrite(idx, attrPtr, needBytes, &remainBytes), "PrepareAttrWrite failed.");

        // 扩容会整块搬迁，必须重新取 attrBlock
        attrBlock = static_cast<gert::ContinuousVector*>(attrPtr);
        attrBlock->Init(elemCount);
        attrBlock->SetSize(elemCount);
        OP_CHECK(memcpy_s(attrBlock->MutableData(), remainBytes - headerBytes, array->GetData(), dataBytes) == EOK,
                 OP_LOGW("Failed to memcpy in update attr arg for %s.", arrayTypeName),
                 ;);
        attrPtr = PtrShift(attrPtr, needBytes);
        OP_LOGD("Update Attr %s*: [%zu], elem count: %zu, payload bytes: %zu, need bytes: %zu, remaining bytes before "
                "output anchors: %zu.",
                arrayTypeName, idx, elemCount, dataBytes, needBytes, remainBytes - needBytes);
        return ACLNN_SUCCESS;
    }

    aclnnStatus UpdateAttrArg(size_t idx, aclIntArray* value, void*& attrPtr)
    {
        return UpdateAttrArrayArg(idx, value, "aclIntArray", attrPtr);
    }

    aclnnStatus UpdateAttrArg(size_t idx, aclFloatArray* fvalue, void*& attrPtr)
    {
        return UpdateAttrArrayArg(idx, fvalue, "aclFloatArray", attrPtr);
    }

    aclnnStatus UpdateAttrArg(size_t idx, aclBoolArray* value, void*& attrPtr)
    {
        return UpdateAttrArrayArg(idx, value, "aclBoolArray", attrPtr);
    }

    aclnnStatus UpdateAttrArg(size_t idx, OpArg& arg, void*& attrPtr)
    {
        switch (arg.type) {
            case OpArgType::OPARG_DATATYPE:
                return UpdateAttrArg(idx, static_cast<op::DataType>(arg->value), attrPtr);
            case OpArgType::OPARG_BOOL:
            case OpArgType::OPARG_INT:
            case OpArgType::OPARG_UINT:
            case OpArgType::OPARG_IMPLMODE:
                return UpdateAttrArg(idx, arg->ivalue, attrPtr);
            case OpArgType::OPARG_FLOAT:
                return UpdateAttrArg(idx, arg->fvalue, attrPtr);
            case OpArgType::OPARG_DOUBLE:
                return UpdateAttrArg(idx, arg->dvalue, attrPtr);
            case OpArgType::OPARG_STRING:
                return UpdateAttrArg(idx, reinterpret_cast<char*>(arg->pointer), attrPtr);
            case OpArgType::OPARG_ACLSCALAR:
                return UpdateAttrArg(idx, reinterpret_cast<aclScalar*>(arg->pointer), attrPtr);
            case OpArgType::OPARG_INT_LIST:
                return UpdateAttrArg(idx, reinterpret_cast<aclIntArray*>(arg->pointer), attrPtr);
            case OpArgType::OPARG_FLOAT_LIST:
                return UpdateAttrArg(idx, reinterpret_cast<aclFloatArray*>(arg->pointer), attrPtr);
            case OpArgType::OPARG_BOOL_LIST:
                return UpdateAttrArg(idx, reinterpret_cast<aclBoolArray*>(arg->pointer), attrPtr);
            default:
                std::string
                    argTypeRange = "[OPARG_ACLSCALAR(2), OPARG_STRING(3), OPARG_BOOL(4), OPARG_INT(5), "
                                   "OPARG_UINT(6), OPARG_FLOAT(7), OPARG_DOUBLE(8), OPARG_DATATYPE(9), "
                                   "OPARG_INT_LIST(10), OPARG_FLOAT_LIST(12), OPARG_BOOL_LIST(13), OPARG_IMPLMODE(14)]";
                auto typeStr = OpArgTypeToStr(arg.type);
                OP_LOGE_FOR_NOT_SUPPORTED_DATA_TYPE(typeStr.GetString(), argTypeRange.c_str());
                return ACLNN_ERR_INNER;
        }
    }

    size_t irInputNum_{0};
    size_t irOutputNum_{0};
    size_t computeNodeInfoSize_{0};

    AnchorInstanceInfo* anchorInfo_{nullptr};
    AnchorInstanceInfo* outputAnchorInfo_{nullptr};
    CompileTimeTensorDesc* compileDesc_{nullptr};
    RuntimeAttrsDef* attrDef_{nullptr};
    size_t attrNum_{0};
};

} // namespace op::internal

#endif // OP_API_OP_API_COMMON_INC_OPDEV_INTERNAL_KERNEL_CONTEXT_HOLDER_H
