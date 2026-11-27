# OP\_LOGE\_FOR\_INVALID\_VALUES\_WITH\_REASON

## Function

Records and reports the value verification error for multiple parameters with a reason. When the values of multiple parameters of an operator are incorrect, outputs ERROR-level logs (including the specific reason) and reports the EZ0027 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_VALUES_WITH_REASON(entityName, paramNames, incorrectValues, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramNames | Input| List of parameter names. Supports the const char* or std::string type.|
| incorrectValues | Input| List of actual parameter values. Supports the const char* or std::string type.|
| reason | Input| Error reason description. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameters align_corners and half_pixel_centers of ResizeNearestNeighborV2Grad have
//          incorrect values true and true. Reason: The values of attributes align_corners and
//          half_pixel_centers cannot be true at the same time.
if (alignCorners_ && halfPixelCenters_) {
    OP_LOGE_FOR_INVALID_VALUES_WITH_REASON("ResizeNearestNeighborV2Grad",
        "align_corners and half_pixel_centers", "true and true",
        "The values of attributes align_corners and half_pixel_centers cannot be true at the same time.");
    return ge::GRAPH_FAILED;
}
```
