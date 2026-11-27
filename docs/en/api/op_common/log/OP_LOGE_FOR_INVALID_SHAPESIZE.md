# OP\_LOGE\_FOR\_INVALID\_SHAPESIZE

## Function

Records and reports the parameter shape size verification error. When the shape size (total number of elements) of a specified parameter of an operator does not meet expectations, outputs ERROR-level logs and reports the EZ0014 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_SHAPESIZE(entityName, paramName, incorrectSize, correctSize)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| incorrectSize | Input| Actual shape size (total number of elements). Supports the const char* or std::string type.|
| correctSize | Input| Expected shape size (total number of elements). Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameter group_index of SwigluMxQuantWithDualAxis has incorrect shape size 0.
//          It should be > 0.
if (groupIndexShape.GetShapeSize() == 0) {
    OP_LOGE_FOR_INVALID_SHAPESIZE("SwigluMxQuantWithDualAxis", "group_index",
        std::to_string(groupIndexShape.GetShapeSize()), "> 0");
    return ge::GRAPH_FAILED;
}
```
