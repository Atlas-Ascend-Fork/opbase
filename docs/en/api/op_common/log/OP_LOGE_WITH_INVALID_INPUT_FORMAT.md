# OP\_LOGE\_WITH\_INVALID\_INPUT\_FORMAT

> **Note:** This API is deprecated. It is recommended that you use [OP_LOGE_FOR_INVALID_FORMAT](OP_LOGE_FOR_INVALID_FORMAT.md) instead.

## Function

Records and reports the operator input format verification error. When the format of a specified input parameter of an operator does not meet expectations, outputs ERROR-level logs and reports the EZ0006 error code.

## Prototype

```cpp
OP_LOGE_WITH_INVALID_INPUT_FORMAT(entityName, paramName, dataFormat, expectedFormatList)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| entityName | Input| Operator name or aclnn API name. Supports the const char* or std::string type.|
| paramName | Input| Input parameter name. Supports the const char* or std::string type.|
| dataFormat | Input| Actual data format (for example, "NHWC"). Supports the const char* or std::string type.|
| expectedFormatList | Input| List of expected formats (for example, "NCHW, NCDHW"). Supports the const char* or std::string type.|

## Returns

None

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Expected output: Input parameter x of ResizeBilinearV2 has incorrect format ND. It should be NCHW or NHWC.
if (format_ != NCHW && format_ != NHWC) {
    OP_LOGE_WITH_INVALID_INPUT_FORMAT("ResizeBilinearV2", "x",
        Ops::Base::ToString(format_).c_str(), "NCHW or NHWC");
    return ge::GRAPH_FAILED;
}
```
