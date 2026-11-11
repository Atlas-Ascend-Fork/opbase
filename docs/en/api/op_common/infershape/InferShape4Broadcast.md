# InferShape4Broadcast

## Function

Infers an output tensor's shape for broadcast operators in graph mode. The overload without `inputNum` uses the first two inputs; the other overload uses the specified number of inputs.

## Prototype

```cpp
ge::graphStatus InferShape4Broadcast(gert::InferShapeContext* context)
```

```cpp
ge::graphStatus InferShape4Broadcast(gert::InferShapeContext* context, size_t inputNum)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| context | Input| `InferShape` context passed by GE.|
| inputNum | Input| Number of inputs participating in broadcasting. Must be at least 1.|

## Returns

The return type is `ge::graphStatus`:

- ge::GRAPH\_SUCCESS: `InferShape` succeeded.
- ge::GRAPH\_FAIL: `InferShape` failed.

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Infer the output shape for the two-input Add operator using broadcasting rules.
ge::graphStatus InferShape4Add(gert::InferShapeContext* context)
{
    return Ops::Base::InferShape4Broadcast(context);
}
// Register the Add operator and its shape inference function with GE.
IMPL_OP_INFERSHAPE(Add).InferShape(InferShape4Add);
```
