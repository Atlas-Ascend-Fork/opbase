# OP\_LOGE\_WITH\_INVALID\_ATTR

> **Note:** This API is deprecated. It is recommended that you use [OP_LOGE_FOR_INVALID_VALUE](OP_LOGE_FOR_INVALID_VALUE.md) instead.

## Function

Records and reports the operator attribute value verification error. When the value of a specified attribute of an operator does not meet expectations, outputs ERROR-level logs and reports the EZ0002 error code.

## Prototype

```cpp
OP_LOGE_WITH_INVALID_ATTR(entityName, attrName, incorrectVal, correctVal)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| attrName | Input| Attribute name. Supports the const char* or std::string type.|
| incorrectVal | Input| Actual attribute value. Supports the const char* or std::string type.|
| correctVal | Input| Expected attribute value. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Attribute axis of MyOp has incorrect value 3. It should be 0 or 1.
if (axis_ != "0" && axis_ != "1") {
    OP_LOGE_WITH_INVALID_ATTR("MyOp", "axis", std::to_string(axis_), "0 or 1");
    return false;
}
```
