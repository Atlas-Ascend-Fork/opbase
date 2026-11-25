# aicpu\_args\_handler

本章接口为预留接口，后续有可能变更或废弃，不建议开发者使用，开发者无需关注。

**表 1** 接口列表

| 接口定义 | 功能说明 |
| --- | --- |
| AicpuArgsHandler(const std::string &opType, const std::string &nodeName, const uint32_t ioNum, const bool needDeviceExt) | AI CPU框架用于构造AI CPU opTask时的辅助类。 |
| opType_(opType) | 算子名称。 |
| nodeName_(nodeName) | 预留参数，当前同算子名称。 |
| ioNum_(ioNum) | 算子的输入和输出个数之和。 |
| needDeviceExt_(needDeviceExt) | 标识是否需要分配额外device侧内存。 |
| args_({}) | 与Runtime运行时交互的参数对象，用于计算任务下发。 |
| GetIoAddr() | 获取args内存中存放input/output地址的内存指针。 |
| get() | 获取智能指针管理的内存的首地址。 |
| GetExtInfoAddr() | 获取args中存放拓展参数的内存指针。 |
| GetArgs() | 获取指向args内存的指针。 |
| GetArgsEx() | 获取与Runtime运行时交互的对象的指针。 |
| GetNodeName() | 获取节点名称，当前为算子名。 |
| GetIoNum() | 获取算子的输入和输出个数之和。 |
| GetHostInputSize() | 获取host侧输入的内存大小。 |
| GetInputAddrAlignBytes() | 获取指定的地址分配字节对齐参数。 |
| SetSpace(void *space) | 记录此Args handle归属于哪个算子空间。 |
| MallocMem() | 申请args的内存。 |
| ResetHostInputInfo() | 重置host侧输入的统计信息。 |
| AddHostInput(const size_t idx, void *data, const size_t srcSize, const size_t alignSize) | 记录host侧输入信息。 |
| UpdateIoAddr(const FVector<const aclTensor *> &inputs, const FVector<aclTensor*> &outputs, const aclrtStream stream, aclOpExecutor *executor, const uint64_t deviceExtMemSize, const uint64_t deviceCacheOffset) | 更新args中的input、output的地址信息。 |
| AicpuArgsHandler() | AI CPU框架用于构造AI CPU opTask时的辅助类。 |
| UpdateDeviceExtInfoAddr(void *deviceExtInfoAddr) | 更新device侧存储拓展参数的地址。 |
| SetLaunchArgs(const size_t argSize) | 设置args中参数大小。 |
| GetDeviceCacheAddr(void *&deviceAddr, aclOpExecutor *executor, const uint64_t deviceCacheOffset) | 获取预留的device侧内存地址。 |
| AicpuCCArgsHandler(const std::string &opType, const std::string &nodeName, const uint32_t ioNum, const bool needDeviceExt) | AI CPU CANN算子框架参数管理类。 |
| GenCCArgs(const FVector<const aclTensor *> &inputs, const FVector<aclTensor*> &outputs, const AicpuAttrs &attrs, std::string &taskInfo) | 生成CANN算子args中的AicpuParamHead、IO 地址及nodedef信息。 |
| BuildCCArgs(const std::string &argData, const std::string &kernelName, const std::string &soName, const size_t extInfoSize) | 封装CANN算子args中参数。 |
| SetHostArgs(const std::string &argData, const size_t extInfoSize) | 设置args中内存的地址和长度信息。 |
| SetOffsetArgs() | 设置args中偏移值。 |
| AicpuTfArgsHandler(const std::string &opType, const std::string &nodeName, const uint32_t ioNum, const bool needDeviceExt) | AI CPU TensorFlow算子框架参数管理类。 |
| GenTfArgs(const FVector<const aclTensor *> &inputs, const FVector<aclTensor*> &outputs, const AicpuAttrs &attrs, STR_FWK_OP_KERNEL &fwkOpKernel, std::string &taskInfo) | 生成TensorFlow算子args中的KernelRunParam及nodedef信息，并填充STR_FWK_OP_KERNEL。 |
| BuildTfArgs(STR_FWK_OP_KERNEL &fwkOpKernel, const std::string &taskInfo, const size_t extInfoSize) | 封装Tensorflow算子args中参数。 |
| GenNodeDef(const FVector<const aclTensor *> &inputs, const AicpuAttrs &attrs, ge::Buffer &nodeDefBytes) | 生成算子的描述符信息，包含输入、输出shape及属性等信息。 |
| UpdateKernelId() | 更新args中的Kernel字段，用于匹配device侧的Kernel Cache。 |

