# OP\_LOGE\_FOR\_INVALID\_TENSORNUMS\_WITH\_REASON

## Function

Records and reports the tensor quantity verification error for multiple parameters with a reason. When the tensor quantities of multiple parameters of an operator are incorrect, outputs ERROR-level logs (including the specific reason) and reports the EZ0023 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_TENSORNUMS_WITH_REASON(entityName, paramNames, incorrectNums, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramNames | Input| List of parameter names. Supports the const char* or std::string type.|
| incorrectNums | Input| List of actual tensor quantities. Supports the const char* or std::string type.|
| reason | Input| Error reason description. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameters lse and go of AttentionUpdate have invalid tensor nums 4 and 4.
//          Reason: The number of tensors in input lse and go should be twice the attr sp, where sp is 2.
if (lseCount != sp * 2 || goCount != sp * 2) {
    std::string numMsg = std::to_string(lseCount) + " and " + std::to_string(goCount);
    OP_LOGE_FOR_INVALID_TENSORNUMS_WITH_REASON("AttentionUpdate", "lse and go",
        numMsg.c_str(),
        "The number of tensors in input lse and go should be twice the attr sp, where sp is 2.");
    return ge::GRAPH_FAILED;
}
```
