# aicpu\_task

本章接口为预留接口，后续有可能变更或废弃，不建议开发者使用，开发者无需关注。

**表 1** 接口列表

| 接口定义 | 功能说明 |
| --- | --- |
| bool RegisterAicpuWorkspaceTilingByName(const char* opName, AicpuWorkspaceTilingFunc tiling) | 按算子类型名称注册workspace大小计算回调，位于op::internal命名空间，是[REGISTER_AICPU_WORKSPACE_TILING](../common_macros_and_classes/REGISTER_AICPU_WORKSPACE_TILING.md)的底层注册入口，算子开发者通过该宏注册即可。opName须为非空字符串，且与创建AICPU执行任务时使用的算子类型名称一致；tiling须为非空的AicpuWorkspaceTilingFunc回调。<br>成功返回true；同名重复注册保留首次回调并返回true。参数无效或捕获到内存分配失败时返回false。本函数不执行回调、不申请设备内存；须在首次创建该算子的执行任务前完成注册。 |
| PrintAicpuAllTimeStampInfo(const char *opType) | 打印AI CPU task执行时的系统时间戳。 |
| AppendTensor(aclOpExecutor *executor, const aclTensor*arg, V &l) | 将aclTensor指针塞入到容器中的模板函数。 |
| AppendTensor(aclOpExecutor *executor, const aclScalar*arg, V &l) | 将aclScalar指针塞入到容器中的模板函数。 |
| AppendTensor(aclOpExecutor *executor, const aclIntArray*arg, V &l) | 将aclIntArray指针塞入到容器中的模板函数。 |
| AppendTensor(aclOpExecutor *executor, const aclTensorList*arg, V &l) | 将aclTensorList指针塞入到容器中的模板函数。 |
| CreateTensorListImpl(aclOpExecutor *executor, OpArg &arg, TensorList &l) | 从输入的arg中创建Tensor列表的模板函数。 |
| CreateTensorList(aclOpExecutor *executor, OpArgList &t, TensorList &l) | 创建Tensor列表。 |
| Append1Byte(uint8_t *buf, uint8_t src) | 将一字节追加到指定的缓冲区，并返回下一个写入位置。 |
| AppendAttrForKey(const V &value, uint8_t *&key, size_t &keyLen) | 向task key字段中拼接属性信息的模板函数。 |
| AppendAttrForKey(const std::string &value, uint8_t *&key, size_t &keyLen) | 向task key字段中拼接const string &属性信息的函数。 |
| AppendAttrForKey(const std::string *value, uint8_t *&key, size_t &keyLen) | 向task key字段中拼接const string指针属性信息的函数。 |
| AppendAttrForKey(std::string *value, uint8_t *&key, size_t &keyLen) | 向task key字段中拼接string指针属性信息的函数。 |
| AppendAttrForKey(const std::vector\<V\> &value, uint8_t *&key, size_t &keyLen) | 向task key字段中拼接vector属性信息的函数。 |
| AppendAttrForKey(const aclIntArray *value, uint8_t *&key, size_t &keyLen) | 向task key字段中拼接const aclIntArray属性信息的函数。 |
| AppendAttrForKey(aclIntArray *value, uint8_t *&key, size_t &keyLen) | 向task key字段中拼接aclIntArray属性信息的函数。 |
| AppendAttrForKey(const aclFloatArray *value, uint8_t *&key, size_t &keyLen) | 向task key字段中拼接const aclFloatArray属性信息的函数。 |
| AppendAttrForKey(aclFloatArray *value, uint8_t *&key, size_t &keyLen) | 向task key字段中拼接aclFloatArray属性信息的函数。 |
| AppendAttrForKey(const aclBoolArray *value, uint8_t *&key, size_t &keyLen) | 向task key字段中拼接const aclBoolArray属性信息的函数。 |
| AppendAttrForKey(aclBoolArray *value, uint8_t *&key, size_t &keyLen) | 向task key字段中拼接aclBoolArray属性信息的函数。 |
| AddAicpuAttr(const aclIntArray *value, const std::string &attrName, AicpuAttrs &attrs) | 添加AI CPU属性字段。 |
| AddAicpuAttr(aclIntArray *value, const std::string &attrName, AicpuAttrs &attrs) | 添加AI CPU属性字段。 |
| AddAicpuAttr(const aclFloatArray *value, const std::string &attrName, AicpuAttrs &attrs) | 添加AI CPU属性字段。 |
| AddAicpuAttr(aclFloatArray *value, const std::string &attrName, AicpuAttrs &attrs) | 添加AI CPU属性字段。 |
| AddAicpuAttr(const aclBoolArray *value, const std::string &attrName, AicpuAttrs &attrs) | 添加AI CPU属性字段。 |
| AddAicpuAttr(aclBoolArray *value, const std::string &attrName, AicpuAttrs &attrs) | 添加AI CPU属性字段。 |
| AddAicpuAttr(const V &value, const std::string &attrName, AicpuAttrs &attrs) | 向AI CPU属性map中加入简单数据类型属性。 |
| GetTid() | 获取当前工作的线程id。 |
| aclnnAicpuFinalize() | AI CPU模块去初始化函数。 |
| CreateAicpuKernelLauncher(uint32_t opType, op::internal::AicpuTaskSpace &space, aclOpExecutor *executor, const FVector\<std::string\> &attrNames, op::OpArgContext*args) | 创建AI CPU task下发对象。 |
| AicpuTask() | AicpuTask类默认的构造函数。 |
| AicpuTask(const std::string &opType, const ge::UnknowShapeOpType unknownType) | AicpuTask类的构造函数（带入参）。 |
| AicpuTfTask(const std::string &opType, const ge::UnknowShapeOpType unknownType) | AI CPU TensorFlow算子框架task构造函数。 |
| aclnnStatus Init(const FVector<const aclTensor *> &inputs, const FVector<aclTensor*> &outputs, const AicpuAttrs &attrs) | 算子框架task初始化函数。 |
| aclnnStatus InitImpl(const FVector\<const aclTensor*\>& inputs, const FVector\<aclTensor*\>& outputs, const AicpuAttrs& attrs, bool hasWorkspaceInfo) | AicpuCCTask的初始化方法。AicpuCCTask::Init调用本方法时hasWorkspaceInfo为false。hasWorkspaceInfo为true时添加workspace拓展记录，为false时保持原有记录布局。<br>成功返回ACLNN_SUCCESS，失败返回错误码。该方法不执行tiling回调或分配workspace。 |
| aclnnStatus Run(aclOpExecutor *executor, aclrtStream stream) | 算子框架task执行函数。 |
| AicpuCCTask(const std::string &opType, const ge::UnknowShapeOpType unknownType) | AI CPU CANN算子框架task构造函数。 |
| aclnnStatus SetIoTensors(aclOpExecutor *executor, op::OpArgContext*args) | 刷新task的输入、输出地址。 |
| void SetSpace(void *space) | 设置task所在的容器。 |
| void SetVisit(bool visit) | 设置task是否被占用。 |
| AicpuTaskSpace(const std::string &opType,  const ge::UnknowShapeOpType unknownType = ge::DEPEND_IN_SHAPE,  const bool isTf = false) | AI CPU task容器构造函数。 |
| AicpuTask *FindTask(aclOpExecutor*executor, op::OpArgContext *args, const FVector<const aclTensor*> &inputs) | 查找是否可复用的task。 |
| AicpuTask* FindTaskImpl(aclOpExecutor* executor, op::OpArgContext* args, const FVector\<const aclTensor*\>& inputs, bool hasWorkspaceInfo) | AicpuTaskSpace的方法，查找键匹配且未被占用的可复用task。hasWorkspaceInfo表示是否包含workspace拓展记录，需与创建时一致。<br>命中时将task标记为占用并返回，未命中返回nullptr。 |
| AicpuTask *GetOrCreateTask(aclOpExecutor*executor, const FVector\<std::string\> &attrNames, op::OpArgContext *args) | 获取新建或者复用的task。 |
| AicpuTask* GetOrCreateTaskImpl(aclOpExecutor* executor, const FVector\<std::string\>& attrNames, op::OpArgContext* args, bool hasWorkspaceInfo) | AicpuTaskSpace的方法，获取可复用task或创建task。hasWorkspaceInfo为true时用于带workspace拓展记录的CANN算子task，调用方应在args中提供对应workspace参数。<br>成功返回task，失败返回nullptr。创建时预留记录，执行前由SetIoTensors更新地址和原始字节数。 |
| void SetRef(const size_t index, const bool isInput = true) | 设置指定索引为引用类型输入。 |
| bool IsRef(const size_t index, const bool isInput = true) const | 获取指定索引是否为引用类型输入。 |
| uint64_t CalcHostInputDataSize(const FVector<const aclTensor *> &inputs, size_t alignBytes) const | 获取输入为host侧数据的总大小。 |
| uint64_t CalcDeviceCacheSize(const FVector<const aclTensor *> &inputs, std::unique_ptr\<AicpuTask\> &aicpuTask) const | 计算device侧预留的内存大小。 |
| void Clear() | 清理缓存的task。 |
| static size_t GenHashBinary(const uint8_t *addr, uint32_t len) | 获取task哈希表键值种子。 |
| size_t GenTaskKey(uint8_t inputKey[], size_t &keyLen, op::OpArgContext *args, const FVector<const aclTensor*> &inputs) const | 生成task查找的key。 |

