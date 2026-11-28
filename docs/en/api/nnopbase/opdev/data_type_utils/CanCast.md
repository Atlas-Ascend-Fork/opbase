# CanCast

## Function

Checks whether a source data type can be cast to a destination data type.

## Prototype

```cpp
bool CanCast(const ge::DataType from, const ge::DataType to)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| from | Input| Source data type.|
| to | Input| Destination data type.|

## Returns

Returns `true` if the cast succeeds; otherwise, returns `false`.

## Restrictions

None

## Examples

```cpp
// Check whether dtype can be cast to int8. If not, return in advance.
void Func(const ge::DataType dtype) {
    if (!op::CanCast(dtype, ge::DataType::DT_INT8)) {
        return;
    }
    // Perform the operator computation logic.
}
```
