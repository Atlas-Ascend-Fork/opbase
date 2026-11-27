# OP\_LOGE\_WITH\_INVALID\_INPUT

> **Note:** This API is deprecated. It is recommended that you use [OP_LOGE_FOR_INVALID_VALUE](OP_LOGE_FOR_INVALID_VALUE.md) or [OP_CHECK_NULL_WITH_CONTEXT](OP_CHECK_NULL_WITH_CONTEXT.md) instead.

## Function

Records and reports the empty input parameter verification error. When a required parameter of an operator is empty, outputs ERROR-level logs and reports the EZ0004 error code.

## Prototype

```cpp
OP_LOGE_WITH_INVALID_INPUT(entityName, paramName)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameter input of MyOp is required, but it is empty.
if (xDesc == nullptr) {
    OP_LOGE_WITH_INVALID_INPUT("MyOp", "input");
    return false;
}
```
