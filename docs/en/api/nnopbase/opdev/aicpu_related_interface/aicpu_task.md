# aicpu\_task

The APIs described in this section are reserved and may be changed or deprecated in the future. Therefore, you are not advised to pay attention to or use these APIs.

**Table 1** API list

| API Definition| Description|
| --- | --- |
| PrintAicpuAllTimeStampInfo(const char *opType) | Prints the system timestamp when an AI CPU task is executed.|
| AppendTensor(aclOpExecutor *executor, const aclTensor*arg, V &l) | Inserts an `aclTensor` pointer into a container. This is a template function.|
| AppendTensor(aclOpExecutor *executor, const aclScalar*arg, V &l) | Inserts an `aclScalar` pointer into a container. This is a template function.|
| AppendTensor(aclOpExecutor *executor, const aclIntArray*arg, V &l) | Inserts an `aclIntArray` pointer into a container. This is a template function.|
| AppendTensor(aclOpExecutor *executor, const aclTensorList*arg, V &l) | Inserts an `aclTensorList` pointer into a container. This is a template function.|
| CreateTensorListImpl(aclOpExecutor *executor, OpArg &arg, TensorList &l) | Creates a tensor list from input `arg`. This is a template function.|
| CreateTensorList(aclOpExecutor *executor, OpArgList &t, TensorList &l) | Creates a tensor list.|
| Append1Byte(uint8_t *buf, uint8_t src) | Appends one byte to the specified buffer and returns the next write position.|
| AppendAttrForKey(const V &value, uint8_t *&key, size_t &keyLen) | Appends attribute information to the task key field. This is a template function.|
| AppendAttrForKey(const std::string &value, uint8_t *&key, size_t &keyLen) | Appends `const string &` attribute information to the task key field.|
| AppendAttrForKey(const std::string *value, uint8_t*&key, size_t &keyLen) | Appends `const string` pointer attribute information to the task key field.|
| AppendAttrForKey(std::string *value, uint8_t*&key, size_t &keyLen) | Appends `string` pointer attribute information to the task key field.|
| AppendAttrForKey(const std::vector\<V\> &value, uint8_t *&key, size_t &keyLen) | Appends `vector` attribute information to the task key field.|
| AppendAttrForKey(const aclIntArray *value, uint8_t*&key, size_t &keyLen) | Appends `const aclIntArray` attribute information to the task key field.|
| AppendAttrForKey(aclIntArray *value, uint8_t*&key, size_t &keyLen) | Appends `aclIntArray` attribute information to the task key field.|
| AppendAttrForKey(const aclFloatArray *value, uint8_t*&key, size_t &keyLen) | Appends `const aclFloatArray` attribute information to the task key field.|
| AppendAttrForKey(aclFloatArray *value, uint8_t*&key, size_t &keyLen) | Appends `aclFloatArray` attribute information to the task key field.|
| AppendAttrForKey(const aclBoolArray *value, uint8_t*&key, size_t &keyLen) | Appends `const aclBoolArray` attribute information to the task key field.|
| AppendAttrForKey(aclBoolArray *value, uint8_t*&key, size_t &keyLen) | Appends `aclBoolArray` attribute information to the task key field.|
| AddAicpuAttr(const aclIntArray *value, const std::string &attrName, AicpuAttrs &attrs) | Adds an AI CPU attribute field.|
| AddAicpuAttr(aclIntArray *value, const std::string &attrName, AicpuAttrs &attrs) | Adds an AI CPU attribute field.|
| AddAicpuAttr(const aclFloatArray *value, const std::string &attrName, AicpuAttrs &attrs) | Adds an AI CPU attribute field.|
| AddAicpuAttr(aclFloatArray *value, const std::string &attrName, AicpuAttrs &attrs) | Adds an AI CPU attribute field.|
| AddAicpuAttr(const aclBoolArray *value, const std::string &attrName, AicpuAttrs &attrs) | Adds an AI CPU attribute field.|
| AddAicpuAttr(aclBoolArray *value, const std::string &attrName, AicpuAttrs &attrs) | Adds an AI CPU attribute field.|
| AddAicpuAttr(const V &value, const std::string &attrName, AicpuAttrs &attrs) | Adds a simple data type attribute into the AI CPU attribute map.|
| GetTid() | Obtains the ID of the current working thread.|
| aclnnAicpuFinalize() | Deinitializes the AI CPU module.|
| CreatAicpuKernelLauncher(uint32_t opType, op::internal::AicpuTaskSpace &space, aclOpExecutor *executor, const FVector\<std::string\> &attrNames, op::OpArgContext*args) | Creates an AI CPU task delivery object.|
| AicpuTask() | Constructs a default instance of the `AicpuTask` class.|
| AicpuTask(const std::string &opType, const ge::UnknowShapeOpType unknownType) | Constructs an instance of the `AicpuTask` class (with input parameters).|
| AicpuTfTask(const std::string &opType, const ge::UnknowShapeOpType unknownType) | Constructs a TensorFlow operator task for the AI CPU framework.|
| aclnnStatus Init(const FVector<const aclTensor *> &inputs, const FVector<aclTensor*> &outputs, const AicpuAttrs &attrs) | Initializes an operator framework task.|
| aclnnStatus Run(aclOpExecutor *executor, aclrtStream stream) | Runs an operator framework task.|
| AicpuCCTask(const std::string &opType, const ge::UnknowShapeOpType unknownType) | Constructs a CANN operator task for the AI CPU framework.|
| aclnnStatus SetIoTensors(aclOpExecutor *executor, op::OpArgContext*args) | Refreshes the input and output addresses of a task.|
| void SetSpace(void *space) | Sets the container where a task is located.|
| void SetVisit(bool visit) | Sets whether a task is occupied.|
| AicpuTaskSpace(const std::string &opType,  const ge::UnknowShapeOpType unknownType = ge::DEPEND_IN_SHAPE,  const bool isTf = false) | Constructs an AI CPU task container.|
| AicpuTask *FindTask(aclOpExecutor*executor, op::OpArgContext *args, const FVector<const aclTensor*> &inputs) | Checks if a task can be reused.|
| AicpuTask *GetOrCreateTask(aclOpExecutor*executor, const FVector\<std::string\> &attrNames, op::OpArgContext *args) | Obtains a new or reused task.|
| void SetRef(const size_t index, const bool isInput = true) | Sets the specified index as a reference-type input.|
| bool IsRef(const size_t index, const bool isInput = true) const | Checks if the specified index is a reference-type input.|
| uint64_t CalcHostInputDataSize(const FVector<const aclTensor *> &inputs, size_t alignBytes) const | Calculates the total size of input data stored on the host side.|
| uint64_t CalcDeviceCacheSize(const FVector<const aclTensor *> &inputs, std::unique_ptr\<AicpuTask\> &aicpuTask) const | Calculates the size of the memory reserved on the device side.|
| void Clear() | Clears cached tasks.|
| static size_t GenHashBinary(const uint8_t *addr, uint32_t len) | Generates the hash key seed for a task.|
| size_t GenTaskKey(uint8_t inputKey[], size_t &keyLen, op::OpArgContext *args, const FVector<const aclTensor*> &inputs) const | Generates a key for task search.|

