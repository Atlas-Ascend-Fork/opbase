# Cast

## Function

Converts the input tensor to the specified data type.

## Prototype

```cpp
const aclTensor *Cast(const aclTensor *self, op::DataType dstDtype, aclOpExecutor *executor)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| self | Input| Input tensor to be converted. The supported data types are FLOAT16, FLOAT, DOUBLE, BFLOAT16, INT8, UINT8, INT16, UINT16, INT32, UINT32, INT64, UINT64, BOOL, COMPLEX64, and COMPLEX128. The supported data format is ND. |
| dstDtype | Input| Target data type after conversion. The supported data types are FLOAT16, FLOAT, DOUBLE, BFLOAT16, INT8, UINT8, INT16, UINT16, INT32, UINT32, INT64, UINT64, BOOL, COMPLEX64, and COMPLEX128. |
| executor | Input| Operator executor, which contains the operator execution flow. |

> **NOTE**: BFLOAT16 is only supported on the following products:
>
> - Atlas A2 products
> - Atlas A3 products

## Returns

A tensor of the `dstDtype` data type.

## Restrictions

None

## Examples

```cpp
// Standard usage: create an OpExecutor
auto uniqueExecutor = CREATE_EXECUTOR();

auto selfCasted = self;
// If self is of the Boolean type, use Cast to convert it to the uint8 type so that integer computation can be performed
if (self->GetDataType() == op::DataType::DT_BOOL) {
  selfCasted = l0op::Cast(self, op::DataType::DT_UINT8, uniqueExecutor.get());
  CHECK_RET(selfCasted != nullptr, ACLNN_ERR_PARAM_NULLPTR);
}
```
