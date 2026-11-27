# OP\_LOGE\_FOR\_INVALID\_ARGUMENT\_WITH\_REASON

## Function

Records and reports the invalid argument error with reason. When a specified parameter of an operator is invalid, outputs ERROR-level logs and reports the EZ0037 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_ARGUMENT_WITH_REASON(entityName, paramName, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| reason | Input| Error reason. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
if (axesSize > xShape.GetDimNum()) {
    OP_LOGE_FOR_INVALID_ARGUMENT_WITH_REASON("MyOp", "axes", "axes size exceeds input dim num");
    return ge::GRAPH_FAILED;
}
```
