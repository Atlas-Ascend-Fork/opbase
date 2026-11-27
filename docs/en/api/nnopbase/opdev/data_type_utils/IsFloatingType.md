# IsFloatingType

## Function

Checks whether the input data type is a floating-point type, including DT_DOUBLE, DT_FLOAT, DT_BF16, DT_FLOAT16, DT_HIFLOAT8, DT_FLOAT8_E5M2, DT_FLOAT8_E4M3FN, DT_FLOAT8_E8M0, DT_FLOAT6_E3M2, DT_FLOAT6_E2M3, DT_FLOAT4_E2M1, DT_FLOAT4_E1M2, DT_HIFLOAT4, and DT_HIFLOAT4_SCALE.

## Prototype

```cpp
bool IsFloatingType(const ge::DataType type)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| type | Input| Input data type.|

## Returns

Returns `true` if the data type is a floating-point type; otherwise, returns `false`.

## Restrictions

None

## Examples

```cpp
// Check whether dtype is a floating-point type. If not, return in advance.
void Func(const ge::DataType dtype) {
    if (!IsFloatingType(dtype)) {
        return;
    }
    // Perform the operator computation logic.
}
```