## 参数说明

以下参数说明对应上述接口表。必填表示调用时必须显式提供该形参，指针是否允许为空由具体参数的约束决定。长度、容量及偏移使用字节为单位，索引从 0 开始。

### 回调注册与时间信息打印参数

| 参数名 | 类型 | 必填性 | 取值范围 | 默认值 | 含义与约束限制 |
| --- | --- | --- | --- | --- | --- |
| `opName` | `const char *` | 必填 | 非空指针，指向非空且以空字符结尾的字符串 | 无 | 算子类型名称，须与创建任务时使用的名称一致；应在首次创建任务前完成注册。 |
| `tiling` | `AicpuWorkspaceTilingFunc` | 必填 | 非空回调函数指针 | 无 | workspace 大小计算回调，其所在动态库的有效期须覆盖回调使用周期。回调通过 `uint64_t &workspaceSize` 输出原始字节数，0 表示无需求；回调返回 `aclnnStatus`。 |
| `opType` | `const char *` | 必填 | 有效、以空字符结尾的算子名称字符串 | 无 | `PrintAicpuAllTimeStampInfo` 使用的打印标签。 |

### Tensor 列表构建参数

`AppendTensor`、`CreateTensorListImpl`、`CreateTensorList` 将结果追加到容器，不先清空已有内容。