## Parameters

The following tables describe the API parameters. A required parameter must be supplied explicitly; whether a pointer may be null depends on its specific constraints. Sizes, capacities, and offsets are measured in bytes. Indices start at 0.

### Callback Registration and Timing Information Parameters

| Parameter Name | Type | Required | Value Range | Default Value | Meaning and Constraints |
| --- | --- | --- | --- | --- | --- |
| `opName` | `const char *` | Yes | Non-null pointer to a nonempty, null-terminated string. | None | Operator type name passed to `RegisterAicpuWorkspaceTilingByName`, matching the name used to create tasks. Registration must complete before the first task is created. |
| `tiling` | `AicpuWorkspaceTilingFunc` | Yes | Non-null callback function pointer. | None | Callback that computes the workspace size. Its library must remain loaded while the callback can be used. The callback returns the raw byte count through `uint64_t &workspaceSize`; 0 indicates no workspace is required. Its return type is `aclnnStatus`. |
| `opType` | `const char *` | Yes | Valid, null-terminated operator name string. | None | Label used by `PrintAicpuAllTimeStampInfo`. |

### Tensor List Construction Parameters

`AppendTensor`, `CreateTensorListImpl`, and `CreateTensorList` append results to the container without clearing its existing content.

| Parameter Name | Type | Required | Value Range | Default Value | Meaning and Constraints |
| --- | --- | --- | --- | --- | --- |
| `executor` | `aclOpExecutor *` | Yes | Valid non-null executor for Scalar and IntArray conversion. | None | Converts scalars or integer arrays to host Tensors. The Tensor and TensorList overloads do not use this parameter. |
| `arg` | `const aclTensor *` | Yes | Valid Tensor pointer or `nullptr`. | None | Appended directly to the container. Null input slots can be retained. |
| `arg` | `const aclScalar *` | Yes | Valid non-null scalar pointer. | None | Converted by the executor to a host Tensor using the scalar data type, then appended. |
| `arg` | `const aclIntArray *` | Yes | Valid non-null integer array pointer. | None | Converted by the executor to a host Tensor of type `DT_INT64`, then appended. |
| `arg` | `const aclTensorList *` | Yes | Valid non-null list pointer. | None | Expanded in order, appending each Tensor pointer. |
| `arg` | `OpArg &` | Yes | Argument of type `OPARG_ACLTENSOR`, `OPARG_ACLSCALAR`, `OPARG_INT_LIST`, or `OPARG_ACLTENSOR_LIST`. | None | Single input of `CreateTensorListImpl`. Its content must satisfy the corresponding `AppendTensor` overload requirements. Other types cause an error to be logged and the current dispatch to end. |
| `t` | `OpArgList &` | Yes | Valid argument list; may be empty. | None | List traversed by `CreateTensorList`. Each entry must satisfy the conversion requirements for its type. |
| `l` | `V &` | Yes | Container supporting `emplace_back` for Tensor pointers. | None | Input/output container receiving results from `AppendTensor`. |
| `l` | `TensorList &` | Yes | Container supporting Tensor pointer appends. | None | Input/output container receiving expanded results from `CreateTensorListImpl` and `CreateTensorList`. |

