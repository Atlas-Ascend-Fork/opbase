# OP\_LOGE\_FOR\_INVALID\_CONFIG

## Function

Records and reports the invalid configuration item value error. When the value of an item in the configuration file does not meet expectations, outputs ERROR-level logs and reports the EZ0032 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_CONFIG(opName, configFile, item, value, expect)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| opName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| configFile | Input| Configuration file path. Supports the const char* or std::string type.|
| item | Input| Configuration item name. Supports the const char* or std::string type.|
| value | Input| Actual value. Supports the const char* or std::string type.|
| expect | Input| Expected value. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
if (configValue != "3") {
    OP_LOGE_FOR_INVALID_CONFIG("MyOp", "config.ini", "version", configValue.c_str(), "3");
    return ge::GRAPH_FAILED;
}
```
