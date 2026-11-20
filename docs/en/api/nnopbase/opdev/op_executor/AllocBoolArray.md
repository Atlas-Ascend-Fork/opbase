# AllocBoolArray

## Function

This function is a member function of the `aclOpExecutor` class and is used to allocate an `aclBoolArray` and initialize it with values from the specified memory.

## Prototype

```cpp
aclBoolArray *AllocBoolArray(const bool *value, uint64_t size)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| value | Input| Source data, which is used to initialize `aclBoolArray`.|
| size | Input| Number of elements in the source data.|

## Returns

Success: allocated `aclBoolArray`. Failure: `nullptr`.

## Restrictions

The input pointer cannot be null.

## Examples

```cpp
// Allocate an aclBoolArray with a length of 10.
void Func(aclOpExecutor *executor) {
    bool myArray[10] = {};
    aclBoolArray *array = executor->AllocBoolArray(myArray, 10);
}
```
