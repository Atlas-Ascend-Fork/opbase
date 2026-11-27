# IsIntegralType (Including Bool)

## Function

Checks whether the input data type is an integer type, including DT_INT8, DT_INT16, DT_INT32, DT_INT64, DT_UINT8, DT_UINT16, DT_UINT32, and DT_UINT64.

If `include_bool` is set to `true`, `DT_BOOL` is also considered an integer type.

## Prototype

```cpp
bool IsIntegralType(const ge::DataType type, const bool include_bool)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| type | Input| Input data type.|
| include_bool | Input| Whether to consider `bool` an integer type.|

## Returns

Returns `true` if the data type is an integer type; otherwise, returns `false`.

## Restrictions

None

## Examples

```cpp
// Check whether dtype is an integer type (including bool). If not, return in advance.
void Func(const ge::DataType type) {
    if (!IsIntegralType(type, true)) {
        return;
    }
    // Perform the operator computation logic.
}
```
