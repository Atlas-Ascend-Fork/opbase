# OP\_LOGE\_FOR\_INVALID\_LISTSIZE

## Function

Records and reports the parameter list size verification error. When the list size (number of elements) of a specified parameter of an operator does not meet expectations, outputs ERROR-level logs and reports the EZ0025 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_LISTSIZE(entityName, paramName, incorrectSize, correctSize)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| incorrectSize | Input| Actual list size. Supports the const char* or std::string type.|
| correctSize | Input| Expected list size. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameter axes of SoftmaxV2 has incorrect element nums 2. It should be 1.
if (axes->GetSize() != 1) {
    OP_LOGE_FOR_INVALID_LISTSIZE("SoftmaxV2", "axes",
        std::to_string(axes->GetSize()), "1");
    return ge::GRAPH_FAILED;
}
```
