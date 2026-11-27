# REGISTER\_AICPU\_WORKSPACE\_TILING

## Function

Registers the Tiling function of an AICPU operator to calculate, in advance, the workspace size required for the intermediate computation of a kernel, so that the kernel can use the preallocated device memory to complete the intermediate computation.

The workspace usage flow is as follows:

1. After the AICPU execution path is selected in the first phase, if no workspace parameter is explicitly provided, the framework calls the registered Tiling function when creating the operator execution task to calculate the workspace size required by the operator and includes it in the total workspace size returned by the first-phase API.
2. The caller applies for device memory based on the total size returned by the first phase and passes it in when calling the second-phase API. The framework passes the workspace address and original length planned for the operator to the kernel.
3. The kernel obtains the workspace address and size from the context and stores the intermediate results or temporary buffers according to the layout agreed with the Tiling function, avoiding additional device memory allocation during the intermediate computation.

An operator developer must implement the Tiling function and the kernel together. The registration macro itself does not execute the Tiling function, does not apply for device memory, and does not replace the existing memory allocation code in the kernel.

## Prototype

When using this macro, include the header file `opdev/aicpu/aicpu_task.h`, and link the `nnopbase` library that supports this API on the host side.

```cpp
REGISTER_AICPU_WORKSPACE_TILING(OP_NAME, tilingFunc)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| OP_NAME | Input| Operator type identifier, for example, MyOp. The macro converts it into a string. The name must be the same as the operator type name used when creating the AICPU execution task.|
| tilingFunc | Input| Tiling function implemented on the host side, of the AicpuWorkspaceTilingFunc type. It calculates the workspace size required for the intermediate computation of a kernel based on the descriptions and attributes of the inputs and outputs.|

The Tiling function type is defined as follows:

```cpp
namespace op::internal {
using AicpuWorkspaceTilingFunc = aclnnStatus (*)(OpArgList& inputs, OpArgList& outputs, OpArgList& attrs,
                                                 uint64_t& workspaceSize);
}
```

**Table 1** Parameters of the Tiling function

| Parameter| Input/Output| Description|
| --- | --- | --- |
| inputs | Input| Parameters passed by the current operator through OP_INPUT. You can use them to obtain the descriptions such as the shape and data type of the inputs.|
| outputs | Input| Parameters passed by the current operator through OP_OUTPUT. You can use them to obtain the descriptions of the outputs.|
| attrs | Input| Attributes passed by the current operator through OP_ATTR, in the same order as when the operator execution task is created.|
| workspaceSize | Output| Original length of the workspace required for the intermediate computation of the current operator kernel, in bytes. It must be assigned before a successful return. 0 indicates that no additional workspace is required. This value does not include the memory requirements of other operators in the whole graph.|

The Tiling function returns `ACLNN_SUCCESS` on success. On failure, it returns the corresponding error code, and the framework stops creating the execution task of the operator and returns the error to the caller.

When the Tiling function returns a positive value, the framework generates the workspace tensor description and includes it in the memory planning. In this case, no actual device memory has been applied for. The total size of the whole graph returned by [GetWorkspaceSize](../op_executor/GetWorkspaceSize.md) also includes other intermediate tensors, alignment, and AICPU running parameter caches. The kernel receives the original workspace length of the current operator.

An operator creates the execution task through [ADD_TO_LAUNCHER_LIST_AICPU](ADD_TO_LAUNCHER_LIST_AICPU.md). If a workspace parameter is explicitly provided through [OP_WORKSPACE](OP_WORKSPACE.md), the framework skips the Tiling function. This parameter must be a single DT_UINT8 workspace tensor managed by the executor, and its original length must be greater than 0.

## Returns

None. This macro is used to generate a registration declaration and is not used as an expression.

## Restrictions

**Registration requirements**

- Use this macro in the namespace scope of the host code, and complete the registration before the first creation of the operator execution task. Do not register again in each L0 call.
- For repeated registration with the same operator name, the first Tiling function is retained. The dynamic library where the function resides must remain loaded during the possible invocation period.

**Tiling function requirements**

- Only read the descriptions and attributes available in the first phase. Do not read device computation results that are not ready, do not modify input parameters or retain parameter references, and do not apply for device memory. The function must be reentrant and must not depend on the number of invocations.
- `workspaceSize` must cover all the temporary device memory required for the intermediate computation of the kernel and must not exceed the maximum value of `int64_t`. When calculating the size, overflow must be checked before addition and multiplication.

**Kernel requirements**

- The memory layout and used size must be consistent with the Tiling function, and the access must not be out of bounds. The intermediate computation must use the passed workspace, and no additional device memory must be allocated for this part of the computation.

## Examples

Implement and register the Tiling function on the host side:

```cpp
#include "opdev/aicpu/aicpu_task.h"

namespace {
aclnnStatus MyOpWorkspaceTiling(op::OpArgList&, op::OpArgList&, op::OpArgList&, uint64_t& workspaceSize)
{
    // Fixed requirement of the example operator. A real operator should calculate the actual requirement based on the inputs and attributes.
    constexpr uint64_t kExampleWorkspaceBytes = 32U;
    workspaceSize = kExampleWorkspaceBytes;
    return ACLNN_SUCCESS;
}

REGISTER_AICPU_WORKSPACE_TILING(MyOp, MyOpWorkspaceTiling);
}
```

This example assumes that the intermediate computation of the MyOp kernel requires 32 bytes of temporary memory, and the Tiling function sets `workspaceSize` to 32. The operator type name used by the L0 API should be `MyOp`.

When the L0 API does not explicitly provide a workspace parameter, the framework calls the Tiling function and includes the 32-byte requirement in the first-phase memory planning. The caller applies for device memory based on the total workspace size returned by the first phase and passes it in to the second-phase API. The framework passes the workspace address planned for MyOp and the original length of 32 bytes to the kernel. The MyOp kernel obtains this workspace from the context and uses it to complete the intermediate computation, without applying for the 32-byte temporary device memory inside the kernel again.

In the `Compute` method of the kernel, call `aicpu::CpuKernelUtils::GetWorkspaceInfo(ctx, workspaceSize, workspaceAddr)` to obtain the workspace information. The second parameter outputs the size, in bytes, and the third parameter outputs the device address. The following example only shows the retrieval and verification. In a real operator, the computation logic should use the memory within this address range after the verification.

```cpp
#include "cpu_kernel.h"
#include "cpu_kernel_utils.h"
#include "status.h"

class MyOpCpuKernel : public aicpu::CpuKernel {
public:
    uint32_t Compute(aicpu::CpuKernelContext& ctx) override
    {
        uint64_t workspaceSize = 0U;
        uint64_t workspaceAddr = 0U;
        aicpu::CpuKernelUtils::GetWorkspaceInfo(ctx, workspaceSize, workspaceAddr);

        // Consistent with the temporary memory requirement agreed with the host-side Tiling function.
        constexpr uint64_t kRequiredWorkspaceBytes = 32U;
        if (workspaceAddr == 0U || workspaceSize < kRequiredWorkspaceBytes) {
            return aicpu::KERNEL_STATUS_PARAM_INVALID;
        }

        // Complete the intermediate computation within the range starting from workspaceAddr with a length of workspaceSize bytes.
        return aicpu::KERNEL_STATUS_OK;
    }
};
```

In this example, during a normal call, `workspaceSize` should be 32, and `workspaceAddr` is the device address planned by the framework for MyOp. The kernel only uses this memory in the current call and must not release it or retain it for use in subsequent calls.