| 参数名 | 类型 | 必填性 | 取值范围 | 默认值 | 含义与约束限制 |
| --- | --- | --- | --- | --- | --- |
| `executor` | `aclOpExecutor *` | 必填 | Scalar、IntArray 转换时须为有效非空执行器 | 无 | 用于将标量或整数数组转为 host Tensor；直接追加 Tensor 或展开 TensorList 的重载不使用该参数。 |
| `arg` | `const aclTensor *` | 必填 | 有效 Tensor 指针或 `nullptr` | 无 | 直接将 Tensor 指针追加到容器，可保留空输入槽位。 |
| `arg` | `const aclScalar *` | 必填 | 有效非空标量指针 | 无 | 通过执行器按标量的数据类型转换为 host Tensor，再追加转换结果。 |
| `arg` | `const aclIntArray *` | 必填 | 有效非空整数数组指针 | 无 | 通过执行器转换为 `DT_INT64` 的 host Tensor，再追加转换结果。 |
| `arg` | `const aclTensorList *` | 必填 | 有效非空列表指针 | 无 | 按顺序展开并追加列表中的 Tensor 指针。 |
| `arg` | `OpArg &` | 必填 | 类型为 `OPARG_ACLTENSOR`、`OPARG_ACLSCALAR`、`OPARG_INT_LIST` 或 `OPARG_ACLTENSOR_LIST` 的参数 | 无 | `CreateTensorListImpl` 的单个输入参数，内容须满足对应 `AppendTensor` 重载的要求；其他类型记录错误并结束当前分派。 |
| `t` | `OpArgList &` | 必填 | 有效参数列表，可为空列表 | 无 | `CreateTensorList` 遍历的参数列表，各项须满足对应类型的转换要求。 |
| `l` | `V &` | 必填 | 支持 Tensor 指针 `emplace_back` 操作的容器 | 无 | `AppendTensor` 的输入输出容器，接收追加结果。 |
| `l` | `TensorList &` | 必填 | 支持 Tensor 指针追加的容器 | 无 | `CreateTensorListImpl`、`CreateTensorList` 的输入输出容器，接收展开结果。 |

