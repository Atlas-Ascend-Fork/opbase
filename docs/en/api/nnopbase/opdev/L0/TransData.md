# TransData

## Function

This API does not change the value of the tensor data but converts the tensor data format.

Specifically, it converts the format of the user input tensor to the specified dstPrimaryFormat.

## Prototype

```cpp
const aclTensor *TransData(const aclTensor *x, op::Format dstPrimaryFormat, int64_t groups, aclOpExecutor *executor)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| x | Input| Tensor to be converted. The supported data types are FLOAT16, FLOAT32, INT32, UINT32, INT8, and UINT8. |
| dstPrimaryFormat | Input| Target format to which the input tensor is converted. |
| groups | Input| Grouping parameter, which is passed in for grouped conversion. The supported data type is INT64. |
| executor | Input| Operator executor, which contains the operator execution flow. |

## Returns

A tensor whose data format is dstPrimaryFormat.

## Restrictions

When the input tensor data type is FLOAT32, INT32, or UINT32, C0 can only be processed as 8.

## Examples

```cpp
// Convert the tensor format from NC1HWC0 to NCHW
auto transGradInput = l0op::TransData(gradInputNC1HWC0, op::Format::FORMAT_NCHW, params.groups, executor);
CHECK_RET(transGradInput != nullptr, ACLNN_ERR_INNER_NULLPTR);
```
