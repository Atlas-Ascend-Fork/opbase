# REGISTER\_AICPU\_WORKSPACE\_TILING

## 宏功能

注册AICPU算子的Tiling函数，提前计算kernel中间计算所需的workspace大小，供kernel使用预分配的设备内存完成中间计算。

workspace的使用流程如下：

1. 第一阶段选择AICPU执行路径后，未显式提供workspace参数时，框架在创建算子执行任务时调用已注册的Tiling函数，计算该算子所需的workspace大小，并纳入第一阶段接口返回的workspace总大小。
2. 调用方根据第一阶段返回的总大小申请设备内存，在调用第二阶段接口时传入。框架将规划给该算子的workspace地址和原始长度传给kernel。
3. kernel从上下文中获取workspace地址和大小，按照与Tiling函数约定的布局存放中间结果或临时缓冲，避免在中间计算过程中额外申请设备内存。

算子开发者须配套实现Tiling函数和kernel；注册宏本身不执行Tiling函数、不申请设备内存，也不会替换kernel中原有的内存申请代码。

## 宏原型

使用时包含头文件`opdev/aicpu/aicpu_task.h`，Host侧链接支持本接口的`nnopbase`库。

```cpp
REGISTER_AICPU_WORKSPACE_TILING(OP_NAME, tilingFunc)
```

## 参数说明

| 参数 | 输入/输出 | 说明 |
| --- | --- | --- |
| OP_NAME | 输入 | 算子类型标识符，例如MyOp。宏将其转换为字符串，名称须与创建AICPU执行任务时使用的算子类型名称一致。 |
| tilingFunc | 输入 | 在Host侧实现的Tiling函数，类型为AicpuWorkspaceTilingFunc。根据输入、输出的描述信息和属性，计算kernel中间计算所需的workspace大小。 |

Tiling函数类型定义如下：

```cpp
namespace op::internal {
using AicpuWorkspaceTilingFunc = aclnnStatus (*)(OpArgList& inputs, OpArgList& outputs, OpArgList& attrs,
                                                 uint64_t& workspaceSize);
}
```

**表 1**  Tiling函数参数说明

| 参数 | 输入/输出 | 说明 |
| --- | --- | --- |
| inputs | 输入 | 当前算子通过OP_INPUT传入的参数，可用于获取输入的形状、数据类型等描述信息。 |
| outputs | 输入 | 当前算子通过OP_OUTPUT传入的参数，可用于获取输出的描述信息。 |
| attrs | 输入 | 当前算子通过OP_ATTR传入的属性，顺序与创建该算子执行任务时一致。 |
| workspaceSize | 输出 | 当前算子kernel中间计算所需workspace的原始长度，单位为字节。成功返回前必须赋值，0表示无需新增workspace。该值不包含整图中其他算子的内存需求。 |

Tiling函数成功时返回`ACLNN_SUCCESS`；失败时返回相应错误码，框架停止创建该算子的执行任务，并将错误返回给调用方。

Tiling函数返回正数时，框架生成workspace tensor描述并纳入内存规划，此时尚未申请实际设备内存。[GetWorkspaceSize](../op_executor/GetWorkspaceSize.md)返回的整图总大小还包含其他中间tensor、对齐和AICPU运行参数缓存；kernel收到的是当前算子的workspace原始长度。

算子通过[ADD_TO_LAUNCHER_LIST_AICPU](ADD_TO_LAUNCHER_LIST_AICPU.md)创建执行任务。若已通过[OP_WORKSPACE](OP_WORKSPACE.md)显式提供workspace参数，框架跳过Tiling函数；该参数须为executor管理的单个DT_UINT8 workspace tensor，原始长度大于0。

## 返回值说明

无。本宏用于生成注册声明，不作为表达式使用。

## 约束说明

**注册要求**

- 在Host代码的命名空间作用域使用本宏，并在首次创建该算子的执行任务前完成注册，不在每次L0调用时重复注册。
- 相同算子名称的重复注册保留首次Tiling函数。该函数所在动态库在可能调用期间须保持加载。

**Tiling函数要求**

- 只读取第一阶段可用的描述信息和属性，不读取尚未就绪的设备计算结果，不修改输入参数或保存参数引用，不申请设备内存；函数应可重入，不依赖调用次数。
- `workspaceSize`须覆盖kernel中间计算所需的全部临时设备内存，且不超过`int64_t`的最大值。计算大小时须在加法、乘法前校验溢出。

**kernel要求**

- 内存布局和使用大小须与Tiling函数一致，访问不得越界；中间计算使用传入的workspace，不再为这部分计算额外申请设备内存。

## 调用示例

Host侧实现并注册Tiling函数：

```cpp
#include "opdev/aicpu/aicpu_task.h"

namespace {
aclnnStatus MyOpWorkspaceTiling(op::OpArgList&, op::OpArgList&, op::OpArgList&, uint64_t& workspaceSize)
{
    // 示例算子的固定需求；实际算子应根据输入和属性计算真实需求。
    constexpr uint64_t kExampleWorkspaceBytes = 32U;
    workspaceSize = kExampleWorkspaceBytes;
    return ACLNN_SUCCESS;
}

REGISTER_AICPU_WORKSPACE_TILING(MyOp, MyOpWorkspaceTiling);
}
```

本例假设MyOp的kernel中间计算需要32字节临时内存，Tiling函数将`workspaceSize`设置为32。L0接口使用的算子类型名称应为`MyOp`。

当L0未显式提供workspace参数时，框架调用该Tiling函数，将32字节需求纳入第一阶段的内存规划。调用方按第一阶段返回的workspace总大小申请设备内存并传入第二阶段接口；框架将规划给MyOp的workspace地址和原始长度32字节传给kernel。MyOp的kernel从上下文中获取这段workspace，使用它完成中间计算，无需在kernel内再申请这32字节的临时设备内存。

在kernel的`Compute`方法中，调用`aicpu::CpuKernelUtils::GetWorkspaceInfo(ctx, workspaceSize, workspaceAddr)`获取workspace信息。第二个参数输出大小，单位为字节；第三个参数输出设备地址。以下示例仅展示获取和校验，实际算子的计算逻辑应在校验后使用该地址范围内的内存。

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

        // 与Host侧Tiling函数约定的临时内存需求一致。
        constexpr uint64_t kRequiredWorkspaceBytes = 32U;
        if (workspaceAddr == 0U || workspaceSize < kRequiredWorkspaceBytes) {
            return aicpu::KERNEL_STATUS_PARAM_INVALID;
        }

        // 在workspaceAddr起、长度为workspaceSize字节的范围内完成中间计算。
        return aicpu::KERNEL_STATUS_OK;
    }
};
```

本例正常调用时，`workspaceSize`应为32，`workspaceAddr`为框架规划给MyOp的设备地址。kernel仅在当前调用中使用这段内存，不得释放或保存供后续调用使用。
