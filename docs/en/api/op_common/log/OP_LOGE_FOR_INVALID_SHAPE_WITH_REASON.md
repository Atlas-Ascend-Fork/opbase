# OP\_LOGE\_FOR\_INVALID\_SHAPE\_WITH\_REASON

## Function

Records and reports the parameter shape verification error with a reason. When the shape of a specified parameter of an operator is incorrect, outputs ERROR-level logs (including the specific reason) and reports the EZ0009 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_SHAPE_WITH_REASON(entityName, paramName, incorrectShape, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| incorrectShape | Input| Actual shape, for example, "[1,128]". Supports the const char* or std::string type.|
| reason | Input| Error reason description. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameter indices 0th tensor of DynamicStitch has incorrect shape [2,-1,128].
//          Reason: The input indices's tensor has negative dimension.
if (hasNegativeDim) {
    OP_LOGE_FOR_INVALID_SHAPE_WITH_REASON("DynamicStitch",
        "indices 0th tensor", "[2,-1,128]",
        "The input indices's tensor has negative dimension.");
    return ge::GRAPH_FAILED;
}
```