### 属性与键数据追加参数

`Append1Byte` 写入一个字节；`AppendAttrForKey` 编码任务键属性；`AddAicpuAttr` 写入属性映射。

| 参数名 | 类型 | 必填性 | 取值范围 | 默认值 | 含义与约束限制 |
| --- | --- | --- | --- | --- | --- |
| `buf` | `uint8_t *` | 必填 | 至少一个字节可写的有效地址 | 无 | 单字节写入位置，调用方保证缓冲区容量有效；写入后通过返回值取得下一写入位置。 |
| `src` | `uint8_t` | 必填 | 0～255 | 无 | 待追加的单个字节值。 |
| `value` | `const V &`、`const std::string &`、`const std::string *`、`std::string *`、`const std::vector<V> &`、`const aclIntArray *`、`aclIntArray *`、`const aclFloatArray *`、`aclFloatArray *`、`const aclBoolArray *`、`aclBoolArray *` | 必填 | 符合对应重载类型的属性值，指针形式须非空 | 无 | `AppendAttrForKey` 的输入属性值，泛型对象按字节表示追加，字符串及列表按元素追加。 |
| `key` | `uint8_t *&` | 必填 | 指向有效键缓冲区当前可写位置 | 无 | 输入输出参数，追加后向后移动；须与累计长度 `keyLen` 对应。 |
| `keyLen` | `size_t &` | 必填 | 初始值为 0～`kAicpuKeyBufLen` | 无 | 输入输出的累计字节数。基础追加逻辑达到当前 1024 字节容量时停止，不返回截断错误，调用方须控制编码长度。 |
| `value` | `const V &`、`const aclIntArray *`、`aclIntArray *`、`const aclFloatArray *`、`aclFloatArray *`、`const aclBoolArray *`、`aclBoolArray *` | 必填 | 符合对应重载类型的属性值，数组指针须非空 | 无 | `AddAicpuAttr` 的输入属性值，数组转换为相应 vector 后写入属性映射。 |
| `attrName` | `const std::string &` | 必填 | 符合目标算子定义的属性名称 | 无 | 写入 `attrs` 时使用的名称。 |
| `attrs` | `AicpuAttrs &` | 必填 | 有效属性映射，可为空映射 | 无 | 输入输出参数，保存名称到属性值的映射；同名项会覆盖原值。 |

### 任务类构造与公共方法参数

三个任务类构造函数的 `opType`、`unknownType` 均需显式提供；`InitImpl` 的前三个参数与 `Init` 含义相同。

