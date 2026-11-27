# OP\_LOGE\_FOR\_INVALID\_SHAPEDIM\_WITH\_REASON

## Function

Records and reports the parameter shape dimension verification error with a reason. When the shape dimension of a specified parameter of an operator is incorrect, outputs ERROR-level logs (including the specific reason) and reports the EZ0012 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_SHAPEDIM_WITH_REASON(entityName, paramName, incorrectDim, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| incorrectDim | Input| Actual dimension (for example, "2D"). Supports the const char* or std::string type.|
| reason | Input| Error reason description. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameter query of ApplyRotaryPosEmb has incorrect shape dim 3D.
//          Reason: The shape dims of input query must be 4 when the attr layout is 1 (BSND).
if (queryDimNum != 4 && layout_ == 1) {
    std::string dimStr = std::to_string(queryDimNum) + "D";
    OP_LOGE_FOR_INVALID_SHAPEDIM_WITH_REASON("ApplyRotaryPosEmb", "query",
        dimStr.c_str(),
        "The shape dims of input query must be 4 when the attr layout is 1 (BSND).");
    return ge::GRAPH_FAILED;
}
```
