# OP\_LOGE\_FOR\_INVALID\_SHAPE

## Function

Records and reports the parameter shape verification error. When the shape of a specified parameter of an operator does not meet expectations, outputs ERROR-level logs and reports the EZ0008 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_SHAPE(entityName, paramName, incorrectShape, correctShape)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| incorrectShape | Input| Actual shape, for example, "[1,128,128]". Supports the const char* or std::string type.|
| correctShape | Input| Expected shape, for example, "[1,256,256]". Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameter y of Ger has incorrect shape [128]. It should be [32,64].
if (yShape.GetDimNum() != 2) {
    std::string incorrectShape = Ops::Base::ToString(yShape);
    OP_LOGE_FOR_INVALID_SHAPE("Ger", "y", incorrectShape.c_str(), "[32,64]");
    return ge::GRAPH_FAILED;
}
```
