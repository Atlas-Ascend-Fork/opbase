# AicpuTaskSpace Class

Manages the logic related to AI CPU task reuse, including creating and searching for a task.

The definition is as follows:

```cpp
class AicpuTaskSpace {
public:
    AicpuTaskSpace(const std::string &opType,
                   const ge::UnknowShapeOpType unknownType = ge::DEPEND_IN_SHAPE,
                   const bool isTf = false)
        : opType_(opType), unknownType_(unknownType), isTf_(isTf) {}
    AicpuTask *FindTask(aclOpExecutor *executor, op::OpArgContext *args,
                        const FVector<const aclTensor *> &inputs);
    AicpuTask *GetOrCreateTask(aclOpExecutor *executor, const FVector<std::string> &attrNames,
                               op::OpArgContext *args);
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

## Constructor Parameters

| Parameter Name | Type | Requirement and Default Value | Meaning and Constraints |
| --- | --- | --- | --- |
| `opType` | `const std::string &` | Required; no default value. | Operator type name managed by this task space. Must match the target operator. |
| `unknownType` | `const ge::UnknowShapeOpType` | Optional; defaults to `ge::DEPEND_IN_SHAPE`. | Dynamic-shape processing category of the operator. |
| `isTf` | `const bool` | Optional; defaults to `false`. | `true` selects the TensorFlow path; `false` selects the CANN path. |

For details about the members in this class, see the following table.

**Table 1** AicpuTaskSpace class members

| Attribute | Type | Default Value | Description |
| --- | --- | --- | --- |
| `kHashSeed` | `static constexpr uint64_t` | `0x9e3779b9U` | Key value seed of the hash table that stores the map of a task. |
| `opType_` | `const std::string` | Initialized from constructor parameter `opType`; no default value. | Operator name. |
| `unknownType_` | `const ge::UnknowShapeOpType` | Initialized from constructor parameter `unknownType`; `ge::DEPEND_IN_SHAPE` if the argument is omitted. | Dynamic-shape processing category of the operator. |
| `isTf_` | `const bool` | Initialized from constructor parameter `isTf`; `false` if the argument is omitted. | Whether a third-party framework or CANN framework is used.<br>  - `true`: A third-party operator framework is used. Currently, only the TensorFlow framework is supported.<br>  - `false`: The CANN framework is used. |
| `hasInit_` | `bool` | `false` | Whether a task is initialized. |
| `inputRefIndexes_` | `std::set<size_t>` | Empty set. | Set of input indices marked as reference types (ref). |
| `outputRefIndexes_` | `std::set<size_t>` | Empty set. | Set of output indices marked as reference types (ref). |
| `mutex_` | `std::mutex` | Default-constructed and initially unlocked. | Mutex of the task map. |
| `hashMap_` | `HashMap` | Empty container. | Hash table that stores a task. |

## Member Function Parameters

All parameters below must be supplied explicitly and have no default value, except for `isInput`. Indices start at 0.

| Parameter Name | Type | Required | Value Range | Default Value | Meaning and Constraints |
| --- | --- | --- | --- | --- | --- |
| `executor` | `aclOpExecutor *` | Yes | Valid non-null executor pointer. | None | Used to expand argument lists and update the device cache budget. |
| `args` | `op::OpArgContext *` | Yes | Valid non-null argument context pointer. | None | Contains inputs, outputs, and attributes for list expansion and task key generation. The context is borrowed; ownership is not transferred. |
| `inputs` | `const FVector<const aclTensor *> &` | Yes | Valid input list; null input slots are allowed. | None | Input data types and view formats contribute to the task key. Null input slots are skipped. |
| `attrNames` | `const FVector<std::string> &` | Yes | Names corresponding one-to-one to the attribute list; an empty container if there are no attributes. | None | The count and order must match the attribute list in `args`. |
| `hasWorkspaceInfo` | `bool` | Yes | `true` or `false`. | None | Whether the task layout includes a workspace extension record. The lookup layout must match the creation layout. On the CANN path, `true` requires the corresponding workspace arguments. |
| `index` | `const size_t` | Yes | Valid index in the selected input or output list, starting at 0. | None | `isInput` selects the list. The caller must ensure the index is valid; the methods do not check the upper bound. Call `SetRef` before creating the first task; the reference sets are no longer updated after `hasInit_` becomes `true`. |
| `isInput` | `const bool` | No | `true` or `false`. | `true` | `true` selects inputs; `false` selects outputs. |
| `inputs` | `const FVector<const aclTensor *> &` | Yes | Valid input list; null input slots are allowed. | None | Computes host input sizes from the storage shape and aligns each size separately. Null pointers and non-host inputs are skipped. |
| `alignBytes` | `size_t` | Yes | Greater than 0 for a meaningful size calculation; the task paths use 4 or 64. | None | Alignment granularity in bytes. Passing 0 makes the current implementation return a total size of 0. |
| `inputs` | `const FVector<const aclTensor *> &` | Yes | Valid input list; null input slots are allowed. | None | Used to calculate the device cache size required by the inputs. |
| `aicpuTask` | `std::unique_ptr<AicpuTask> &` | Yes | Reference to a smart pointer owning a valid, initialized task. | None | Used to read the device extended information size and alignment granularity. Ownership is not transferred. |

`Clear()` has no parameters.
