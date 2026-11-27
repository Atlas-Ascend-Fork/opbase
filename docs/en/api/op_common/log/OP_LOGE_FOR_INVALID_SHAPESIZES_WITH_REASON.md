# OP\_LOGE\_FOR\_INVALID\_SHAPESIZES\_WITH\_REASON

## Function

Records and reports the shape size verification error for multiple parameters with a reason. When the shape sizes of multiple parameters of an operator are incorrect, outputs ERROR-level logs (including the specific reason) and reports the EZ0016 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_SHAPESIZES_WITH_REASON(entityName, paramNames, incorrectSizes, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramNames | Input| List of parameter names. Supports the const char* or std::string type.|
| incorrectSizes | Input| List of actual shape sizes. Supports the const char* or std::string type.|
| reason | Input| Error reason description. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameters query, key, cos and sin of ApplyRotaryPosEmb have incorrect shape sizes
//          0, 0, 0 and 0. Reason: All inputs must be non-empty tensors.
if (qSize == 0 || kSize == 0 || cSize == 0 || sSize == 0) {
    std::string sizeMsg = std::to_string(qSize) + ", " + std::to_string(kSize) + ", " +
        std::to_string(cSize) + " and " + std::to_string(sSize);
    OP_LOGE_FOR_INVALID_SHAPESIZES_WITH_REASON("ApplyRotaryPosEmb",
        "query, key, cos and sin", sizeMsg.c_str(),
        "All inputs must be non-empty tensors.");
    return ge::GRAPH_FAILED;
}
```
