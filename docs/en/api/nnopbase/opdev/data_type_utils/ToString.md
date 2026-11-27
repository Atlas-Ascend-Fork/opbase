# ToString

## Function

Serializes a group of data types into a string. An empty list returns `"[]"`. For the `initializer_list` overload, a non-empty list returns a string in the format of `"[DT_FLOAT,DT_INT8,]"`, where each element is followed by an English comma (including the trailing comma). For the `array` and `vector` overloads, a non-empty list returns a string in the format of `"[DT_FLOAT,DT_INT8]"`, where the elements are separated by English commas without a trailing comma.

## Prototype

```cpp
ge::AscendString ToString(const std::initializer_list<DataType>& dataTypes)

template <size_t N>
ge::AscendString ToString(const std::array<DataType, N>& dataTypes)

ge::AscendString ToString(const std::vector<DataType>& dataTypes)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| dataTypes | Input| Data type list to be serialized, of the type `std::initializer_list<DataType>`, `std::array<DataType, N>`, or `std::vector<DataType>`.|

## Returns

Returns the serialized string of the type `ge::AscendString`. An empty list returns `"[]"`. The output formats of the overloads for a non-empty list differ as follows:

- `initializer_list` overload: returns `"[DT_FLOAT,DT_INT8,]"`, where each element is followed by an English comma (including the trailing comma).
- `array` and `vector` overloads: return `"[DT_FLOAT,DT_INT8]"`, where the elements are separated by English commas without a trailing comma.

> **Note:** The output format of the `array`/`vector` overloads differs from that of the `initializer_list` overload. This is an existing design decision: the new overloads fix the trailing comma, while the `initializer_list` overload keeps the original format to avoid breaking existing logs.

## Restrictions

None

## Examples

```cpp
// Serialize a group of data types into a string and print it.
void Func() {
    ge::AscendString typeStr = ToString({DT_FLOAT, DT_INT8});
    // The content of typeStr is "[DT_FLOAT,DT_INT8,]".
}

// Use std::array or std::vector for serialization (without a trailing comma).
void FuncContainer() {
    ge::AscendString arrStr = ToString(std::array<ge::DataType, 2>{DT_FLOAT, DT_INT8});
    // The content of arrStr is "[DT_FLOAT,DT_INT8]".

    ge::AscendString vecStr = ToString(std::vector<ge::DataType>{DT_FLOAT, DT_INT8});
    // The content of vecStr is "[DT_FLOAT,DT_INT8]".
}
```
