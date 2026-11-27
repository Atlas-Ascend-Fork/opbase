# OP\_LOGE\_FOR\_INVALID\_FORMATS\_WITH\_REASON

## Function

Records and reports the format verification error for multiple parameters with a reason. When the formats of multiple parameters of an operator are incorrect, outputs ERROR-level logs (including the specific reason) and reports the EZ0018 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_FORMATS_WITH_REASON(entityName, paramNames, incorrectFormats, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramNames | Input| List of parameter names. Supports the const char* or std::string type.|
| incorrectFormats | Input| List of actual formats. Supports the const char* or std::string type.|
| reason | Input| Error reason description. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameters x and y of OperatorName have incorrect formats ND and NCHW.
//          Reason: The formats of all inputs must match.
if (xFormat != yFormat) {
    std::string formatMsg = Ops::Base::ToString(xFormat) + " and " + Ops::Base::ToString(yFormat);
    OP_LOGE_FOR_INVALID_FORMATS_WITH_REASON("OperatorName", "x and y",
        formatMsg.c_str(), "The formats of all inputs must match.");
    return ge::GRAPH_FAILED;
}
```
