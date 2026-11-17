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

#include <cstdint>
#include <cstdlib>
#include <numeric>
#include <string>
#include <vector>

#include "acl/acl.h"
#include "aclnn/acl_meta.h"
#include "exe_graph/runtime/continuous_vector.h"
#include "kernel_context_holder.h"
#include "op_ctx_def.h"
#include "opdev/op_arg_def.h"
#include "thread_local_context.h"

using namespace op::internal;

namespace {
constexpr size_t kLargeElemCount = 10000;
constexpr size_t kAttrGrowthFactor = 2U;
} // namespace

class AttrDynamicCapacityUT : public testing::Test {
protected:
    static void SetUpTestCase()
    {
        setenv("ASCEND_OPP_PATH", OP_API_COMMON_UT_SRC_DIR, 1);
        GetThreadLocalContext().cacheHasFull_ = true;
    }
    static void TearDownTestCase() {}
};

static aclTensor* MakeTensor()
{
    static int64_t shape[] = {1, 2, 3};
    return aclCreateTensor(shape, 3, aclDataType::ACL_FLOAT, nullptr, 0, aclFormat::ACL_FORMAT_ND, shape, 3, nullptr);
}

static op::OpArg MakeTensorArg(aclTensor* tensor)
{
    op::OpArg arg;
    arg.type = op::OpArgType::OPARG_ACLTENSOR;
    arg.value.data.pointer = tensor;
    return arg;
}

static op::OpArg MakeIntArrayArg(aclIntArray* array)
{
    op::OpArg arg;
    arg.type = op::OpArgType::OPARG_INT_LIST;
    arg.value.data.pointer = array;
    return arg;
}

static op::OpArg MakeStringArg(std::string& storage)
{
    op::OpArg arg;
    arg.type = op::OpArgType::OPARG_STRING;
    arg.value.data.pointer = storage.data();
    return arg;
}

static op::OpArg MakeNullStringArg()
{
    op::OpArg arg;
    arg.type = op::OpArgType::OPARG_STRING;
    arg.value.data.pointer = nullptr;
    return arg;
}

static op::OpArg MakeScalarArg(aclScalar* scalar)
{
    op::OpArg arg;
    arg.type = op::OpArgType::OPARG_ACLSCALAR;
    arg.value.data.pointer = scalar;
    return arg;
}

static op::OpArg MakeFloatArrayArg(aclFloatArray* array)
{
    op::OpArg arg;
    arg.type = op::OpArgType::OPARG_FLOAT_LIST;
    arg.value.data.pointer = array;
    return arg;
}

static op::OpArg MakeBoolArrayArg(aclBoolArray* array)
{
    op::OpArg arg;
    arg.type = op::OpArgType::OPARG_BOOL_LIST;
    arg.value.data.pointer = array;
    return arg;
}

static aclnnStatus RunUpdate(KernelContextHolder& holder, std::vector<op::OpArg>& inputs,
                             std::vector<op::OpArg>& outputs, std::vector<op::OpArg>& attrs)
{
    op::OpArgList inputList(inputs.data(), inputs.size());
    op::OpArgList outputList(outputs.data(), outputs.size());
    op::OpArgList attrList(attrs.data(), attrs.size());
    return holder.UpdateComputeNodeInfo("Axpy", inputList, outputList, attrList);
}

static const uint8_t* AttrAddr(const KernelContextHolder& holder, size_t idx)
{
    return reinterpret_cast<const uint8_t*>(holder.attrDef_) + holder.attrDef_->offset[idx];
}

// 属性区容量按 kAttrGrowthFactor 倍从 ATTR_CAPACITY 递增，直到覆盖需求。
static size_t ExpectedAttrCapacity(size_t requiredBytes)
{
    size_t capacity = ATTR_CAPACITY;
    while (capacity < requiredBytes) {
        capacity *= kAttrGrowthFactor;
    }
    return capacity;
}

// 字符串属性按 sizeof(size_t) 对齐后的占用字节数。
static size_t AlignedStringBytes(size_t length)
{
    const size_t totalBytes = length + 1U;
    return (totalBytes + sizeof(size_t) - 1U) / sizeof(size_t) * sizeof(size_t);
}

// 数组属性占用字节数 = ContinuousVector 头部 + 元素载荷。
static size_t ArrayAttrBytes(size_t elemCount, size_t elemSize)
{
    static const gert::ContinuousVector kEmptyVector{};
    return PtrOffset(&kEmptyVector, kEmptyVector.GetData()) + elemCount * elemSize;
}

