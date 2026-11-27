# Cast

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

<!-- npu="A3,910b" id7 -->
> **NOTE**: BFLOAT16 is only supported on the following products:
>
> - Atlas A2 products
> - Atlas A3 products
<!-- end id7 -->

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
