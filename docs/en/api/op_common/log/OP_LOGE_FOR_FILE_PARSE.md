# OP\_LOGE\_FOR\_FILE\_PARSE

## Function

Records and reports the file parsing error. When the content of a file cannot be parsed, outputs ERROR-level logs and reports the EZ0031 error code.

## Prototype

```cpp
OP_LOGE_FOR_FILE_PARSE(opName, fileName, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| opName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| fileName | Input| File name. Supports the const char* or std::string type.|
| reason | Input| Reason why the file cannot be parsed. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
if (!ParseJsonFile(fileName, config)) {
    OP_LOGE_FOR_FILE_PARSE("MyOp", fileName, "invalid json format");
    return ge::GRAPH_FAILED;
}
```
