# OP\_LOGE\_FOR\_INVALID\_STRIDE

## Function

Records and reports the parameter stride verification error. When the stride of a specified parameter of an operator does not meet expectations, outputs ERROR-level logs and reports the EZ0028 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_STRIDE(entityName, paramName, incorrectStride, correctStride)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| incorrectStride | Input| Actual stride, for example, "[1,1,1,1]". Supports the const char* or std::string type.|
| correctStride | Input| Expected stride, for example, "[1,1,1,1]". Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
if (!CheckStrideValid(inputStride, expectedStride)) {
    OP_LOGE_FOR_INVALID_STRIDE("MyOp", "x", StrideToStr(inputStride).c_str(), "[1,1,1,1]");
    return ge::GRAPH_FAILED;
}
```
