# CheckType

## Function

Checks whether a given data type is in the valid data type list. If the data type is found in the list, `true` is returned; otherwise, `false` is returned.

## Prototype

```cpp
bool CheckType(const DataType dtype, const std::initializer_list<DataType>& validTypes)

template <size_t N>
bool CheckType(const DataType dtype, const std::array<DataType, N>& validTypes)

bool CheckType(const DataType dtype, const std::vector<DataType>& validTypes)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| dtype | Input| Data type to be checked.|
| validTypes | Input| Valid data type list, of the type `std::initializer_list<DataType>`, `std::array<DataType, N>`, or `std::vector<DataType>`.|

## Returns

Returns `true` if `dtype` is in the valid data type list; otherwise, returns `false`. The three overloads behave consistently: they all use a linear search and return `true` as soon as `dtype` is matched in the list. An empty list returns `false`.

## Restrictions

None

## Examples

```cpp
// Check whether dtype is float16 or float32. If not, return in advance.
void Func(const ge::DataType dtype) {
    if (!CheckType(dtype, {DT_FLOAT16, DT_FLOAT})) {
        return;
    }
    // Perform the operator computation logic.
}

// Use a compile-time fixed data type list (std::array) for checking.
void FuncArray(const ge::DataType dtype) {
    static constexpr std::array<ge::DataType, 2> validTypes = {DT_FLOAT16, DT_FLOAT};
    if (!CheckType(dtype, validTypes)) {
        return;
    }
    // Perform the operator computation logic.
}
```
