# InferShape4Reduce

## Function

Infers an output tensor's shape. It is the `InferShape` for `Reduce` operators in graph mode.

## Prototype

```cpp
ge::graphStatus InferShape4Reduce(gert::InferShapeContext *context)
```

Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| context | Input| `InferShape` context passed by GE.|

## Returns

The return type is `ge::graphStatus`:

- ge::GRAPH\_SUCCESS: `InferShape` succeeded.
- ge::GRAPH\_FAILED: `InferShape` failed.

## Restrictions

- Input 0 is the tensor to reduce; input 1 is an int32 or int64 tensor containing the reduction axes.
- Attribute 0 is the bool keep_dims attribute.
- Use this helper only for Reduce operators with this input and attribute layout. Shape-preserving elementwise operators such as IsFinite should use InferShape4Elewise.

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Reuse Reduce shape inference when ReduceSum has inputs x and axes, and attribute 0 is keep_dims.
ge::graphStatus InferShape4ReduceSum(gert::InferShapeContext* context)
{
    return Ops::Base::InferShape4Reduce(context);
}
// Register ReduceSum and its shape inference function with GE.
IMPL_OP_INFERSHAPE(ReduceSum).InferShape(InferShape4ReduceSum);
```
