# OP\_LOGE\_FOR\_FILE\_OPEN

## Function

Records and reports the file opening error. When a file cannot be opened, outputs ERROR-level logs and reports the EZ0030 error code.

## Prototype

```cpp
OP_LOGE_FOR_FILE_OPEN(opName, filePath, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| opName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| filePath | Input| File path. Supports the const char* or std::string type.|
| reason | Input| Reason why the file cannot be opened. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
std::ifstream ifs(filePath);
if (!ifs.is_open()) {
    OP_LOGE_FOR_FILE_OPEN("MyOp", filePath, "permission denied");
    return ge::GRAPH_FAILED;
}
```
