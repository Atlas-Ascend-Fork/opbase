# Reshape

## Function

This API does not change the tensor data but only converts the shape of the input tensor x passed in by the user to the second parameter shape.

## Prototype

```cpp
const aclTensor *Reshape(const aclTensor *x, const op::Shape &shape, aclOpExecutor *executor)
```

```cpp
const aclTensor *Reshape(const aclTensor *x, const aclIntArray *shape, aclOpExecutor *executor)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| x | Input| Input tensor to be converted. Data type and data format are not limited. The input must be stored in contiguous memory. |
| shape | Input| Target shape after conversion. The supported types are aclIntArray* and op::Shape (that is, gert::Shape). Data type and data format are not limited. |
| executor | Input| Operator executor, which contains the operator execution flow. |

## Returns

An `aclTensor` carrying the target shape information if the Reshape conversion succeeds; `nullptr` otherwise.

## Restrictions

- The Reshape conversion succeeds only if the ShapeSize of x equals the ShapeSize of the second parameter shape. For example, if the shape of A is \(1, 3, 256, 256\), the ShapeSize of A is 1\*3\*256\*256.
- Converting to an empty tensor (a shape containing 0) is currently not supported.

## Examples

```cpp
void Func(const aclTensor *x, const op::Shape &shape, aclOpExecutor *executor) {
    auto ret = l0op::Reshape(x, shape, executor);
    return;
}
```