## 参数说明

### 构造参数

三个类的构造参数含义相同，均为输入参数、必填且无默认值。

| 名称 | 类型 | 必填性 | 取值范围 | 单位 | 约束说明 |
| --- | --- | --- | --- | --- | --- |
| `opType` | `const std::string &` | 必填 | 与目标算子一致的类型名称 | 无 | 算子类型名称；构造函数本身不检查字符串是否为空。 |
| `nodeName` | `const std::string &` | 必填 | 节点名称字符串 | 无 | 当前任务初始化路径传入与 `opType` 相同的值，构造函数本身不执行相等性校验。 |
| `ioNum` | `const uint32_t` | 必填 | 0～UINT32_MAX | 个 | 输入、输出地址槽位总数，应与后续输入输出列表及序列化参数一致。 |
| `needDeviceExt` | `const bool` | 必填 | `true`、`false` | 无 | 是否使用独立的设备扩展信息内存。当前任务路径在动态形状处理类别为 `ge::DEPEND_SHAPE_RANGE` 时传 `true`，否则传 `false`。 |

### host 输入参数

`AddHostInput` 的参数均为输入参数、必填且无默认值。

| 名称 | 类型 | 必填性 | 取值范围 | 单位 | 约束说明 |
| --- | --- | --- | --- | --- | --- |
| `idx` | `const size_t` | 必填 | 当前输入列表的有效索引，从 0 开始且小于 `ioNum_` | 无 | 输入地址槽位索引，调用方保证索引有效；方法本身不检查索引范围。 |
| `data` | `void *` | 必填 | `srcSize > 0` 时为有效源地址，`srcSize == 0` 时可为空 | 无 | host 侧源数据地址，须至少有 `srcSize` 个可读字节；长度为 0 时跳过复制。 |
| `srcSize` | `const size_t` | 必填 | 0～`alignSize` | 字节 | 实际复制的原始数据长度，应与源数据一致。 |
| `alignSize` | `const size_t` | 必填 | 不小于 `srcSize`，且累计对齐后长度不超过 1024 | 字节 | 按对齐粒度向上取整后的数据长度。当前调用路径使用 `RoundUp(srcSize, GetInputAddrAlignBytes())`，它不是对齐粒度本身。 |

调用前应完成缓冲区分配和偏移设置，调用方保证索引、源数据及各长度关系有效。累计对齐后长度须满足 `hostInputSize_ + alignSize <= 1024`，单位为字节；对齐粒度由 `GetInputAddrAlignBytes()` 获取，CANN 路径初始为 4 字节，TensorFlow 参数构建后为 64 字节。

### 地址更新与任务空间参数

以下参数均需显式提供，无默认值；长度和偏移的单位为字节。

