# Pad

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：不支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品：支持
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas训练系列产品：支持
<!-- end id6 -->

## 功能说明

将输入tensor按照paddings的大小对各个维度进行填充，填充值为0。

## 函数原型

```cpp
const aclTensor* Pad(const aclTensor* self, const aclTensor* paddings, aclOpExecutor* executor)
```

## 参数说明

| 参数 | 输入/输出 | 说明 |
| --- | --- | --- |
| self | 输入 | 待填充的输入tensor，数据类型支持FLOAT16、FLOAT、INT16、UINT16、INT32、INT64、BFLOAT16、INT8。数据格式支持ND。 |
| paddings | 输入 | 输入tensor每个维度被填充的大小，形状为[self.dim, 2]，数据类型支持INT32、INT64。数据格式支持ND。 |
| executor | 输入 | op执行器，包含了算子计算流程。 |

<!-- npu="A3,910b" id7 -->
> **说明**：BFLOAT16和INT8仅适用于如下产品
>
> - Atlas A2系列产品
> - Atlas A3系列产品
<!-- end id7 -->

## 返回值说明

返回被填充了0的tensor。

## 约束说明

无

## 调用示例

```cpp
//调用l0op::Pad对self进行补维
l0op::Pad(self, paddings, executor);
```
