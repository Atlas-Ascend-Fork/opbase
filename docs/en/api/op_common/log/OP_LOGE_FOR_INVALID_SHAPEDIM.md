# OP\_LOGE\_FOR\_INVALID\_SHAPEDIM

## Function

Records and reports the parameter shape dimension verification error. When the shape dimension of a specified parameter of an operator does not meet expectations, outputs ERROR-level logs and reports the EZ0011 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_SHAPEDIM(entityName, paramName, incorrectDim, correctDim)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| incorrectDim | Input| Actual dimension (for example, "3D"). Supports the const char* or std::string type.|
| correctDim | Input| Expected dimension (for example, "4D"). Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameter x of SoftmaxV2 has incorrect shape dim 5D. It should be less than
//          or equal to 4.
if (xShape.GetDimNum() > 4) {
    std::string dimStr = std::to_string(xShape.GetDimNum()) + "D";
    OP_LOGE_FOR_INVALID_SHAPEDIM("SoftmaxV2", "x", dimStr.c_str(),
        "less than or equal to 4");
    return ge::GRAPH_FAILED;
}
```
