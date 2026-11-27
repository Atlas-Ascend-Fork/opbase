# OP\_LOGE\_FOR\_INVALID\_SHAPEDIMS\_WITH\_REASON

## Function

Records and reports the shape dimension verification error for multiple parameters with a reason. When the shape dimensions of multiple parameters of an operator are incorrect, outputs ERROR-level logs (including the specific reason) and reports the EZ0013 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_SHAPEDIMS_WITH_REASON(entityName, paramNames, incorrectDims, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramNames | Input| List of parameter names. Supports the const char* or std::string type.|
| incorrectDims | Input| List of actual dimensions. Supports the const char* or std::string type.|
| reason | Input| Error reason description. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameters dy, cos and sin of RotaryPositionEmbeddingGrad have incorrect shape dims
//          3, 4 and 4. Reason: The numbers of dimensions of input dy, cos and sin should all be 3D or 4D.
if (dyDimNum != expectedDim || cosDimNum != expectedDim || sinDimNum != expectedDim) {
    std::string dimMsg = std::to_string(dyDimNum) + ", " + std::to_string(cosDimNum) +
        " and " + std::to_string(sinDimNum);
    OP_LOGE_FOR_INVALID_SHAPEDIMS_WITH_REASON("RotaryPositionEmbeddingGrad",
        "dy, cos and sin", dimMsg.c_str(),
        "The numbers of dimensions of input dy, cos and sin should all be 3D or 4D.");
    return ge::GRAPH_FAILED;
}
```
