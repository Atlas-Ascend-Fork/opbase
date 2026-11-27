# OP\_LOGE\_FOR\_INVALID\_SHAPESIZE\_WITH\_REASON

## Function

Records and reports the parameter shape size verification error with a reason. When the shape size of a specified parameter of an operator is incorrect, outputs ERROR-level logs (including the specific reason) and reports the EZ0015 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_SHAPESIZE_WITH_REASON(entityName, paramName, incorrectSize, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| incorrectSize | Input| Actual shape size (total number of elements). Supports the const char* or std::string type.|
| reason | Input| Error reason description. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameter y of ResizeLinear has incorrect shape size [144,144,1].
//          Reason: The linear-dimension of output y must be equal to value (256) of input parameter size.
if (yShape.GetShapeSize() != expectedSize) {
    OP_LOGE_FOR_INVALID_SHAPESIZE_WITH_REASON("ResizeLinear", "y",
        Ops::Base::ToString(yShape),
        "The linear-dimension of output y must be equal to value (256) of input parameter size.");
    return ge::GRAPH_FAILED;
}
```
