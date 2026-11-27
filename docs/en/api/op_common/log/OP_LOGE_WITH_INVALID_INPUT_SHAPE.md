# OP\_LOGE\_WITH\_INVALID\_INPUT\_SHAPE

> **Note:** This API is deprecated. It is recommended that you use [OP_LOGE_FOR_INVALID_SHAPE](OP_LOGE_FOR_INVALID_SHAPE.md) instead.

## Function

Records and reports the operator input shape verification error. When the shape of a specified input tensor of an operator does not meet expectations, outputs ERROR-level logs and reports the EZ0001 error code.

## Prototype

```cpp
OP_LOGE_WITH_INVALID_INPUT_SHAPE(entityName, index, incorrectShape, correctShape)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| index | Input| Input tensor index (starting from 0), of the int type.|
| incorrectShape | Input| Actual input shape, for example, "[1,128,128]". Supports the const char* or std::string type.|
| correctShape | Input| Expected input shape, for example, "[1,256,256]". Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: The 0th input of MatMul has incorrect shape [128,128]. It should be [256,512].
const gert::Shape expectedShape({256, 512});
if (inputShape != expectedShape) {
    std::string incorrectShape = Ops::Base::ToString(inputShape);
    OP_LOGE_WITH_INVALID_INPUT_SHAPE("MatMul", 0, incorrectShape.c_str(), "[256,512]");
    return ge::GRAPH_FAILED;
}
```
