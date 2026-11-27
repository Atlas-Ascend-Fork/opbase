# OP\_LOGE\_FOR\_INVALID\_DTYPE\_WITH\_REASON

## Function

Records and reports the parameter data type verification error with a reason. When the data type of a specified parameter of an operator is incorrect, outputs ERROR-level logs (including the specific reason) and reports the EZ0020 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_DTYPE_WITH_REASON(entityName, paramName, incorrectDtype, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| incorrectDtype | Input| Actual data type (for example, "INT8"). Supports the const char* or std::string type.|
| reason | Input| Error reason description. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameter k_cache of KvRmsNormRopeCache has incorrect dtype FLOAT16.
//          Reason: The dtype of input k_cache should be INT8, HIFLOAT8, FLOAT8_E4M3FN or FLOAT8_E5M2,
//          or the same as the dtype of input kv.
if (!IsSupportedDtype(kCacheDtype, kvDtype)) {
    OP_LOGE_FOR_INVALID_DTYPE_WITH_REASON("KvRmsNormRopeCache", "k_cache",
        Ops::Base::ToString(kCacheDtype).c_str(),
        "The dtype of input k_cache should be INT8, HIFLOAT8, FLOAT8_E4M3FN or FLOAT8_E5M2, "
        "or the same as the dtype of input kv.");
    return ge::GRAPH_FAILED;
}
```
