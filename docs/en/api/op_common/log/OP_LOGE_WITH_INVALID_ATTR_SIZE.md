# OP\_LOGE\_WITH\_INVALID\_ATTR\_SIZE

> **Note:** This API is deprecated. It is recommended that you use [OP_LOGE_FOR_INVALID_LISTSIZE](OP_LOGE_FOR_INVALID_LISTSIZE.md) instead.

## Function

Records and reports the operator attribute size verification error. When the size of a specified attribute of an operator does not meet expectations, outputs ERROR-level logs and reports the EZ0003 error code.

## Prototype

```cpp
OP_LOGE_WITH_INVALID_ATTR_SIZE(entityName, attrName, incorrectSize, correctSize)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| attrName | Input| Attribute name. Supports the const char* or std::string type.|
| incorrectSize | Input| Actual attribute size. Supports the const char* or std::string type.|
| correctSize | Input| Expected attribute size. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Attribute strides of MyOp has incorrect size 3. It should be 4.
if (strides.size() != 4) {
    OP_LOGE_WITH_INVALID_ATTR_SIZE("MyOp", "strides", std::to_string(strides.size()), "4");
    return false;
}
```
