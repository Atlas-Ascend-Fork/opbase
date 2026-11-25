# AicpuTaskSpace类

AicpuTaskSpace类用于管理AI CPU task复用相关的逻辑，包括新建task、查找task等功能。

具体定义如下：

```cpp
class AicpuTaskSpace {
public:
    AicpuTaskSpace(const std::string &opType,
                   const ge::UnknowShapeOpType unknownType = ge::DEPEND_IN_SHAPE,
                   const bool isTf = false)
        : opType_(opType), unknownType_(unknownType), isTf_(isTf) {}
    AicpuTask *FindTask(aclOpExecutor *executor, op::OpArgContext *args,
                        const FVector<const aclTensor *> &inputs);
    AicpuTask* FindTaskImpl(aclOpExecutor* executor, op::OpArgContext* args, const FVector<const aclTensor*>& inputs,
                            bool hasWorkspaceInfo);
    AicpuTask *GetOrCreateTask(aclOpExecutor *executor, const FVector<std::string> &attrNames,
                               op::OpArgContext *args);
    AicpuTask* GetOrCreateTaskImpl(aclOpExecutor* executor, const FVector<std::string>& attrNames,
                                   op::OpArgContext* args, bool hasWorkspaceInfo);
    void SetRef(const size_t index, const bool isInput = true);
    bool IsRef(const size_t index, const bool isInput = true) const;
    uint64_t CalcHostInputDataSize(const FVector<const aclTensor *> &inputs, size_t alignBytes) const;
    uint64_t CalcDeviceCacheSize(const FVector<const aclTensor *> &inputs,
                                 std::unique_ptr<AicpuTask> &aicpuTask) const;
    void Clear()
    {
        hashMap_.clear();
    }
    friend class AicpuTask;
private:
    static constexpr uint64_t kHashSeed = 0x9e3779b9U;
    static size_t GenHashBinary(const uint8_t *addr, uint32_t len);
    size_t GenTaskKey(uint8_t inputKey[], size_t &keyLen, op::OpArgContext *args,
                      const FVector<const aclTensor *> &inputs) const;
    const std::string opType_;
    const ge::UnknowShapeOpType unknownType_;
    const bool isTf_;
    bool hasInit_ = false;
    std::set<size_t> inputRefIndexes_;
    std::set<size_t> outputRefIndexes_;
    std::mutex mutex_;
    using HashMap = std::unordered_map<size_t, std::vector<std::unique_ptr<AicpuTask>>>;
    HashMap hashMap_;
};
```

## 构造函数参数说明

| 参数名 | 类型 | 必填性与默认值 | 含义与约束 |
| --- | --- | --- | --- |
| `opType` | `const std::string &` | 必填，无默认值 | 此任务空间对应的算子类型名称，应与目标算子一致。 |
| `unknownType` | `const ge::UnknowShapeOpType` | 可省略，默认 `ge::DEPEND_IN_SHAPE` | 算子动态形状处理类别。 |
| `isTf` | `const bool` | 可省略，默认 `false` | `true` 使用 TensorFlow 路径，`false` 使用 CANN 路径。 |

类成员属性的详细介绍请参考下表。

**表 1** AicpuTaskSpace类成员说明

| 属性名 | 属性类型 | 默认值 | 属性说明 |
| --- | --- | --- | --- |
| `kHashSeed` | `static constexpr uint64_t` | `0x9e3779b9U` | 存储task的map的哈希表的键值种子。 |
| `opType_` | `const std::string` | 由构造参数 `opType` 初始化，无默认值 | 算子名称。 |
| `unknownType_` | `const ge::UnknowShapeOpType` | 由构造参数 `unknownType` 初始化；省略该实参时为 `ge::DEPEND_IN_SHAPE` | 算子的动态形状处理类别。 |
| `isTf_` | `const bool` | 由构造参数 `isTf` 初始化；省略该实参时为 `false` | 标识是执行第三方算子框架还是CANN算子框架。<br>  - 取值true时：采用第三方算子框架，当前仅支持TensorFlow框架。<br>  - 取值false时：采用CANN算子框架。 |
| `hasInit_` | `bool` | `false` | 标识task是否被初始化。 |
| `inputRefIndexes_` | `std::set<size_t>` | 空集合 | 存储被标记为引用类型（ref）的输入索引的集合。 |
| `outputRefIndexes_` | `std::set<size_t>` | 空集合 | 存储被标记为引用类型（ref）的输出索引的集合。 |
| `mutex_` | `std::mutex` | 默认构造，初始未加锁 | task map的锁。 |
| `hashMap_` | `HashMap` | 空容器 | 存储task的哈希表。 |

## 成员函数参数说明

除 `isInput` 外，下列参数均需显式提供，无默认值；索引从 0 开始。

| 参数名 | 类型 | 必填性 | 取值范围 | 默认值 | 含义与约束限制 |
| --- | --- | --- | --- | --- | --- |
| `executor` | `aclOpExecutor *` | 必填 | 有效非空执行器指针 | 无 | 供列表展开及设备缓存预算更新使用。 |
| `args` | `op::OpArgContext *` | 必填 | 有效非空参数上下文指针 | 无 | 上下文包含输入、输出及属性，供列表展开和任务键生成使用；接口借用该对象，不转移所有权。 |
| `inputs` | `const FVector<const aclTensor *> &` | 必填 | 有效输入列表，可含空输入槽位 | 无 | 输入的数据类型和 view 格式参与任务键生成；空输入槽位会跳过。 |
| `attrNames` | `const FVector<std::string> &` | 必填 | 与属性列表一一对应的名称列表，无属性时为空容器 | 无 | 数量和顺序应与 `args` 中的属性列表一致。 |
| `hasWorkspaceInfo` | `bool` | 必填 | `true`、`false` | 无 | 表示是否使用带 workspace 扩展记录的任务布局，查找须与创建时一致；CANN 路径为 `true` 时应提供对应 workspace 参数。 |
| `index` | `const size_t` | 必填 | 对应输入或输出列表的有效索引，从 0 开始 | 无 | 由 `isInput` 选择所属列表，调用方保证索引有效，方法本身不检查上界。`SetRef` 应在首次任务创建前调用，`hasInit_` 为 `true` 后不再修改引用集合。 |
| `isInput` | `const bool` | 可省略 | `true`、`false` | `true` | `true` 选择输入，`false` 选择输出。 |
| `inputs` | `const FVector<const aclTensor *> &` | 必填 | 有效输入列表，可含空输入槽位 | 无 | 按 storage shape 统计 host 输入的字节数并分别对齐，跳过空指针和非 host 输入。 |
| `alignBytes` | `size_t` | 必填 | 有效容量计算应大于 0，实际任务路径取 4 或 64 | 无 | 表示对齐粒度，单位为字节；传 0 时当前实现统计值为 0。 |
| `inputs` | `const FVector<const aclTensor *> &` | 必填 | 有效输入列表，可含空输入槽位 | 无 | 用于统计输入所需的设备缓存大小。 |
| `aicpuTask` | `std::unique_ptr<AicpuTask> &` | 必填 | 持有有效且已初始化任务的智能指针引用 | 无 | 用于读取设备扩展信息长度和对齐粒度；不转移任务所有权。 |

`Clear()` 无参数。
