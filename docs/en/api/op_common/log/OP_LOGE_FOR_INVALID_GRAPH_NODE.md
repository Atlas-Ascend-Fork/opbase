# OP\_LOGE\_FOR\_INVALID\_GRAPH\_NODE

## Function

Records and reports the invalid graph node error. When the input graph contains invalid nodes, outputs ERROR-level logs and reports the EZ0036 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_GRAPH_NODE(nodeName, reason)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| nodeName | Input| Graph node name. Supports the const char* or std::string type.|
| reason | Input| Error reason. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
if (nodeType == nullptr) {
    OP_LOGE_FOR_INVALID_GRAPH_NODE(nodeName, "node type is null");
    return ge::GRAPH_FAILED;
}
```
