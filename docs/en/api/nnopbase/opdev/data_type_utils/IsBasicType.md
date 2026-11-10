# IsBasicType

## Function

Checks whether the input data type is a basic type, including DT_COMPLEX128, DT_COMPLEX64, DT_DOUBLE, DT_FLOAT, DT_FLOAT16, DT_INT16, DT_INT32, DT_INT64, DT_INT8, DT_QINT16, DT_QINT32, DT_QINT8, DT_QUINT16, DT_QUINT8, DT_UINT16, DT_UINT32, DT_UINT64, DT_UINT8, and DT_BF16.

## Prototype

```cpp
bool IsBasicType(const DataType dtype)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| dtype | Input| Input data type.|

## Returns

If the input data type is a basic type, true is returned. Otherwise, false is returned.

## Restrictions

None

## Examples

```cpp
// If the data type is not a basic type, a return statement is executed.
void Func(const DataType dtype) {
    if (!IsBasicType(dtype)) {
        return;
    }
}
```