// 期望值完全由测试入参推导，不引用被测实现写入的 attr_size_ / attrCapacity_。
// attr_size_ 只覆盖 RuntimeAttrsDef + 偏移表 + 属性数据，尾部输出锚点不计入。
static void ExpectExactAttrCapacity(const KernelContextHolder& holder, size_t attrPayloadBytes, size_t attrNum,
                                    size_t outputNum)
{
    const size_t offsetTableBytes = sizeof(size_t) * attrNum;
    const size_t expectedRequired = offsetTableBytes + attrPayloadBytes + sizeof(AnchorInstanceInfo) * outputNum;
    EXPECT_EQ(holder.computeNodeInfo_->attr_size_, sizeof(RuntimeAttrsDef) + offsetTableBytes + attrPayloadBytes);
    EXPECT_GE(holder.attrCapacity_, expectedRequired);
    EXPECT_EQ(holder.attrCapacity_, ExpectedAttrCapacity(expectedRequired));
}

TEST_F(AttrDynamicCapacityUT, LargeIntArrayTriggersExactExpansion)
{
    KernelContextHolder holder;
    aclTensor* in = MakeTensor();
    aclTensor* out = MakeTensor();
    std::vector<op::OpArg> inputs = {MakeTensorArg(in)};
    std::vector<op::OpArg> outputs = {MakeTensorArg(out)};

    std::vector<int64_t> values(kLargeElemCount);
    std::iota(values.begin(), values.end(), 0);
    aclIntArray* intArray = aclCreateIntArray(values.data(), values.size());
    std::vector<op::OpArg> attrs = {MakeIntArrayArg(intArray)};

    ASSERT_EQ(RunUpdate(holder, inputs, outputs, attrs), ACLNN_SUCCESS);
    ExpectExactAttrCapacity(holder, ArrayAttrBytes(kLargeElemCount, sizeof(int64_t)), 1U, 1U);

    const auto* vector = reinterpret_cast<const gert::ContinuousVector*>(AttrAddr(holder, 0));
    ASSERT_EQ(vector->GetSize(), kLargeElemCount);
    const auto* data = static_cast<const int64_t*>(vector->GetData());
    EXPECT_EQ(data[0], 0);
    EXPECT_EQ(data[kLargeElemCount - 1], static_cast<int64_t>(kLargeElemCount - 1));

    aclDestroyIntArray(intArray);
    aclDestroyTensor(in);
    aclDestroyTensor(out);
}

TEST_F(AttrDynamicCapacityUT, PrepareAttrWriteReservesExactBytes)
{
    KernelContextHolder holder;
    holder.irInputNum_ = 1;
    holder.inputNum_ = 1;
    holder.outputNum_ = 0;
    holder.attrNum_ = 1;
    holder.UpdateCompileDescOffset(1);
    holder.UpdateAttrDefOffset(1, 1);

    // 需求字节数取较大值，使 required > 初始 attrCapacity_，强制走扩容搬迁路径。
    constexpr size_t kSentinelBytes = 128U;
    constexpr size_t kNeedBytes = 40000U;
    auto* attrBase = static_cast<uint8_t*>(holder.attrDataStart_);
    for (size_t i = 0; i < kSentinelBytes; ++i) {
        attrBase[i] = static_cast<uint8_t>(i);
    }

    void* attrPtr = PtrShift(holder.attrDataStart_, kSentinelBytes);
    size_t remainBytes = 0U;
    ASSERT_EQ(holder.PrepareAttrWrite(0, attrPtr, kNeedBytes, &remainBytes), ACLNN_SUCCESS);

    const size_t expectedCapacity = ExpectedAttrCapacity(sizeof(size_t) + kSentinelBytes + kNeedBytes);
    EXPECT_EQ(attrPtr, PtrShift(holder.attrDataStart_, kSentinelBytes));
    EXPECT_EQ(holder.attrDef_->offset[0], sizeof(RuntimeAttrsDef) + sizeof(size_t) + kSentinelBytes);
    EXPECT_EQ(holder.attrCapacity_, expectedCapacity);
    EXPECT_EQ(remainBytes, expectedCapacity - sizeof(size_t) - kSentinelBytes);

    const auto* relocatedBase = static_cast<const uint8_t*>(holder.attrDataStart_);
    for (size_t i = 0; i < kSentinelBytes; ++i) {
        EXPECT_EQ(relocatedBase[i], static_cast<uint8_t>(i));
    }
}

