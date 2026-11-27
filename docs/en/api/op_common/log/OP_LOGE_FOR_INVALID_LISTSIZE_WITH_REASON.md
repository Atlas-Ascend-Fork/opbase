# OP\_LOGE\_FOR\_INVALID\_LISTSIZE\_WITH\_REASON

## Function

Records and reports the parameter list size verification error with reason. When the list size of a specified parameter of an operator is invalid, outputs ERROR-level logs and reports the EZ0038 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_LISTSIZE_WITH_REASON(entityName, paramName, incorrectSize, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| incorrectSize | Input| Actual list size. Supports the const char* or std::string type.|
| reason | Input| Error reason. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
if (listSize > maxListSize) {
    OP_LOGE_FOR_INVALID_LISTSIZE_WITH_REASON("MyOp", "dims", std::to_string(listSize).c_str(), "exceeds max limit");
    return ge::GRAPH_FAILED;
}
```
