# TransDataSpecial

## Function

This API does not change the value of the tensor data but converts the tensor data format. It is similar to [TransData](TransData.md).

Specifically, it converts the format of the user input tensor to the specified dstPrimaryFormat.

## Prototype

```cpp
const aclTensor *TransDataSpecial(const aclTensor *x, op::Format dstPrimaryFormat, int64_t groups, aclOpExecutor *executor)
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

When the input tensor data type is FLOAT32, INT32, or UINT32, C0 can only be processed as 16.

## Examples

```cpp
// Standard usage: create an OpExecutor
auto uniqueExecutor = CREATE_EXECUTOR();
// Convert the format of gradOutputReFormat to NC1HWC0
auto gradOutputTransData = l0op::TransDataSpecial(gradOutputReFormat, op::Format::FORMAT_NC1HWC0, 0,uniqueExecutor.get());
```