TEST_F(AttrDynamicCapacityUT, MultipleLargeArraysGrowCapacity)
{
    KernelContextHolder holder;
    aclTensor* in = MakeTensor();
    aclTensor* out = MakeTensor();
    std::vector<op::OpArg> inputs = {MakeTensorArg(in)};
    std::vector<op::OpArg> outputs = {MakeTensorArg(out)};

    constexpr size_t kCount = 20000;
    std::vector<aclIntArray*> arrays;
    std::vector<op::OpArg> attrs;
    for (size_t i = 0; i < 2U; ++i) {
        std::vector<int64_t> values(kCount);
        for (size_t k = 0; k < kCount; ++k) {
            values[k] = static_cast<int64_t>(i * kCount + k);
        }
        aclIntArray* array = aclCreateIntArray(values.data(), values.size());
        arrays.push_back(array);
        attrs.push_back(MakeIntArrayArg(array));
    }

    ASSERT_EQ(RunUpdate(holder, inputs, outputs, attrs), ACLNN_SUCCESS);
    ExpectExactAttrCapacity(holder, 2U * ArrayAttrBytes(kCount, sizeof(int64_t)), 2U, 1U);

    for (size_t i = 0; i < 2U; ++i) {
        const auto* vector = reinterpret_cast<const gert::ContinuousVector*>(AttrAddr(holder, i));
        ASSERT_EQ(vector->GetSize(), kCount);
        const auto* data = static_cast<const int64_t*>(vector->GetData());
        EXPECT_EQ(data[0], static_cast<int64_t>(i * kCount));
        EXPECT_EQ(data[kCount - 1], static_cast<int64_t>(i * kCount + kCount - 1U));
    }

    for (auto* array : arrays) {
        aclDestroyIntArray(array);
    }
    aclDestroyTensor(in);
    aclDestroyTensor(out);
}

TEST_F(AttrDynamicCapacityUT, BoundaryElementCounts)
{
    // attrNum=1、1 个输出时，4082 个 int64 恰好用尽初始容量，4083 个触发首次扩容。
    struct BoundaryCase {
        size_t elemCount;
        size_t expectedCapacity;
    };
    const BoundaryCase cases[] = {{4082U, ATTR_CAPACITY}, {4083U, ATTR_CAPACITY * kAttrGrowthFactor}};

    for (const auto& testCase : cases) {
        KernelContextHolder holder;
        aclTensor* in = MakeTensor();
        aclTensor* out = MakeTensor();
        std::vector<op::OpArg> inputs = {MakeTensorArg(in)};
        std::vector<op::OpArg> outputs = {MakeTensorArg(out)};

        std::vector<int64_t> values(testCase.elemCount, 1);
        aclIntArray* array = aclCreateIntArray(values.data(), values.size());
        std::vector<op::OpArg> attrs = {MakeIntArrayArg(array)};

        ASSERT_EQ(RunUpdate(holder, inputs, outputs, attrs), ACLNN_SUCCESS);
        ExpectExactAttrCapacity(holder, ArrayAttrBytes(testCase.elemCount, sizeof(int64_t)), 1U, 1U);
        EXPECT_EQ(holder.attrCapacity_, testCase.expectedCapacity);

        const auto* vector = reinterpret_cast<const gert::ContinuousVector*>(AttrAddr(holder, 0));
        ASSERT_EQ(vector->GetSize(), testCase.elemCount);
        const auto* data = static_cast<const int64_t*>(vector->GetData());
        EXPECT_EQ(data[testCase.elemCount - 1U], 1);

        aclDestroyIntArray(array);
        aclDestroyTensor(in);
        aclDestroyTensor(out);
    }
}

TEST_F(AttrDynamicCapacityUT, LongStringExpandsCapacity)
{
    for (size_t length : {8192U, 1024U * 1024U}) {
        KernelContextHolder holder;
        aclTensor* in = MakeTensor();
        aclTensor* out = MakeTensor();
        std::vector<op::OpArg> inputs = {MakeTensorArg(in)};
        std::vector<op::OpArg> outputs = {MakeTensorArg(out)};

        std::string storage(length, 'a');
        std::vector<op::OpArg> attrs = {MakeStringArg(storage)};

        ASSERT_EQ(RunUpdate(holder, inputs, outputs, attrs), ACLNN_SUCCESS);
        ExpectExactAttrCapacity(holder, AlignedStringBytes(length), 1U, 1U);

        const char* stored = reinterpret_cast<const char*>(AttrAddr(holder, 0));
        EXPECT_EQ(std::string(stored), storage);

        aclDestroyTensor(in);
        aclDestroyTensor(out);
    }
}

