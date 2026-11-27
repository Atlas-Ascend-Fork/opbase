# Transpose

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Not supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference products: Supported
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas training products: Supported
<!-- end id6 -->

## Function

This API does not change the value of the tensor data but transposes the tensor.

Through the Transpose API, the arrangement order of the tensor in different dimensions can be changed, implementing dimension rearrangement of the tensor.

Specifically, the shape of the input tensor x passed in by the user is transposed according to the permutation perm of the specified dimensions and then output.

## Prototype

- The input and output are at different addresses

    ```cpp
    const aclTensor *Transpose(const aclTensor *x, const aclTensor *y, const aclTensor *perm, aclOpExecutor *executor)
    ```

- The input and output are at the same address

    ```cpp
    const aclTensor *Transpose(const aclTensor *x, const aclIntArray *perm, aclOpExecutor *executor)
    ```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| x | Input/Output| Original input tensor. The input tensor x must be stored in contiguous memory. The supported data types are FLOAT16, FLOAT, INT8, INT16, INT32, INT64, UINT8, UINT16, UINT32, UINT64, BOOL, and BFLOAT16. The supported data format is ND. |
| y | Output| Output tensor after transposition. Its data type and data format are the same as those of x. |
| perm | Input| Integer array, representing the dimensions of the input tensor x. The supported types are aclIntArray\* and aclTensor*. A transpose of up to 8 dimensions is supported, and the value must be in the range [0, number of dimensions of x - 1]. The supported data types are INT32 and INT64. The supported data format is ND. |
| executor | Input| Operator executor, which contains the operator execution flow. |

<!-- npu="A3,910b" id7 -->
> **NOTE**: BFLOAT16 is only supported on the following products:
>
> - Atlas A2 products
> - Atlas A3 products
<!-- end id7 -->

## Returns

The transposed tensor if the transposition succeeds; `nullptr` otherwise.

## Restrictions

- A transpose of up to 8 dimensions is supported, that is, the dims of input x and perm must be at most 8.
- The dimensions of input x and perm must be the same.

## Examples

```cpp
// Standard usage: create an OpExecutor and check parameters
auto uniqueExecutor = CREATE_EXECUTOR();
CHECK_RET(uniqueExecutor.get() != nullptr, ACLNN_ERR_INNER_CREATE_EXECUTOR);

// Standard usage: convert the input self to a contiguous tensor
auto selfContiguous = l0op::Contiguous(self, uniqueExecutor.get());
CHECK_RET(selfContiguous != nullptr, ACLNN_ERR_INNER_NULLPTR);

int64_t dims = selfContiguous->GetViewShape().GetDimNum();
int64_t valuePerm[dims] = {0, 2, 1, 3}; // Transpose the middle two of the original four dimensions, that is, swap axis 1 and axis 2

auto perm = executor->AllocIntArray(valuePerm, dims);
selfContiguous = l0op::Transpose(selfContiguous, perm, uniqueExecutor.get());
```
