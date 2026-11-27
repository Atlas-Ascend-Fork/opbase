# OP\_LOGE\_FOR\_INVALID\_CONFIGS\_WITH\_REASON

## Function

Records and reports the invalid value error for multiple configuration items with reason. When the values of multiple items in the configuration file are invalid, outputs ERROR-level logs and reports the EZ0034 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_CONFIGS_WITH_REASON(opName, configFile, items, values, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| opName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| configFile | Input| Configuration file path. Supports the const char* or std::string type.|
| items | Input| List of configuration item names. Supports the const char* or std::string type.|
| values | Input| List of actual values. Supports the const char* or std::string type.|
| reason | Input| Error reason. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
OP_LOGE_FOR_INVALID_CONFIGS_WITH_REASON("MyOp", "config.ini", "block_size, ub_size", "128, 64", "value exceeds limit");
```
