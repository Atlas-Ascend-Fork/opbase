# ReFormat

## Function

This API does not change the value of the tensor data. When the specified format has the same dimensions as the input x, it sets the input data format to the target format.

Specifically, it unifies the viewFormat, originalFormat, and storageFormat of the user input tensor into the specified format.

## Prototype

```cpp
const aclTensor *ReFormat(const aclTensor *x, const op::Format &format, aclOpExecutor *executor=nullptr)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| x | Input| Tensor to be converted. The supported data types are FLOAT16, FLOAT32, INT32, UINT32, INT8, and UINT8. |
| format | Input| Target format to which the input tensor is converted. |
| executor | Input| Operator executor, which contains the operator execution flow. |

## Returns

A tensor whose data format is the target format.

## Restrictions

The dimensions of the input tensor data must be the same as those of the specified format.

## Examples

```cpp
// Reformat the input to the NCHW format
auto reformatInput = l0op::ReFormat(unsqueezedInput, op::Format::FORMAT_NCHW);
CHECK_RET(reformatInput != nullptr, nullptr);
```
