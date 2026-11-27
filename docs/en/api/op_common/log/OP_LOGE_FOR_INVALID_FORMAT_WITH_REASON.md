# OP\_LOGE\_FOR\_INVALID\_FORMAT\_WITH\_REASON

## Function

Records and reports the parameter format verification error with reason. When the format of a specified parameter of an operator is invalid, outputs ERROR-level logs and reports the EZ0035 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_FORMAT_WITH_REASON(entityName, paramName, incorrectFormat, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| incorrectFormat | Input| Actual format, for example, "NHWC". Supports the const char* or std::string type.|
| reason | Input| Error reason. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
if (!IsSupportedFormat(format)) {
    OP_LOGE_FOR_INVALID_FORMAT_WITH_REASON("MyOp", "x", "NDHWC", "only support NCHW and NHWC");
    return ge::GRAPH_FAILED;
}
```
