# OP\_LOGE\_FOR\_INVALID\_TENSORNUM

## Function

Records and reports the parameter tensor quantity verification error. When the tensor quantity of a specified parameter of an operator does not meet expectations, outputs ERROR-level logs and reports the EZ0022 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_TENSORNUM(entityName, paramName, incorrectNum, correctNum)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| incorrectNum | Input| Actual tensor quantity, of the int64_t type.|
| correctNum | Input| Expected tensor quantity. Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameter instance of Foreach has invalid tensor num 1000.
//          It should be within the range [1, 950].
if (tensorCount > MAX_COUNT || tensorCount <= 0) {
    OP_LOGE_FOR_INVALID_TENSORNUM("Foreach", "instance",
        static_cast<int64_t>(tensorCount),
        ("within the range [1, " + std::to_string(MAX_COUNT) + "]").c_str());
    return ge::GRAPH_FAILED;
}
```
