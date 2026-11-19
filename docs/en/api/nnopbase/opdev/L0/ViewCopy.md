# ViewCopy

## Function

This is a common L0 API that copies data from a contiguous tensor to a contiguous or non-contiguous tensor.

Since the output tensor of an L2 API may be non-contiguous, this API is required to copy the contiguous tensor to the non-contiguous tensor.

The output tensor may be contiguous or non-contiguous, and the API handles both cases internally, but the input must be contiguous.

Some aclnn APIs are implemented in-place, that is, the output and the input point to the same memory. When x and y of ViewCopy constitute an in-place relationship (see [Restrictions](#restrictions) for the determining conditions), the 3-parameter prototype disables the aclnn operator cache; with the 4-parameter prototype and `inplaceFlag` set to true, the aclnn operator cache is not disabled.

## Prototype

```cpp
const aclTensor *ViewCopy(const aclTensor *x, const aclTensor *y, aclOpExecutor *executor)
```

```cpp
const aclTensor *ViewCopy(const aclTensor *x, const aclTensor *y, aclOpExecutor *executor, bool inplaceFlag)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| x | Input| Input tensor. Data type and data format are not limited. The input must be stored in contiguous memory. |
| y | Output| Output tensor. Data type and data format are not limited, but its data type, ViewShape, and data format must be the same as those of x. |
| executor | Input| Operator executor, which contains the operator execution flow. |
| inplaceFlag | Input| In-place scenario flag, supported only by the 4-parameter prototype. Set it to true when the caller can determine that x and y constitute an in-place relationship; otherwise, set it to false. For details, see [Restrictions](#restrictions). |

## Returns

The output tensor if the copy succeeds; `nullptr` otherwise.

## Restrictions

The input tensor must be a contiguous tensor.

The data type, data format, and ViewShape of x and y must be the same, and y must not come from a workspace. Otherwise, the API returns `nullptr`.

x and y constitute an in-place relationship when both x and y are contiguous tensors, neither comes from the workspace allocated by the executor (that is, the data of both comes from the tensors passed in by the aclnn API), and `x->GetData()` equals `y->GetData()` (that is, both point to the same memory address of the tensor data). When the caller can determine that x and y constitute an in-place relationship, `inplaceFlag` can be set to true; otherwise, it should be set to false (or the 3-parameter prototype is used). The API identifies and processes this parameter as follows:

- **Identification scenario**: the API identifies `inplaceFlag` only when x and y constitute an in-place relationship; in other cases, this parameter is not identified.
- **Processing method**: after identifying the parameter, the API decides whether to disable the aclnn operator cache based on its value:
  - If `inplaceFlag` is false (or not specified): the aclnn operator cache is disabled. The reason is that the cache key does not contain the memory address of the tensor data, so it cannot distinguish between the two calls where x and y are at the same address and at different addresses, while the execution logic of the aclnn operator differs between these two calls. Reusing the cache generated for the different-address scenario when x and y are at the same address would not match the expectation.
  - If `inplaceFlag` is true: the aclnn operator cache is not disabled. The caller has determined that x and y are in an in-place relationship, so there is no mixed use of same-address and different-address scenarios, and the cache key no longer has the preceding ambiguity, avoiding unnecessary cache invalidation in the in-place scenario.

## Examples

```cpp
// Standard usage: create an OpExecutor
auto uniqueExecutor = CREATE_EXECUTOR();

// If the output parameter out is a non-contiguous tensor, convert the computed contiguous tensor to a non-contiguous one
auto viewCopyResult = l0op::ViewCopy(absResult, out, uniqueExecutor.get());

// In-place scenario: x and y point to the same memory; set inplaceFlag to true
auto viewCopyInplaceResult = l0op::ViewCopy(selfRef, selfRef, uniqueExecutor.get(), true);
```
