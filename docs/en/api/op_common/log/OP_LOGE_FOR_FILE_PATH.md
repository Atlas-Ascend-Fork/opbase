# OP\_LOGE\_FOR\_FILE\_PATH

## Function

Records and reports the invalid file path error. When the input file path is invalid, outputs ERROR-level logs and reports the EZ0029 error code.

## Prototype

```cpp
OP_LOGE_FOR_FILE_PATH(opName, filePath, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| opName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| filePath | Input| File path. Supports the const char* or std::string type.|
| reason | Input| Error reason. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
if (access(filePath, F_OK) != 0) {
    OP_LOGE_FOR_FILE_PATH("MyOp", filePath, "file does not exist");
    return ge::GRAPH_FAILED;
}
```
