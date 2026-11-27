# IsComplexType

## Function

Checks whether the input data type is a complex type, including DT_COMPLEX32, DT_COMPLEX64, and DT_COMPLEX128.

## Prototype

```cpp
bool IsComplexType(const ge::DataType type)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| type | Input| Input data type.|

## Returns

Returns `true` if the input data type is a complex type; otherwise, returns `false`.

## Restrictions

None

## Examples

```cpp
// Check whether dtype is a complex type. If not, return in advance.
void Func(const ge::DataType dtype) {
    if (!IsComplexType(dtype)) {
        return;
    }
    // Perform the operator computation logic.
}
```
