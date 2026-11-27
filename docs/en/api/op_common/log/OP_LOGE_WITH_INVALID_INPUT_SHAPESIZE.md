# OP\_LOGE\_WITH\_INVALID\_INPUT\_SHAPESIZE

> **Note:** This API is deprecated. It is recommended that you use [OP_LOGE_FOR_INVALID_SHAPESIZE](OP_LOGE_FOR_INVALID_SHAPESIZE.md) instead.

## Function

Records and reports the operator input shape size verification error. When the shape size (total number of elements) of a specified input of an operator does not meet expectations, outputs ERROR-level logs and reports the EZ0005 error code.

## Prototype

```cpp
OP_LOGE_WITH_INVALID_INPUT_SHAPESIZE(entityName, index, incorrectSize, correctSize)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| index | Input| Input tensor index (starting from 0), of the int type.|
| incorrectSize | Input| Actual shape size. Supports the const char* or std::string type.|
| correctSize | Input| Expected shape size. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: The 0th input of MyOp has incorrect shape size 1024. It should be 2048.
if (xShape.GetShapeSize() != 2048) {
    OP_LOGE_WITH_INVALID_INPUT_SHAPESIZE("MyOp", 0,
        std::to_string(xShape.GetShapeSize()), "2048");
    return false;
}
```
