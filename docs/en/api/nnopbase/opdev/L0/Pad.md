# Pad

## Function

Pads each dimension of the input tensor by the size specified in paddings, with the padding value 0.

## Prototype

```cpp
const aclTensor* Pad(const aclTensor* self, const aclTensor* paddings, aclOpExecutor* executor)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| self | Input| Input tensor to be padded. The supported data types are FLOAT16, FLOAT, INT16, UINT16, INT32, INT64, BFLOAT16, and INT8. The supported data format is ND. |
| paddings | Input| Padding size of each dimension of the input tensor. The shape is [self.dim, 2]. The supported data types are INT32 and INT64. The supported data format is ND. |
| executor | Input| Operator executor, which contains the operator execution flow. |

> **NOTE**: BFLOAT16 and INT8 are only supported on the following products:
>
> - Atlas A2 products
> - Atlas A3 products

## Returns

A tensor padded with 0.

## Restrictions

None

## Examples

```cpp
// Call l0op::Pad to pad self
l0op::Pad(self, paddings, executor);
```