### Attribute and Key Data Parameters

`Append1Byte` writes one byte, `AppendAttrForKey` encodes task key attributes, and `AddAicpuAttr` writes to an attribute map.

| Parameter Name | Type | Required | Value Range | Default Value | Meaning and Constraints |
| --- | --- | --- | --- | --- | --- |
| `buf` | `uint8_t *` | Yes | Valid address with at least one writable byte. | None | Position for the single-byte write. The caller must ensure sufficient capacity; the next write position is returned. |
| `src` | `uint8_t` | Yes | 0 to 255. | None | Single byte to append. |
| `value` | `const V &`, `const std::string &`, `const std::string *`, `std::string *`, `const std::vector<V> &`, `const aclIntArray *`, `aclIntArray *`, `const aclFloatArray *`, `aclFloatArray *`, `const aclBoolArray *`, `aclBoolArray *` | Yes | Attribute value matching the overload type; pointer arguments must be non-null. | None | Input attribute of `AppendAttrForKey`. Generic objects are appended using their byte representation; strings and lists are appended element by element. |
| `key` | `uint8_t *&` | Yes | Current writable position in a valid key buffer. | None | Input/output parameter advanced after appending. Must correspond to the accumulated length `keyLen`. |
| `keyLen` | `size_t &` | Yes | Initial value from 0 to `kAicpuKeyBufLen`. | None | Input/output accumulated byte count. The basic append logic stops at the current capacity of 1024 bytes without reporting a truncation error. The caller must limit the encoded length. |
| `value` | `const V &`, `const aclIntArray *`, `aclIntArray *`, `const aclFloatArray *`, `aclFloatArray *`, `const aclBoolArray *`, `aclBoolArray *` | Yes | Attribute value matching the overload type; array pointers must be non-null. | None | Input attribute of `AddAicpuAttr`. Arrays are converted to the corresponding vector before being stored in the attribute map. |
| `attrName` | `const std::string &` | Yes | Attribute name matching the target operator definition. | None | Name used when writing to `attrs`. |
| `attrs` | `AicpuAttrs &` | Yes | Valid attribute map; may be empty. | None | Input/output map from names to attribute values. An existing entry with the same name is overwritten. |

