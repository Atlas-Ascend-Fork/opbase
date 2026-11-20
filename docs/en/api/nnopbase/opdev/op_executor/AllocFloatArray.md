# AllocFloatArray

## Function

This function is a member function of the `aclOpExecutor` class and is used to allocate an `aclFloatArray` and initialize it with values from the specified memory.

## Prototype

```cpp
aclFloatArray *AllocFloatArray(const float *value, uint64_t size)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| value | Input| Source data, which is used to initialize `aclFloatArray`.|
| size | Input| Number of elements in the source data.|

## Returns

Success: allocated `aclFloatArray`. Failure: `nullptr`.

## Restrictions

The input pointer cannot be null.

## Examples

```cpp
// Allocate an aclFloatArray with a length of 10.
void Func(aclOpExecutor *executor) {
    float myArray[10] = {};
    aclFloatArray *array = executor->AllocFloatArray(myArray, 10);
}
```