| 参数名 | 类型 | 必填性 | 取值范围 | 默认值 | 含义与约束限制 |
| --- | --- | --- | --- | --- | --- |
| `opType` | `const std::string &` | 必填 | 与目标算子一致的类型名称 | 无 | 任务构造参数，用于参数生成、日志及算子加载。 |
| `unknownType` | `const ge::UnknowShapeOpType` | 必填 | 配套 `graph/types.h` 定义的合法枚举值 | 无 | 算子动态形状处理类别。 |
| `inputs` | `const FVector<const aclTensor *> &` | 必填 | 符合算子定义的输入 Tensor 列表 | 无 | 初始化任务参数描述，次序须与执行前绑定一致；可按算子定义保留空输入槽位。 |
| `outputs` | `const FVector<aclTensor *> &` | 必填 | 元素有效且符合算子定义的输出 Tensor 列表 | 无 | 初始化任务参数描述，次序须与执行前绑定一致。 |
| `attrs` | `const AicpuAttrs &` | 必填 | 符合算子定义的属性映射，可为空映射 | 无 | 初始化所需的算子属性。 |
| `hasWorkspaceInfo` | `bool` | 必填 | `true`、`false` | 无 | `InitImpl` 是否预留 workspace 扩展记录，`Init` 调用该方法时传 `false`；实际地址和原始字节数在执行前刷新。 |
| `executor` | `aclOpExecutor *` | 必填 | 有效非空执行器指针 | 无 | `Run`、`SetIoTensors` 使用的执行器，执行阶段须提供有效的 workspace 地址及执行流。 |
| `stream` | `aclrtStream` | 必填 | 本次执行使用的运行时流 | 无 | `Run` 的执行流，须与执行器的流一致；调用前完成初始化及 `SetIoTensors` 刷新。 |
| `args` | `op::OpArgContext *` | 必填 | 有效非空参数上下文指针 | 无 | `SetIoTensors` 从中展开输入、输出并按需刷新 workspace 记录；本方法借用该对象，不接管所有权。 |
| `space` | `void *` | 必填 | 所属 `AicpuTaskSpace` 的有效地址 | 无 | `SetSpace` 绑定的任务空间，其有效期须覆盖任务使用周期。 |
| `visit` | `bool` | 必填 | `true`、`false` | 无 | `SetVisit` 的占用标记，`true` 表示占用，`false` 表示释放占用。 |

### 任务空间参数

| 参数名 | 类型 | 必填性 | 取值范围 | 默认值 | 含义与约束限制 |
| --- | --- | --- | --- | --- | --- |
| `opType` | `const std::string &` | 必填 | 与目标算子一致的类型名称 | 无 | 此任务空间管理的算子类型。 |
| `unknownType` | `const ge::UnknowShapeOpType` | 可省略 | 配套 `graph/types.h` 定义的合法枚举值 | `ge::DEPEND_IN_SHAPE` | 动态形状处理类别。 |
| `isTf` | `const bool` | 可省略 | `true`、`false` | `false` | `true` 使用 TensorFlow 路径，`false` 使用 CANN 路径。 |
| `executor` | `aclOpExecutor *` | 必填 | 有效非空执行器指针 | 无 | 供查找及创建任务使用，按需更新设备侧 AI CPU 内存预算。 |
| `args` | `op::OpArgContext *` | 必填 | 有效非空参数上下文指针 | 无 | 查找、创建所需的输入、输出及属性上下文，调用方保留其所有权。 |
| `inputs` | `const FVector<const aclTensor *> &` | 必填 | 有效输入列表，可含空输入槽位 | 无 | 查找时数据类型和 view 格式参与键生成；容量统计时只累计 host 输入，跳过空指针和非 host 输入。 |
| `attrNames` | `const FVector<std::string> &` | 必填 | 与属性列表一一对应的名称列表 | 无 | 数量、顺序须与 `args` 中属性列表一致，无属性时为空容器。 |
| `hasWorkspaceInfo` | `bool` | 必填 | `true`、`false` | 无 | `FindTaskImpl`、`GetOrCreateTaskImpl` 使用的扩展布局标记，查找须与创建时一致；CANN 路径为 `true` 时应提供对应 workspace 参数。 |
| `index` | `const size_t` | 必填 | 对应输入或输出列表的有效索引，从 0 开始 | 无 | 引用索引，调用方保证上界有效；引用标记应在首次任务创建前设置。 |
| `isInput` | `const bool` | 可省略 | `true`、`false` | `true` | `true` 选择输入，`false` 选择输出。 |
| `alignBytes` | `size_t` | 必填 | 有效容量计算取大于 0 的值，实际任务路径取 4 或 64 | 无 | host 输入的对齐粒度，单位为字节；传 0 时当前实现统计值为 0。 |
| `aicpuTask` | `std::unique_ptr<AicpuTask> &` | 必填 | 持有有效且已初始化任务的智能指针引用 | 无 | 用于读取设备扩展信息长度及对齐粒度，不转移任务所有权。 |

