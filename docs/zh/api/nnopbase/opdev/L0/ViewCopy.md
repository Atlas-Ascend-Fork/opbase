# ViewCopy

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：不支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品：支持
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas训练系列产品：支持
<!-- end id6 -->

## 功能说明

该函数为公共的L0接口，作用是将连续tensor搬运到连续或非连续tensor上。

由于L2级API的输出tensor可能是非连续的，需要通过该API将连续的tensor搬运到非连续的tensor上。

输出tensor可以是连续或非连续的，接口内部会兼容处理，但输入要求是连续的。

部分aclnn接口采用原地实现，即输出与输入指向同一块内存。当调用ViewCopy的x、y构成原地关系（判定条件见[约束说明](#约束说明)）时，3参数原型会禁用aclnn算子缓存；使用4参数原型并将`inplaceFlag`设置为true，则不禁用aclnn算子缓存。

## 函数原型

```cpp
const aclTensor *ViewCopy(const aclTensor *x, const aclTensor *y, aclOpExecutor *executor)
```

```cpp
const aclTensor *ViewCopy(const aclTensor *x, const aclTensor *y, aclOpExecutor *executor, bool inplaceFlag)
```

## 参数说明

| 参数 | 输入/输出 | 说明 |
| --- | --- | --- |
| x | 输入 | 输入tensor。数据类型和数据格式不限制。输入必须保证是连续内存数据。 |
| y | 输出 | 输出tensor。数据类型和数据格式不限制，但数据类型、ViewShape和数据格式要求与x一致。 |
| executor | 输入 | op执行器，包含了算子计算流程。 |
| inplaceFlag | 输入 | 原地场景标记，仅4参数原型支持。当调用方能确定x、y构成原地关系时，设置为true；否则为false。详细说明参见[约束说明](#约束说明)。 |

## 返回值说明

若转换成功，返回为输出Tensor，若转换失败，则返回nullptr。

## 约束说明

要求输入tensor是连续的tensor。

x与y的数据类型、数据格式和ViewShape需一致，且y不能来自workspace，否则接口返回nullptr。

x与y构成原地关系的条件为：x、y均为连续tensor，均不是executor分配的workspace数据（即二者的数据均来自aclnn接口传入的tensor），且`x->GetData()`与`y->GetData()`相等（即二者指向同一块tensor数据的内存地址）。当调用方能确定x、y构成原地关系时，可将`inplaceFlag`设置为true；否则应设置为false（或使用3参数原型）。接口按下述逻辑识别并处理该参数：

- **识别场景**：仅在x与y构成原地关系时，接口识别`inplaceFlag`；其余情况不识别该参数。
- **处理方式**：识别到该参数后，接口据其取值决定是否禁用aclnn算子缓存：
  - `inplaceFlag`为false（或未指定）：禁用aclnn算子缓存。原因是缓存key不包含tensor数据的内存地址，无法区分“x与y同址”和“x与y不同址”这两种调用，而这两种调用下aclnn算子的执行逻辑并不相同，同址时复用按不同址场景生成的缓存会与预期不符。
  - `inplaceFlag`为true：不禁用aclnn算子缓存。调用方已确定x、y为原地关系，不存在同址与不同址混用的场景，缓存key不再有上述歧义，可避免原地场景下不必要的缓存失效。

## 调用示例

```cpp
// 标准写法，创建OpExecutor
auto uniqueExecutor = CREATE_EXECUTOR();

// 如果出参out是非连续Tensor，需要把计算完的连续Tensor转非连续
auto viewCopyResult = l0op::ViewCopy(absResult, out, uniqueExecutor.get());

// 原地场景：x和y指向同一块内存，将inplaceFlag设置为true
auto viewCopyInplaceResult = l0op::ViewCopy(selfRef, selfRef, uniqueExecutor.get(), true);
```
