# OP\_LOGE\_FOR\_INVALID\_CONFIG\_WITH\_REASON

## Function

Records and reports the invalid configuration item value error with reason. When the value of an item in the configuration file is invalid, outputs ERROR-level logs and reports the EZ0033 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_CONFIG_WITH_REASON(opName, configFile, item, value, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| opName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| configFile | Input| Configuration file path. Supports the const char* or std::string type.|
| item | Input| Configuration item name. Supports the const char* or std::string type.|
| value | Input| Actual value. Supports the const char* or std::string type.|
| reason | Input| Error reason. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
if (!IsValidConfig(value)) {
    OP_LOGE_FOR_INVALID_CONFIG_WITH_REASON("MyOp", "config.ini", "block_size", value.c_str(), "out of range");
    return ge::GRAPH_FAILED;
}
```
