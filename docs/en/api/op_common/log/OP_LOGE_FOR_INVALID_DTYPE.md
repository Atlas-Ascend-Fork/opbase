# OP\_LOGE\_FOR\_INVALID\_DTYPE

## Function

Records and reports the parameter data type verification error. When the data type of a specified parameter of an operator does not meet expectations, outputs ERROR-level logs and reports the EZ0019 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_DTYPE(entityName, paramName, incorrectDtype, correctDtype)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| incorrectDtype | Input| Actual data type (for example, "INT32"). Supports the const char* or std::string type.|
| correctDtype | Input| Expected data type (for example, "FLOAT16"). Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameter softmax of SoftmaxGrad has incorrect dtype INT32. It should be
//          FLOAT, FLOAT16 or BF16.
if (dtype_ != ge::DT_FLOAT && dtype_ != ge::DT_FLOAT16 && dtype_ != ge::DT_BF16) {
    OP_LOGE_FOR_INVALID_DTYPE("SoftmaxGrad", "softmax",
        Ops::Base::ToString(dtype_).c_str(), "FLOAT, FLOAT16 or BF16");
    return ge::GRAPH_FAILED;
}
```
