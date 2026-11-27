# OP\_LOGE\_FOR\_INVALID\_VALUE

## Function

Records and reports the parameter value verification error. When the value of a specified parameter of an operator does not meet expectations, outputs ERROR-level logs and reports the EZ0024 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_VALUE(entityName, paramName, incorrectValue, correctValue)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| incorrectValue | Input| Actual parameter value. Supports the const char* or std::string type.|
| correctValue | Input| Expected parameter value. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameter sp of AttentionUpdate has incorrect value 17. It should be
//          in range of [1, 16].
if (sp_ < 1 || sp_ > 16) {
    OP_LOGE_FOR_INVALID_VALUE("AttentionUpdate", "sp", std::to_string(sp_), "in range of [1, 16]");
    return ge::GRAPH_FAILED;
}
```
