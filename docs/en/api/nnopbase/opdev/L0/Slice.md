# Slice

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Not supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference products: Supported
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas training products: Supported
<!-- end id6 -->

## Function

Extracts the required slices from the input tensor.

## Prototype

```cpp
const aclTensor *Slice(const aclTensor *x, const aclTensor *y, const aclTensor *offset, const aclTensor *size, aclOpExecutor *executor)
```

```cpp
const aclTensor *Slice(const aclTensor *x, const aclIntArray *offsets, const aclIntArray *size, aclOpExecutor *executor)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| x | Input| Input tensor. The supported data types are FLOAT16, FLOAT, BOOL, INT8, UINT8, INT16, UINT16, INT32, UINT32, INT64, BFLOAT16, and UINT64. The supported data format is ND. |
| y | Output| Output tensor after slicing. The supported data types are FLOAT16, FLOAT, BOOL, INT8, UINT8, INT16, UINT16, INT32, UINT32, INT64, BFLOAT16, and UINT64. The supported data format is ND. |
| offsets | Input| Of the const aclIntArray* type, indicating the start position of the slice in each dimension of the input x. The shape is the dimensions of x. The supported data types are INT32 and INT64. The supported data format is ND. |
| offset | Input| Of the const aclTensor* type, indicating the start position of the slice in each dimension of the input x. The shape is the dimensions of x. The supported data types are INT32 and INT64. The supported data format is ND. |
| size | Input| Size of the slice in each dimension of the input x. The shape is the dimensions of x. The supported types are aclIntArray* and aclTensor*. The supported data types are INT32 and INT64. The supported data format is ND. |
| executor | Input| Operator executor, which contains the operator execution flow. |

> **NOTE**: BFLOAT16 is only supported on the following products:
>
> - Atlas A2 products
> - Atlas A3 products

## Returns

A tensor whose data type is the same as that of the input tensor and whose shape is size.

## Restrictions

None

## Examples

```cpp
// Call l0op::Slice to process each block
auto sliceRes = l0op::Slice(self, offsetArray, sizeArray, executor);
// Call l0op::Slice to process each block
auto sliceRes = l0op::Slice(xTensor, yTensor, offsetTensor, sizeTensor, executor);
```
