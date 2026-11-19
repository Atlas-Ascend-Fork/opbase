/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file reduce_tensor_move.h
 * \brief reduce tensor move kernel
 */

#ifndef REDUCE_TENSOR_MOVE_H
#define REDUCE_TENSOR_MOVE_H

namespace Ops {
namespace Base {
namespace ReduceOpTmpl {
template <typename T>
class ReduceTensorMove {
public:
    __aicore__ inline ReduceTensorMove(const ReduceOpTilingDataCommon* tilingData, GlobalTensor<uint8_t>* input,
                                       GlobalTensor<uint8_t>* output)
    {
        tiling_ = tilingData;
        input_ = input;
        output_ = output;
        constexpr uint32_t UB_SIZE = Ops::Base::GetUbBlockSize();
        bufferSize_ = static_cast<uint32_t>(tiling_->basicBlock);
        bufferSize_ = (bufferSize_ + UB_SIZE - 1) / UB_SIZE * UB_SIZE;
    };

    __aicore__ inline void Process();

private:
    __aicore__ inline void ProcessPerCore();

private:
    GlobalTensor<uint8_t>* input_;
    GlobalTensor<uint8_t>* output_;
    constexpr static int32_t BUFFER_NUM = 2;

    const ReduceOpTilingDataCommon* tiling_;
    uint32_t bufferSize_ = 0;
    int64_t tailLoopLen_ = 0;
    int64_t addrOffset_ = 0;

    uint64_t loopStartIdx_ = 0;
    uint64_t loopEndIdx_ = 0;

    DataCopyPadExtParams<T> padParams_{false, 0, 0, 0};
    DataCopyExtParams copyOutParams_{1, 0, 0, 0, 0};
};

template <typename T>
__aicore__ inline void ReduceTensorMove<T>::Process()
{
    int64_t blockIdx = GetBlockIdx();
    loopStartIdx_ = blockIdx * tiling_->factorACntPerCore;
    loopEndIdx_ = loopStartIdx_ + tiling_->factorACntPerCore;
    if (unlikely(loopEndIdx_ > tiling_->factorATotalCnt)) {
        loopEndIdx_ = tiling_->factorATotalCnt;
    }

    tailLoopLen_ = tiling_->outSize % tiling_->ubFactorA;
    ProcessPerCore();
}

template <typename T>
__aicore__ inline void ReduceTensorMove<T>::ProcessPerCore()
{
    GlobalTensor<T> inputGM;
    inputGM.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(input_[0].GetPhyAddr(0)));
    GlobalTensor<T> outputGM;
    outputGM.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(output_[0].GetPhyAddr(0)));
    int64_t copyElementNum = tiling_->ubFactorA;
    int64_t ping = 0;
    SetFlag<HardEvent::MTE3_MTE2>(EVENT_ID0);
    SetFlag<HardEvent::MTE3_MTE2>(EVENT_ID1);
    for (int64_t loopIdx = loopStartIdx_; loopIdx < loopEndIdx_; loopIdx++) {
        if (loopIdx == tiling_->factorATotalCnt - 1 && tailLoopLen_ != 0) {
            copyElementNum = tailLoopLen_;
        }
        event_t evt = (ping == 0) ? EVENT_ID0 : EVENT_ID1;
        uint32_t offset = ping * bufferSize_;
        LocalTensor<T> bindLocalIn(TPosition::VECIN, offset, bufferSize_ / sizeof(T));
        LocalTensor<T> bindLocalOut(TPosition::VECOUT, offset, bufferSize_ / sizeof(T));

        WaitFlag<HardEvent::MTE3_MTE2>(evt);
        copyOutParams_.blockLen = copyElementNum * sizeof(T);
        DataCopyPad(bindLocalIn, inputGM[loopIdx * tiling_->ubFactorA], copyOutParams_, padParams_);
        SetFlag<HardEvent::MTE2_MTE3>(evt);
        WaitFlag<HardEvent::MTE2_MTE3>(evt);

        DataCopyPad(outputGM[loopIdx * tiling_->ubFactorA], bindLocalOut, copyOutParams_);
        SetFlag<HardEvent::MTE3_MTE2>(evt);
        ping ^= 1;
    }
    WaitFlag<HardEvent::MTE3_MTE2>(EVENT_ID0);
    WaitFlag<HardEvent::MTE3_MTE2>(EVENT_ID1);
}

} // namespace ReduceOpTmpl
} // namespace Base
} // namespace Ops
#endif // REDUCE_TENSOR_MOVE_H