### Task Constructor and Public Method Parameters

The `opType` and `unknownType` constructor arguments must be supplied for all three task classes. The first three parameters of `InitImpl` have the same meaning as those of `Init`.

| Parameter Name | Type | Required | Value Range | Default Value | Meaning and Constraints |
| --- | --- | --- | --- | --- | --- |
| `opType` | `const std::string &` | Yes | Type name matching the target operator. | None | Task constructor argument used for parameter generation, logging, and operator loading. |
| `unknownType` | `const ge::UnknowShapeOpType` | Yes | Valid enumeration value defined in the matching `graph/types.h`. | None | Dynamic-shape processing category of the operator. |
| `inputs` | `const FVector<const aclTensor *> &` | Yes | Input Tensor list matching the operator definition. | None | Used to initialize task parameter descriptions. The order must match the bindings refreshed before execution. Null input slots are allowed as defined by the operator. |
| `outputs` | `const FVector<aclTensor *> &` | Yes | Output Tensor list with valid elements matching the operator definition. | None | Used to initialize task parameter descriptions. The order must match the bindings refreshed before execution. |
| `attrs` | `const AicpuAttrs &` | Yes | Attribute map matching the operator definition; may be empty. | None | Operator attributes required for initialization. |
| `hasWorkspaceInfo` | `bool` | Yes | `true` or `false`. | None | Whether `InitImpl` reserves a workspace extension record. `Init` passes `false`; the actual address and raw byte count are refreshed before execution. |
| `executor` | `aclOpExecutor *` | Yes | Valid non-null executor pointer. | None | Executor used by `Run` and `SetIoTensors`. Must provide a valid workspace address and stream during execution. |
| `stream` | `aclrtStream` | Yes | Runtime stream for this execution. | None | Stream passed to `Run`. Must match the executor stream; initialization and the `SetIoTensors` refresh must complete before the call. |
| `args` | `op::OpArgContext *` | Yes | Valid non-null argument context pointer. | None | `SetIoTensors` expands inputs and outputs from it and refreshes any workspace record. This method borrows the context without taking ownership. |
| `space` | `void *` | Yes | Valid address of the owning `AicpuTaskSpace`. | None | Task space bound by `SetSpace`. Must remain valid while the task is used. |
| `visit` | `bool` | Yes | `true` or `false`. | None | Occupancy flag passed to `SetVisit`: `true` marks the task as occupied; `false` releases it. |

### Task Space Parameters

| Parameter Name | Type | Required | Value Range | Default Value | Meaning and Constraints |
| --- | --- | --- | --- | --- | --- |
| `opType` | `const std::string &` | Yes | Type name matching the target operator. | None | Operator type managed by this task space. |
| `unknownType` | `const ge::UnknowShapeOpType` | No | Valid enumeration value defined in the matching `graph/types.h`. | `ge::DEPEND_IN_SHAPE` | Dynamic-shape processing category. |
| `isTf` | `const bool` | No | `true` or `false`. | `false` | `true` selects the TensorFlow path; `false` selects the CANN path. |
| `executor` | `aclOpExecutor *` | Yes | Valid non-null executor pointer. | None | Used to find and create tasks, updating the device AI CPU memory budget as needed. |
| `args` | `op::OpArgContext *` | Yes | Valid non-null argument context pointer. | None | Input, output, and attribute context for task lookup and creation. The caller retains ownership. |
| `inputs` | `const FVector<const aclTensor *> &` | Yes | Valid input list; null input slots are allowed. | None | Data types and view formats contribute to lookup keys. Size calculations count only host inputs, skipping null pointers and non-host inputs. |
| `attrNames` | `const FVector<std::string> &` | Yes | Names corresponding one-to-one to the attribute list. | None | The count and order must match the attribute list in `args`. Use an empty container if there are no attributes. |
| `hasWorkspaceInfo` | `bool` | Yes | `true` or `false`. | None | Extension layout flag used by `FindTaskImpl` and `GetOrCreateTaskImpl`. Lookup and creation layouts must match. On the CANN path, `true` requires the corresponding workspace arguments. |
| `index` | `const size_t` | Yes | Valid index in the selected input or output list, starting at 0. | None | Reference index whose upper bound must be ensured by the caller. Reference indices must be configured before the first task is created. |
| `isInput` | `const bool` | No | `true` or `false`. | `true` | `true` selects inputs; `false` selects outputs. |
| `alignBytes` | `size_t` | Yes | Greater than 0 for a meaningful size calculation; the task paths use 4 or 64. | None | Host input alignment granularity in bytes. Passing 0 makes the current implementation return a total size of 0. |
| `aicpuTask` | `std::unique_ptr<AicpuTask> &` | Yes | Reference to a smart pointer owning a valid, initialized task. | None | Used to read the device extended information size and alignment granularity. Ownership is not transferred. |