### 任务键辅助方法参数

`GenHashBinary`、`GenTaskKey` 为 `AicpuTaskSpace` 的私有辅助方法。

| 参数名 | 类型 | 必填性 | 取值范围 | 默认值 | 含义与约束限制 |
| --- | --- | --- | --- | --- | --- |
| `addr` | `const uint8_t *` | 必填 | 至少有 `len` 字节可读的有效地址 | 无 | `GenHashBinary` 的待哈希数据。 |
| `len` | `uint32_t` | 必填 | 有效数据的字节长度 | 无 | 调用方保证长度不超过可读缓冲区。 |
| `inputKey` | `uint8_t[]` | 必填 | 至少 1024 字节的可写缓冲区 | 无 | `GenTaskKey` 的输出键缓冲区，调用方须保证输入及属性编码总量不超过容量。 |
| `keyLen` | `size_t &` | 必填 | 输出的有效键字节数 | 无 | 输出参数，返回键长度；函数本身返回哈希值。 |
| `args` | `op::OpArgContext *` | 必填 | 有效非空参数上下文指针 | 无 | 其中的属性参与键生成，本方法借用上下文。 |
| `inputs` | `const FVector<const aclTensor *> &` | 必填 | 有效输入列表，可含空输入槽位 | 无 | 数据类型及 view 格式参与键生成，空输入槽位会跳过。 |

### 下发器创建参数

`CreatAicpuKernelLauncher` 接收以下参数，名称与当前源码保持一致。

| 参数名 | 类型 | 必填性 | 取值范围 | 默认值 | 含义与约束限制 |
| --- | --- | --- | --- | --- | --- |
| `opType` | `uint32_t` | 必填 | 已登记且满足 `0 < opType < MAX_OP_TYPE_COUNT` 的算子类型 ID | 无 | 当前上界为 1024，须与任务空间的算子类型对应。 |
| `space` | `op::internal::AicpuTaskSpace &` | 必填 | 有效任务空间引用 | 无 | 用于任务查找、创建和复用，有效期须覆盖任务及下发器使用周期。 |
| `executor` | `aclOpExecutor *` | 必填 | 有效非空执行器指针 | 无 | 用于 workspace 规划及下发器登记。 |
| `attrNames` | `const FVector<std::string> &` | 必填 | 与 `args` 属性列表的数量和顺序一致的名称列表 | 无 | 无属性时为空容器。 |
| `args` | `op::OpArgContext *` | 必填 | 框架创建的有效非空参数上下文 | 无 | 输入并转移所有权，调用后由本函数或下发器释放，调用方不得重复释放。 |

`GetTid()`、`aclnnAicpuFinalize()`、`Clear()` 无参数。任务类的调用约定见 [AicpuTask类](AicpuTaskClass.md)，任务空间约束见 [AicpuTaskSpace类](AicpuTaskSpaceClass.md)。

## 调用示例

以下片段假设算子已注册，任务空间、执行器及参数上下文均有效，且属性列表为空，展示下发器创建调用。完整设备执行还需准备运行时环境、输入输出及 workspace。

```cpp
#include "opdev/aicpu/aicpu_task.h"

aclnnStatus EnqueueAicpuTask(
    uint32_t opType, op::internal::AicpuTaskSpace &space,
    aclOpExecutor *executor, op::OpArgContext *args)
{
    const op::FVector<std::string> attrNames{};
    // args 的所有权交给创建函数，调用后不再由调用方释放。
    return op::internal::CreatAicpuKernelLauncher(
        opType, space, executor, attrNames, args);
}
```
