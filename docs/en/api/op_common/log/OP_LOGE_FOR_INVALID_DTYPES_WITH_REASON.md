# OP\_LOGE\_FOR\_INVALID\_DTYPES\_WITH\_REASON

## Function

Records and reports the data type verification error for multiple parameters with a reason. When the data types of multiple parameters of an operator are incorrect, outputs ERROR-level logs (including the specific reason) and reports the EZ0021 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_DTYPES_WITH_REASON(entityName, paramNames, incorrectDtypes, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramNames | Input| List of parameter names. Supports the const char* or std::string type.|
| incorrectDtypes | Input| List of actual data types. Supports the const char* or std::string type.|
| reason | Input| Error reason description. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameters cos and dy of RotaryPositionEmbeddingGrad have incorrect dtypes
//          FLOAT16 and FLOAT32. Reason: The dtypes of input cos and input dy should be the same.
if (cosDtype != dyDtype) {
    std::string dtypeMsg = Ops::Base::ToString(cosDtype) + " and " + Ops::Base::ToString(dyDtype);
    OP_LOGE_FOR_INVALID_DTYPES_WITH_REASON("RotaryPositionEmbeddingGrad", "cos and dy",
        dtypeMsg.c_str(), "The dtypes of input cos and input dy should be the same.");
    return ge::GRAPH_FAILED;
}
```
