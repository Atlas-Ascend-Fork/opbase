# OP\_LOGE\_FOR\_INVALID\_SHAPES\_WITH\_REASON

## Function

Records and reports the shape verification error for multiple parameters with a reason. When the shapes of multiple parameters of an operator are incorrect, outputs ERROR-level logs (including the specific reason) and reports the EZ0010 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_SHAPES_WITH_REASON(entityName, paramNames, incorrectShapes, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramNames | Input| List of parameter names. Supports the const char* or std::string type.|
| incorrectShapes | Input| List of actual shapes. Supports the const char* or std::string type.|
| reason | Input| Error reason description. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameters indices 0th tensor and x 0th tensor of DynamicStitch have incorrect shapes
//          [2,3] and [4,5,6]. Reason: The shape of indices's tensor should match the shape formed by
//          the first 2 axes of x's corresponding tensor.
if (indicesShape != expectedShape) {
    std::string shapeMsg = Ops::Base::ToString(indicesShape) + " and " + Ops::Base::ToString(xShape);
    OP_LOGE_FOR_INVALID_SHAPES_WITH_REASON("DynamicStitch",
        "indices 0th tensor and x 0th tensor", shapeMsg.c_str(),
        "The shape of indices's tensor should match the shape formed by the "
        "first 2 axes of x's corresponding tensor.");
    return ge::GRAPH_FAILED;
}
```