TEST_F(AttrDynamicCapacityUT, NullAndEmptyAttrs)
{
    KernelContextHolder holder;
    aclTensor* in = MakeTensor();
    aclTensor* out = MakeTensor();
    std::vector<op::OpArg> inputs = {MakeTensorArg(in)};
    std::vector<op::OpArg> outputs = {MakeTensorArg(out)};

    int64_t dummy = 1;
    aclIntArray* emptyArray = aclCreateIntArray(&dummy, 0);
    std::vector<op::OpArg> attrs = {MakeIntArrayArg(emptyArray), MakeNullStringArg(), MakeScalarArg(nullptr)};

    ASSERT_EQ(RunUpdate(holder, inputs, outputs, attrs), ACLNN_SUCCESS);
    ExpectExactAttrCapacity(holder, sizeof(gert::ContinuousVector) + 2U * sizeof(int64_t), 3U, 1U);

    const auto* vector = reinterpret_cast<const gert::ContinuousVector*>(AttrAddr(holder, 0));
    EXPECT_EQ(vector->GetSize(), 0U);
    EXPECT_EQ(*reinterpret_cast<const char*>(AttrAddr(holder, 1)), '\0');
    EXPECT_EQ(*reinterpret_cast<const int64_t*>(AttrAddr(holder, 2)), 0);
    EXPECT_EQ(holder.attrDef_->offset[1] - holder.attrDef_->offset[0], sizeof(gert::ContinuousVector));
    EXPECT_EQ(holder.attrDef_->offset[2] - holder.attrDef_->offset[1], sizeof(int64_t));

    aclDestroyIntArray(emptyArray);
    aclDestroyTensor(in);
    aclDestroyTensor(out);
}

TEST_F(AttrDynamicCapacityUT, MixedTypesKeepOffsetsAndAnchors)
{
    KernelContextHolder holder;
    aclTensor* in = MakeTensor();
    aclTensor* out = MakeTensor();
    std::vector<op::OpArg> inputs = {MakeTensorArg(in)};
    std::vector<op::OpArg> outputs = {MakeTensorArg(out)};

    int64_t scalarValue = 42;
    aclScalar* scalar = aclCreateScalar(&scalarValue, aclDataType::ACL_INT64);
    std::string storage = "hello";
    int64_t intValues[] = {7, 8, 9};
    aclIntArray* intArray = aclCreateIntArray(intValues, 3);
    float floatValues[] = {1.5F, 2.5F};
    aclFloatArray* floatArray = aclCreateFloatArray(floatValues, 2);
    bool boolValues[] = {true, false};
    aclBoolArray* boolArray = aclCreateBoolArray(boolValues, 2);

    std::vector<op::OpArg> attrs = {MakeScalarArg(scalar), MakeStringArg(storage), MakeIntArrayArg(intArray),
                                    MakeFloatArrayArg(floatArray), MakeBoolArrayArg(boolArray)};

    ASSERT_EQ(RunUpdate(holder, inputs, outputs, attrs), ACLNN_SUCCESS);
    const size_t payloadBytes = sizeof(int64_t) + AlignedStringBytes(storage.size()) +
                                ArrayAttrBytes(3U, sizeof(int64_t)) + ArrayAttrBytes(2U, sizeof(float)) +
                                ArrayAttrBytes(2U, sizeof(bool));
    ExpectExactAttrCapacity(holder, payloadBytes, attrs.size(), 1U);

    EXPECT_EQ(*reinterpret_cast<const int64_t*>(AttrAddr(holder, 0)), scalarValue);
    EXPECT_EQ(std::string(reinterpret_cast<const char*>(AttrAddr(holder, 1))), storage);
    const auto* intVector = reinterpret_cast<const gert::ContinuousVector*>(AttrAddr(holder, 2));
    ASSERT_EQ(intVector->GetSize(), 3U);
    const auto* intData = static_cast<const int64_t*>(intVector->GetData());
    EXPECT_EQ(intData[0], 7);
    EXPECT_EQ(intData[2], 9);
    const auto* floatVector = reinterpret_cast<const gert::ContinuousVector*>(AttrAddr(holder, 3));
    ASSERT_EQ(floatVector->GetSize(), 2U);
    const auto* floatData = static_cast<const float*>(floatVector->GetData());
    EXPECT_FLOAT_EQ(floatData[0], 1.5F);
    EXPECT_FLOAT_EQ(floatData[1], 2.5F);
    const auto* boolVector = reinterpret_cast<const gert::ContinuousVector*>(AttrAddr(holder, 4));
    ASSERT_EQ(boolVector->GetSize(), 2U);
    const auto* boolData = static_cast<const bool*>(boolVector->GetData());
    EXPECT_TRUE(boolData[0]);
    EXPECT_FALSE(boolData[1]);

    EXPECT_EQ(holder.attrDef_->offset[1] - holder.attrDef_->offset[0], sizeof(int64_t));
    EXPECT_EQ(holder.attrDef_->offset[2] - holder.attrDef_->offset[1], AlignedStringBytes(storage.size()));
    EXPECT_EQ(holder.attrDef_->offset[3] - holder.attrDef_->offset[2], ArrayAttrBytes(3U, sizeof(int64_t)));
    EXPECT_EQ(holder.attrDef_->offset[4] - holder.attrDef_->offset[3], ArrayAttrBytes(2U, sizeof(float)));

    aclDestroyScalar(scalar);
    aclDestroyIntArray(intArray);
    aclDestroyFloatArray(floatArray);
    aclDestroyBoolArray(boolArray);
    aclDestroyTensor(in);
    aclDestroyTensor(out);
}

