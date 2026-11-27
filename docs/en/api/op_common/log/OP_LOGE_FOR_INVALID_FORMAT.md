# OP\_LOGE\_FOR\_INVALID\_FORMAT

## Function

Records and reports the parameter format verification error. When the format of a specified parameter of an operator does not meet expectations, outputs ERROR-level logs and reports the EZ0017 error code.

## Prototype

```cpp
OP_LOGE_FOR_INVALID_FORMAT(entityName, paramName, incorrectFormat, correctFormat)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Parameter name. Supports the const char* or std::string type.|
| incorrectFormat | Input| Actual format (for example, "NHWC"). Supports the const char* or std::string type.|
| correctFormat | Input| Expected format (for example, "NCHW"). Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Parameter x of ResizeBilinearV2 has incorrect format ND. It should be
//          NCHW or NHWC.
if (format_ != ge::FORMAT_NCHW && format_ != ge::FORMAT_NHWC) {
    OP_LOGE_FOR_INVALID_FORMAT("ResizeBilinearV2", "x",
        Ops::Base::ToString(format_).c_str(), "NCHW or NHWC");
    return ge::GRAPH_FAILED;
}
```