### Task Key Helper Parameters

`GenHashBinary` and `GenTaskKey` are private helper methods of `AicpuTaskSpace`.

| Parameter Name | Type | Required | Value Range | Default Value | Meaning and Constraints |
| --- | --- | --- | --- | --- | --- |
| `addr` | `const uint8_t *` | Yes | Valid address with at least `len` readable bytes. | None | Binary data to hash in `GenHashBinary`. |
| `len` | `uint32_t` | Yes | Valid data length in bytes. | None | The caller must ensure the length does not exceed the readable buffer. |
| `inputKey` | `uint8_t[]` | Yes | Writable buffer of at least 1024 bytes. | None | Output key buffer for `GenTaskKey`. The caller must ensure the total input and attribute encoding fits within the capacity. |
| `keyLen` | `size_t &` | Yes | Output byte count of the valid key. | None | Output parameter receiving the key length. The function itself returns the hash value. |
| `args` | `op::OpArgContext *` | Yes | Valid non-null argument context pointer. | None | Attributes in the borrowed context contribute to key generation. |
| `inputs` | `const FVector<const aclTensor *> &` | Yes | Valid input list; null input slots are allowed. | None | Data types and view formats contribute to key generation. Null input slots are skipped. |

### Launcher Creation Parameters

`CreatAicpuKernelLauncher` accepts the following parameters. Its name matches the current source code.

| Parameter Name | Type | Required | Value Range | Default Value | Meaning and Constraints |
| --- | --- | --- | --- | --- | --- |
| `opType` | `uint32_t` | Yes | Registered operator type ID satisfying `0 < opType < MAX_OP_TYPE_COUNT`. | None | The current upper bound is 1024. The ID must correspond to the operator type of the task space. |
| `space` | `op::internal::AicpuTaskSpace &` | Yes | Valid task space reference. | None | Used to find, create, and reuse tasks. Must remain valid while the task and launcher are used. |
| `executor` | `aclOpExecutor *` | Yes | Valid non-null executor pointer. | None | Used for workspace planning and launcher registration. |
| `attrNames` | `const FVector<std::string> &` | Yes | Name list matching the count and order of the attributes in `args`. | None | Use an empty container if there are no attributes. |
| `args` | `op::OpArgContext *` | Yes | Valid non-null argument context created by the framework. | None | Input whose ownership is transferred. The creation function or launcher releases it after the call; the caller must not release it again. |

`GetTid()`, `aclnnAicpuFinalize()`, and `Clear()` have no parameters. For task calling conventions, see [AicpuTask Class](AicpuTaskClass.md). For task space constraints, see [AicpuTaskSpace Class](AicpuTaskSpaceClass.md).

## Calling Example

This fragment assumes that the operator is registered, the task space, executor, and argument context are valid, and the attribute list is empty. It illustrates launcher creation. Complete device execution also requires a runtime environment, inputs, outputs, and workspace.

```cpp
#include "opdev/aicpu/aicpu_task.h"

aclnnStatus EnqueueAicpuTask(
    uint32_t opType, op::internal::AicpuTaskSpace &space,
    aclOpExecutor *executor, op::OpArgContext *args)
{
    const op::FVector<std::string> attrNames{};
    // Transfer ownership of args to the creation function; the caller must not release it afterward.
    return op::internal::CreatAicpuKernelLauncher(
        opType, space, executor, attrNames, args);
}
```