TEST_F(AttrDynamicCapacityUT, OutputAnchorsAreAccounted)
{
    constexpr size_t kOutputCount = 100;
    KernelContextHolder holder;
    aclTensor* in = MakeTensor();
    std::vector<op::OpArg> inputs = {MakeTensorArg(in)};

    std::vector<aclTensor*> outputTensors;
    std::vector<op::OpArg> outputs;
    for (size_t i = 0; i < kOutputCount; ++i) {
        aclTensor* tensor = MakeTensor();
        outputTensors.push_back(tensor);
        outputs.push_back(MakeTensorArg(tensor));
    }

    constexpr size_t kCount = 4000;
    std::vector<int64_t> values(kCount);
    std::iota(values.begin(), values.end(), 0);
    aclIntArray* array = aclCreateIntArray(values.data(), values.size());
    std::vector<op::OpArg> attrs = {MakeIntArrayArg(array)};

    ASSERT_EQ(RunUpdate(holder, inputs, outputs, attrs), ACLNN_SUCCESS);
    const size_t payloadBytes = ArrayAttrBytes(kCount, sizeof(int64_t));
    ExpectExactAttrCapacity(holder, payloadBytes, 1U, kOutputCount);

    // 输出锚点紧跟在属性数据之后，扩容搬迁后必须指向新块。
    auto* expectedAnchorAddr = PtrCastTo<AnchorInstanceInfo>(
        PtrShift(holder.attrDataStart_, static_cast<int64_t>(payloadBytes)));
    EXPECT_EQ(holder.outputAnchorInfo_, expectedAnchorAddr);

    // 搬迁不应破坏已写入的属性数据。
    const auto* vector = reinterpret_cast<const gert::ContinuousVector*>(AttrAddr(holder, 0));
    ASSERT_EQ(vector->GetSize(), kCount);
    const auto* data = static_cast<const int64_t*>(vector->GetData());
    EXPECT_EQ(data[0], 0);
    EXPECT_EQ(data[kCount - 1U], static_cast<int64_t>(kCount - 1U));

    for (size_t i = 0; i < kOutputCount; ++i) {
        EXPECT_EQ(holder.outputAnchorInfo_[i].instance_start_, i);
        EXPECT_EQ(holder.outputAnchorInfo_[i].instantiation_num_, 1U);
    }

    aclDestroyIntArray(array);
    for (auto* tensor : outputTensors) {
        aclDestroyTensor(tensor);
    }
    aclDestroyTensor(in);
}

TEST_F(AttrDynamicCapacityUT, PrepareAttrWriteRejectsInvalidInput)
{
    KernelContextHolder holder;
    holder.irInputNum_ = 1;
    holder.inputNum_ = 1;
    holder.outputNum_ = 0;
    holder.attrNum_ = 1;
    holder.UpdateCompileDescOffset(1);
    holder.UpdateAttrDefOffset(1, 1);

    // 索引必须小于属性个数。
    void* attrPtr = holder.attrDataStart_;
    EXPECT_EQ(holder.PrepareAttrWrite(1, attrPtr, sizeof(int64_t)), ACLNN_ERR_INNER);

    // 需求字节数加偏移表后溢出时必须返回错误，而不是回绕后放行。
    attrPtr = holder.attrDataStart_;
    EXPECT_EQ(holder.PrepareAttrWrite(0, attrPtr, SIZE_MAX), ACLNN_ERR_INNER);
}

TEST_F(AttrDynamicCapacityUT, EnsureAttrCapacityRejectsGrowthOverflow)
{
    KernelContextHolder holder;
    // 需求超过 2 倍增长可表示范围时，必须返回错误，而不是回绕或死循环。
    EXPECT_EQ(holder.EnsureAttrCapacity(SIZE_MAX), ACLNN_ERR_INNER);
}
