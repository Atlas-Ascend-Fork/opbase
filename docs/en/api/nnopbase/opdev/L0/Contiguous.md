# Contiguous

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
- Atlas 200I/500 A2 inference products: Supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference products: Supported
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas training products: Supported
<!-- end id6 -->

## Function

This is a common L0 API that converts a non-contiguous tensor to a contiguous tensor.

Since the input tensor of an L2 API may be non-contiguous while an L0 operator generally accepts only contiguous tensors as input, the tensor needs to be converted to a contiguous one before being used as the input of another L0 API.

The input tensor may already be contiguous; the API handles this case internally.

## Prototype

```cpp
const aclTensor *Contiguous(const aclTensor *x, aclOpExecutor *executor)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| x | Input| Input tensor to be converted. Data type and data format are not limited. The input does not need to be stored contiguously, but the data it represents must fall within the storage range. |
| executor | Input| Operator executor, which contains the operator execution flow. |

## Returns

A contiguous `aclTensor` if the conversion succeeds; `nullptr` otherwise.

## Restrictions

The input tensor must be valid: the data represented by the shape and stride must fall within the storage size. For example, if the shape is \(2, 3\), the stride is \(10, 30\), and the storage size is 8, the data actually exceeds the storage size of 8. Such a tensor is invalid, and Contiguous returns `nullptr`.

## Examples

```cpp
// Standard usage: create an OpExecutor
auto uniqueExecutor = CREATE_EXECUTOR();
// Convert self if it is non-contiguous
auto selfContiguous = l0op::Contiguous(self, executor);
```