| 名称 | 类型 | 必填性 | 取值范围 | 单位 | 约束说明 |
| --- | --- | --- | --- | --- | --- |
| `SetSpace.space` | `void *` | 必填 | 有效任务空间地址 | 无 | 输入所属 `AicpuTaskSpace` 的地址，空间有效期应覆盖任务使用周期。TensorFlow 参数生成会通过该地址读取引用标记，生成前应绑定有效空间。 |
| `UpdateIoAddr.inputs` | `const FVector<const aclTensor *> &` | 必填 | 有效输入列表，可含空输入槽位 | 无 | 本次执行的输入列表，次序与构建时一致；空输入指针写为地址 0，host 输入走复制或设备缓存路径。 |
| `UpdateIoAddr.outputs` | `const FVector<aclTensor *> &` | 必填 | 元素有效且位置为 `kOnDeviceHbm` 的输出列表 | 无 | 本次执行的输出列表，次序与构建时一致；非设备输出返回内部错误。 |
| `UpdateIoAddr.stream` | `const aclrtStream` | 必填 | 本次执行使用的有效运行时流 | 无 | 异步复制使用的执行流，应与执行器及后续任务下发的流一致。 |
| `UpdateIoAddr.executor` | `aclOpExecutor *` | 必填 | 有效非空执行器指针 | 无 | 用于获取 workspace 基址及设备缓存预算。 |
| `UpdateIoAddr.deviceExtMemSize` | `const uint64_t` | 必填 | 0～执行器的设备侧 AI CPU 内存预算 | 字节 | 已预留的设备扩展信息长度。 |
| `UpdateIoAddr.deviceCacheOffset` | `const uint64_t` | 必填 | workspace 预留设备缓存区域内的有效偏移 | 字节 | 设备侧 host 输入缓存起始位置相对 workspace 基址的偏移，由执行前的扩展信息刷新流程确定。 |
| `GetDeviceCacheAddr.deviceAddr` | `void *&` | 必填 | 由方法写入的有效设备缓存地址 | 无 | 输出参数，调用方通过引用接收本次可用设备缓存地址。 |
| `GetDeviceCacheAddr.executor` | `aclOpExecutor *` | 必填 | 有效非空执行器指针 | 无 | workspace 已分配且预留区域足以容纳本次数据。 |
| `GetDeviceCacheAddr.deviceCacheOffset` | `const uint64_t` | 必填 | workspace 预留设备缓存区域内的有效偏移 | 字节 | 首次建立设备缓存基址时使用的 workspace 偏移；返回地址还叠加已使用的 `deviceCacheSize_`。 |
| `UpdateDeviceExtInfoAddr.deviceExtInfoAddr` | `void *` | 必填 | 需要独立设备扩展信息时为有效地址，其他路径可为空 | 无 | 输入设备扩展信息地址；其他路径使用运行时占位记录。 |

### 参数生成与构建参数

以下参数均需显式提供，无默认值。

| 名称 | 类型 | 必填性 | 取值范围 | 单位 | 约束说明 |
| --- | --- | --- | --- | --- | --- |
| `GenCCArgs/GenTfArgs.inputs` | `const FVector<const aclTensor *> &` | 必填 | 与目标算子定义一致的输入 Tensor 列表 | 无 | 用于参数描述生成，顺序与后续执行一致。 |
| `GenCCArgs/GenTfArgs.outputs` | `const FVector<aclTensor *> &` | 必填 | 与目标算子定义一致的输出 Tensor 列表 | 无 | 用于参数描述生成，顺序与后续执行一致。 |
| `GenCCArgs/GenTfArgs.attrs` | `const AicpuAttrs &` | 必填 | 符合算子定义的属性映射，可为空 | 无 | 输入属性映射，无属性时为空映射。 |
| `GenCCArgs/GenTfArgs.taskInfo` | `std::string &` | 必填 | 可追加二进制任务参数的字符串 | 无 | 输入输出参数，追加生成的二进制任务参数；当前构建路径传入空字符串，方法不先清空已有内容。 |
| `GenTfArgs.fwkOpKernel` | `STR_FWK_OP_KERNEL &` | 必填 | 可写的 TensorFlow 框架下发结构 | 无 | 输入输出参数，填写框架下发结构，供 `BuildTfArgs` 继续使用。 |
| `BuildCCArgs/SetHostArgs.argData` | `const std::string &` | 必填 | 由 CANN 生成路径产生的二进制参数 | 无 | 输入参数，参数头中的 IO 数量应与 `ioNum_` 一致。 |
| `BuildCCArgs.kernelName` | `const std::string &` | 必填 | 算子加载、注册流程提供的函数名称 | 无 | 输入待下发的 kernel 函数名称。 |
| `BuildCCArgs.soName` | `const std::string &` | 必填 | 算子加载流程提供的共享库名称 | 无 | 输入共享库名称，实现对自定义算子库保留特殊处理。 |
| `BuildCCArgs/SetHostArgs/BuildTfArgs.extInfoSize` | `const size_t` | 必填 | 实际扩展信息的序列化长度 | 字节 | 输入参数，须与实际扩展信息长度一致。 |
| `BuildTfArgs.fwkOpKernel` | `STR_FWK_OP_KERNEL &` | 必填 | 由 `GenTfArgs` 填充的框架下发结构 | 无 | 输入输出参数，继续更新扩展长度、kernel ID 等字段并复制入缓冲区。 |
| `BuildTfArgs.taskInfo` | `const std::string &` | 必填 | 由 `GenTfArgs` 生成的二进制任务参数 | 无 | 输入构建 TensorFlow 参数缓冲区所需的任务参数。 |
