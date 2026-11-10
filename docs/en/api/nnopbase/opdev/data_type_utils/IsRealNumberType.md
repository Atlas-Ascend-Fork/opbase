# IsRealNumberType

## Function

Checks if the input data type only consists of real-number types, including DT_DOUBLE, DT_FLOAT, DT_FLOAT16, DT_INT16, DT_INT32, DT_INT64, DT_INT8, DT_UINT16, DT_UINT32, DT_UINT64, DT_UINT8, DT_BF16, DT_HIFLOAT8, DT_FLOAT8_E5M2, DT_FLOAT8_E4M3FN, DT_FLOAT8_E8M0, DT_FLOAT6_E3M2, DT_FLOAT6_E2M3, DT_FLOAT4_E2M1, DT_FLOAT4_E1M2, DT_HIFLOAT4, and DT_HIFLOAT4_SCALE.

## Prototype

```cpp
bool IsRealNumberType(const DataType dtype)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| dtype | Input| Input data type.|

## Returns

If the input data type only consists of real-number types, **true** is returned. Otherwise, **false** is returned.

## Restrictions

None

## Examples

```cpp
// If the data type consists of more than just real-number types, the function returns.
void Func(const DataType dtype) {
    if (!IsRealNumberType(dtype)) {
        return;
    }
}
```
