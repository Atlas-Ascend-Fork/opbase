# OP\_LOGE\_FOR\_INVALID\_VALUE\_WITH\_REASON

## Function

Records and reports the parameter value verification error with a reason. When the value of a specified parameter of an operator is incorrect, outputs ERROR-level logs (including the specific reason) and reports the EZ0026 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_VALUE_WITH_REASON(entityName, paramName, incorrectValue, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| incorrectValue | Input| Actual parameter value. Supports the const char* or std::string type.|
| reason | Input| Error reason description. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameter output_size of UpsampleNearest3D has incorrect value
//          (2147483648, 512, 512). Reason: Each value of output_size must be less than or equal to
//          INT32_MAX.
if (outputSizeVal > INT32_MAX) {
    OP_LOGE_FOR_INVALID_VALUE_WITH_REASON("UpsampleNearest3D", "output_size",
        outputSizeStr.c_str(),
        "Each value of output_size must be less than or equal to INT32_MAX.");
    return ge::GRAPH_FAILED;
}
```
