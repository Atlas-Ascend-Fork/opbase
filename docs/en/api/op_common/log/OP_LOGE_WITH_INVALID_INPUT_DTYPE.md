# OP\_LOGE\_WITH\_INVALID\_INPUT\_DTYPE

> **Note:** This API is deprecated. It is recommended that you use [OP_LOGE_FOR_INVALID_DTYPE](OP_LOGE_FOR_INVALID_DTYPE.md) instead.

## Function

Records and reports the operator input data type verification error. When the data type of a specified input parameter of an operator does not meet expectations, outputs ERROR-level logs and reports the EZ0007 error code.

## Prototype

```cpp
OP_LOGE_WITH_INVALID_INPUT_DTYPE(entityName, paramName, dataDtype, expectedDtypeList)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Input parameter name. Supports the const char* or std::string type.|
| dataDtype | Input| Actual data type (for example, "INT32"). Supports the const char* or std::string type.|
| expectedDtypeList | Input| List of expected data types (for example, "FLOAT16, FLOAT"). Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Input parameter x of SoftmaxV2 has incorrect dtype INT32. It should be FLOAT, FLOAT16 or BF16.
if (dtype_ != FLOAT && dtype_ != FLOAT16 && dtype_ != BF16) {
    OP_LOGE_WITH_INVALID_INPUT_DTYPE("SoftmaxV2", "x",
        Ops::Base::ToString(dtype_).c_str(), "FLOAT, FLOAT16 or BF16");
    return ge::GRAPH_FAILED;
}
```
