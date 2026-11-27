# PromoteType

## Function

Deduces the data type to which parameters should be promoted when performing operations with different data types. For example, when DT_FLOAT16 operates with DT_FLOAT, DT_FLOAT16 should first be promoted to DT_FLOAT before the operation.

## Prototype

```cpp
ge::DataType PromoteType(ge::DataType type_a, ge::DataType type_b)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| type_a | Input| Data type of the first parameter.|
| type_b | Input| Data type of the second parameter.|

## Returns

If the promoted data type cannot be deduced, the operation fails and DT\_UNDEFINED is returned.

## Restrictions

None

## Examples

```cpp
// Check whether dtype can operate with DT_FLOAT. If not, return in advance.
void Func(const ge::DataType dtype) {
    if (PromoteType(dtype, DT_FLOAT) == DT_UNDEFINED) {
        return;
    }
    // Perform the operator computation logic.
}
```
