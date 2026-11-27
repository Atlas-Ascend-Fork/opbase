# BroadcastShape

## Function

Deduces the output shape after broadcasting based on the shapes of the input tensors. Two overloads are supported: two inputs and multiple inputs.

## Prototype

```cpp
bool BroadcastShape(const gert::Shape* in1Shape, const gert::Shape* in2Shape, gert::Shape* outShape)
```

```cpp
bool BroadcastShape(const std::vector<const gert::Shape*>& inShapes, gert::Shape* outShape)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| in1Shape | Input| Shape of the first input tensor.|
| in2Shape | Input| Shape of the second input tensor.|
| inShapes | Input| List of the shapes of all input tensors in the multi-input scenario.|
| outShape | Output| Deduced output shape.|

## Returns

Returns a bool value:

- true: The broadcast deduction succeeds.
- false: The broadcast deduction fails because the input shapes cannot be broadcast.

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
// Two-input scenario
auto in1Shape = context->GetInputShape(0);
auto in2Shape = context->GetInputShape(1);
auto outShape = context->GetOutputShape(0);
if (!Ops::Base::BroadcastShape(in1Shape, in2Shape, outShape)) {
    return ge::GRAPH_FAILED;
}

// Multi-input scenario
std::vector<const gert::Shape*> inShapes = {context->GetInputShape(0), context->GetInputShape(1), context->GetInputShape(2)};
auto outShape = context->GetOutputShape(0);
if (!Ops::Base::BroadcastShape(inShapes, outShape)) {
    return ge::GRAPH_FAILED;
}
```
