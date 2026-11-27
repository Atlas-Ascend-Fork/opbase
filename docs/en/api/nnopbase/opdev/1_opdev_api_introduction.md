# nnopbase API List

Both built-in CANN operators and custom operators (for example, Transformer and NN operators) can invoke the underlying operators through the aclnn API (also called the [Level2 API](#level2-api)).

**For the scenario of developing the aclnn API**, this section provides the underlying **framework capability APIs** and **basic tensor operation APIs** (also called the [Level0 API](#level0-api)) that are relied on during development.

- **Framework capability APIs**:

  Provide the basic capability APIs for implementing the aclnn API, such as operator executor (OpExecutor) processing and data type, format, and shape operations, as shown in [Table 1](#table1). They also include common classes and macros, as shown in [Table 2](#table2) and [Table 3](#table3).

- **Basic tensor operation APIs**:

  Provide the basic tensor operation APIs for implementing the aclnn API, such as tensor data type conversion and shape reconstruction, as shown in [Table 4](#table4).

## Framework Capability APIs

Header file description: When calling the APIs in this section, include the dependent header files based on the site requirements. The header files are stored in the `${INSTALL_DIR}/include` directory. `${INSTALL_DIR}` indicates the CANN software installation path. For example, if the software is installed as the root user, the installation path is `/usr/local/Ascend/cann`.

**Table 1** API list <a id="table1"></a>

| API Category| Description| Header File|
| --- | --- | --- |
| [bfloat16](bfloat16/bfloat16.md) | Describes the implementation class of the bfloat16 data type on the CPU side.| aclnn/opdev/bfloat16.h |
| [common_types](common_types/common_types.md) | Describes the basic aclnn data structures such as aclTensor and aclScalar.| aclnn/opdev/common_types.h |
| [data_type_utils](data_type_utils/data_type_utils.md) | Provides the basic APIs related to DataType, for example, obtaining the size of a specified DataType.| aclnn/opdev/data_type_utils.h |
| fast_vector | Describes the FastVector data type, which is an efficient vector data structure implemented in aclnn.<br>**Note:** The APIs defined in this header file are reserved APIs and can be ignored.| aclnn/opdev/fast_vector.h |
| [format_utils](format_utils/format_utils.md) | Provides the basic APIs related to Format.| aclnn/opdev/format_utils.h |
| [fp16_t](fp16_t/fp16_t.md) | Describes the implementation class of the float16 data type on the CPU side.| aclnn/opdev/fp16_t.h |
| [float4_e1m2](float4_e1m2/float4_e1m2.md) | Describes the implementation class of the Float4E1M2 data type on the CPU side.| aclnn/opdev/float4_e1m2.h |
| [float4_e2m1](float4_e2m1/float4_e2m1.md) | Describes the implementation class of the Float4E2M1 data type on the CPU side.| aclnn/opdev/float4_e2m1.h |
| [float6_e2m3](float6_e2m3/float6_e2m3.md) | Describes the implementation class of the Float6E2M3 data type on the CPU side.| aclnn/opdev/float6_e2m3.h |
| [float6_e3m2](float6_e3m2/float6_e3m2.md) | Describes the implementation class of the Float6E3M2 data type on the CPU side.| aclnn/opdev/float6_e3m2.h |
| [float8_e4m3fn](float8_e4m3fn/float8_e4m3fn.md) | Describes the implementation class of the Float8E4M3FN data type on the CPU side.| aclnn/opdev/float8_e4m3fn.h |
| [float8_e5m2](float8_e5m2/float8_e5m2.md) | Describes the implementation class of the Float8E5M2 data type on the CPU side.| aclnn/opdev/float8_e5m2.h |
| [float8_e8m0](float8_e8m0/float8_e8m0.md) | Describes the implementation class of the Float8E8M0 data type on the CPU side.| aclnn/opdev/float8_e8m0.h |
| [hifloat4](hifloat4/hifloat4.md) | Describes the implementation class of the HiFloat4 data type on the CPU side.| aclnn/opdev/hifloat4.h |
| [hifloat8](hifloat8/hifloat8.md) | Describes the implementation class of the HiFloat8 data type on the CPU side.| aclnn/opdev/hifloat8.h |
| [framework_op](framework_op/framework_op.md) | Describes the copy capability from the host side to the device side provided by the framework.| aclnn/opdev/framework_op.h |
| make_op_executor | Provides the macro declaration for initializing aclOpExecutor.<br>**Note:** The APIs defined in this header file are reserved APIs and can be ignored.| aclnn/opdev/make_op_executor.h |
| [object](object/object.md) | Describes the base class Object of the basic data structures such as aclTensor in aclnn, which is used to overload and implement the new and delete methods.| aclnn/opdev/object.h |
| [op_arg_def](op_arg_def/op_arg_def.md) | Describes the OpArgContext class and provides macro declarations such as OP_INPUT.| aclnn/opdev/op_arg_def.h |
| [op_cache](op_cache/op_cache.md) | Describes the OpExecCache class and related classes, which are used to complete aclnn caching and improve running performance.| aclnn/opdev/op_cache.h |
| [op_cache_container](op_cache_container/op_cache_container.md) | Describes the aclnn cache container with the LRU eviction mechanism.| aclnn/opdev/op_cache_container.h |
| [op_config](op_config/op_config.md) | Provides the configuration information related to operator running, such as the deterministic computation switch.| aclnn/opdev/op_config.h |
| [op_def](op_def/op_def.md) | Defines the basic enums and constants, for example, the precision mode OpImplMode.| aclnn/opdev/op_def.h |
| [op_dfx](op_dfx/op_dfx.md) | Describes the DfxGuard class, which is used for API printing and profiling reporting.| aclnn/opdev/op_dfx.h |
| [aclnn Return Codes](aclnn_return_code/aclnn_return_codes.md) | Defines the aclnn error codes.| aclnn/opdev/op_errno.h |
| [op_executor](op_executor/op_executor.md) | Describes the aclOpExecutor class.| aclnn/opdev/op_executor.h |
| [op_log](op_log/op_log.md) | Defines the log printing macros in aclnn.| aclnn/opdev/op_log.h |
| [platform](platform/platform.md) | Describes the PlatformInfo class, which stores the SOC platform information.| aclnn/opdev/platform.h |
| [pool_allocator](pool_allocator/pool_allocator.md) | Describes the PoolAllocator class, which implements the internal CPU memory pool of aclnn.| aclnn/opdev/pool_allocator.h |
| [shape_utils](shape_utils/shape_utils.md) | Provides the basic operations related to shape, such as shape printing.| aclnn/opdev/shape_utils.h |
| [small_vector](small_vector/small_vector.md) | Describes the SmallVector class, which is an efficient vector data structure implemented in aclnn and is mainly used in scenarios where the data volume is known to be small.| aclnn/opdev/small_vector.h |
| [tensor_view_utils](tensor_view_utils/tensor_view_utils.md) | Provides the basic operations on the View class, such as determining whether aclTensor is contiguous.| aclnn/opdev/tensor_view_utils.h |
| [data_type_utils](op_common_related_interface/data_type_utils.md) | Provides the basic APIs related to DataType, for example, determining whether a specified DataType is an integer type.| aclnn/opdev/op_common/data_type_utils.h<br>**Note:** This header file will be deprecated in later versions. You are advised to replace it with the APIs defined in aclnn/opdev/data_type_utils.h.|
| [aicpu_args_handler](aicpu_related_interface/aicpu_args_handler.md) | Provides the processing logic of AI CPU-related combined computation tasks, such as concatenating the parameters related to computation tasks.| aclnn/opdev/aicpu/aicpu_args_handler.h |
| [aicpu_ext_info_handle](aicpu_related_interface/aicpu_ext_info_handle.md) | Provides the processing logic of the extended parameters of AI CPU-related computation tasks, such as the APIs for concatenating and parsing extended parameters.| aclnn/opdev/aicpu/aicpu_ext_info_handle.h |
| [aicpu_task](aicpu_related_interface/aicpu_task.md) | Provides the logic for setting and dispatching AI CPU tasks, such as setting which AI CPU operator to call and setting the operator inputs and outputs.| aclnn/opdev/aicpu/aicpu_task.h |
| [aicpu_utils](aicpu_related_interface/aicpu_utils.md) | Provides some common APIs required by AI CPU tasks.| aclnn/opdev/aicpu/aicpu_utils.h |

**Table 2** Common macros <a id="table2"></a>

| Macro| Description| Header File|
| --- | --- | --- |
| [DFX_IN](common_macros_and_classes/DFX_IN.md) | In L2_DFX_PHASE_1, packages all host-side API input parameters.| aclnn/opdev/op_dfx.h |
| [DFX_OUT](common_macros_and_classes/DFX_OUT.md) | In L2_DFX_PHASE_1, packages all host-side API output parameters.| aclnn/opdev/op_dfx.h |
| [L0_DFX](common_macros_and_classes/L0_DFX.md) | Must be used in the L0 API of a host-side API for printing the API and the L0 API input parameters.| aclnn/opdev/op_dfx.h |
| [L2_DFX_PHASE_1](common_macros_and_classes/L2_DFX_PHASE_1.md) | Must be called at the beginning of a first-phase API for printing the API and the first-phase input parameters.| aclnn/opdev/op_dfx.h |
| [L2_DFX_PHASE_2](common_macros_and_classes/L2_DFX_PHASE_2.md) | Must be called at the beginning of a second-phase API for printing the API.| aclnn/opdev/op_dfx.h |
| [OP_TYPE_REGISTER](common_macros_and_classes/OP_TYPE_REGISTER.md) | Must be used at the beginning of the L0 API to register an L0 operator.| aclnn/opdev/op_dfx.h |
| [OP_ATTR](common_macros_and_classes/OP_ATTR.md) | Packages operator attribute parameters in ADD_TO_LAUNCHER_LIST_AICORE.| aclnn/opdev/op_arg_def.h |
| [OP_EMPTY_ARG](common_macros_and_classes/OP_EMPTY_ARG.md) | Used in ADD_TO_LAUNCHER_LIST_AICORE to occupy an empty input or output.| aclnn/opdev/op_arg_def.h |
| [OP_INPUT](common_macros_and_classes/OP_INPUT.md) | Packages an operator input aclTensor in ADD_TO_LAUNCHER_LIST_AICORE.| aclnn/opdev/op_arg_def.h |
| [OP_MODE](common_macros_and_classes/OP_MODE.md) | Packages operator running options in ADD_TO_LAUNCHER_LIST_AICORE, for example, whether to enable HF32.| aclnn/opdev/op_arg_def.h |
| [OP_OUTPUT](common_macros_and_classes/OP_OUTPUT.md) | Packages an operator output aclTensor in ADD_TO_LAUNCHER_LIST_AICORE.| aclnn/opdev/op_arg_def.h |
| [OP_OUTSHAPE](common_macros_and_classes/OP_OUTSHAPE.md) | In ADD_TO_LAUNCHER_LIST_AICORE, for a third-category operator, sets the aclTensor that stores the output shape.| aclnn/opdev/op_arg_def.h |
| [OP_OPTION](common_macros_and_classes/OP_OPTION.md) | Packages the precision mode specified by an operator in ADD_TO_LAUNCHER_LIST_AICORE.| aclnn/opdev/op_arg_def.h |
| [OP_WORKSPACE](common_macros_and_classes/OP_WORKSPACE.md) | Packages the workspace parameter explicitly specified by an operator in ADD_TO_LAUNCHER_LIST_AICORE.| aclnn/opdev/make_op_executor.h |
| [CREATE_EXECUTOR](common_macros_and_classes/CREATE_EXECUTOR.md) | Creates a UniqueExecutor object, which is the factory class that generates aclOpExecutor.| aclnn/opdev/make_op_executor.h |
| [INFER_SHAPE](common_macros_and_classes/INFER_SHAPE.md) | For a specified operator, runs its infershape function to deduce the output shape.| aclnn/opdev/make_op_executor.h |
| [ADD_TO_LAUNCHER_LIST_AICORE](common_macros_and_classes/ADD_TO_LAUNCHER_LIST_AICORE.md) | Creates the execution task of an AI Core operator and places it in the execution queue of aclOpExecutor. It is executed in the second phase.| aclnn/opdev/make_op_executor.h |
| [OP_ATTR_NAMES](common_macros_and_classes/OP_ATTR_NAMES.md) | A vector of the String type, which packages the character-type attributes of an AI CPU operator.| aclnn/opdev/aicpu/aicpu_task.h |
| [ADD_TO_LAUNCHER_LIST_AICPU](common_macros_and_classes/ADD_TO_LAUNCHER_LIST_AICPU.md) | Creates the execution task of an AI CPU operator and places it in the execution queue of aclOpExecutor. It is executed in the second phase.| aclnn/opdev/aicpu/aicpu_task.h |
| [REGISTER_AICPU_WORKSPACE_TILING](common_macros_and_classes/REGISTER_AICPU_WORKSPACE_TILING.md) | Registers the Tiling function of an AICPU operator to calculate, in advance, the workspace size required for the intermediate computation of a kernel, so that the framework plans and passes the preallocated workspace.| aclnn/opdev/aicpu/aicpu_task.h |

**Table 3** Common classes and structs <a id="table3"></a>

| Class/Struct| Description| Header File|
| --- | --- | --- |
| aclOpExecutor | Represents the operator executor and records the context structure of the running information of the entire host-side API, such as the computation graph during L2 API execution, the L0 operator launch subtasks, and the workspace address and size.<br>The member variables defined in this class are private and can be ignored. For the defined member functions, see [op_executor](op_executor/op_executor.md).| aclnn/opdev/op_executor.h |
| aclTensor | Represents a tensor object, including the shape, dtype, format, and address information that describes the tensor. The data can be stored on the host side or device side.<br>The member variables defined in this class are private and can be ignored. For the defined member functions, see [common_types](common_types/common_types.md).| aclnn/opdev/common_types.h |
| aclScalar | Represents a scalar object. The data is usually stored on the host side.<br>The member variables defined in this class are private and can be ignored. For the defined member functions, see [common_types](common_types/common_types.md).| aclnn/opdev/common_types.h |
| aclTensorList | Represents a list object consisting of a group of aclTensor objects.<br>The member variables defined in this class are private and can be ignored. For the defined member functions, see [common_types](common_types/common_types.md).| aclnn/opdev/common_types.h |
| aclScalarList | Represents a list object consisting of a group of aclScalar objects.<br>The member variables defined in this class are private and can be ignored. For the defined member functions, see [common_types](common_types/common_types.md).| aclnn/opdev/common_types.h |
| aclBoolArray | Represents a boolean array object. The data is usually stored on the host side.<br>The member variables defined in this class are private and can be ignored. For the defined member functions, see [common_types](common_types/common_types.md).| aclnn/opdev/common_types.h |
| aclIntArray | Represents an int64_t array object. The data is usually stored on the host side.<br>The member variables defined in this class are private and can be ignored. For the defined member functions, see [common_types](common_types/common_types.md).| aclnn/opdev/common_types.h |
| aclFloatArray | Represents an fp32 array object. The data is usually stored on the host side.<br>The member variables defined in this class are private and can be ignored. For the defined member functions, see [common_types](common_types/common_types.md).| aclnn/opdev/common_types.h |
| aclFp16Array | Represents an fp16 array object. The data is usually stored on the host side.<br>The member variables defined in this class are private and can be ignored. For the defined member functions, see [common_types](common_types/common_types.md).| aclnn/opdev/common_types.h |
| aclBf16Array | Represents a bf16 array object. The data is usually stored on the host side.<br>The member variables defined in this class are private and can be ignored. For the defined member functions, see [common_types](common_types/common_types.md).| aclnn/opdev/common_types.h |
| SmallVector | Implements a vector container using an internal memory pool. The basic functions are the same as those of the std::vector container in the C++ standard library, and it does not need to apply for memory each time the capacity is expanded, avoiding performance impact. The member variables defined in it are private and can be ignored. For the defined member functions, see [small_vector](small_vector/small_vector.md).| aclnn/opdev/small_vector.h |
| OpExecMode | Enum class that represents the operator running mode. For the definition, see [OpExecMode](common_macros_and_classes/OpExecMode.md).| aclnn/opdev/op_def.h |
| OpImplMode | Enum class that represents the operator precision mode. For the definition, see [OpImplMode](common_macros_and_classes/OpImplMode.md).| aclnn/opdev/op_def.h |

> **SmallVector usage instructions**:
>
> - op::FVector: In essence, a SmallVector with a storage capacity of 8.
>
>   ```cpp
>   namespace op {
>   template<typename T, size_t N = 8>
>   using FVector = op::internal::SmallVector<T, N, op::internal::PoolAllocator<T>>;
>   }
>   ```
>
> - op::Strides: In essence, an FVector with a storage capacity of 25 and the element type int64_t, which stores the stride information.
>
>   ```cpp
>   namespace op {
>   constexpr uint64_t MAX_DIM_NUM = 25;
>   using Strides = FVector<int64_t, MAX_DIM_NUM>;
>   }
>   ```
>
> - op::ShapeVector: In essence, an FVector with a storage capacity of 25 and the element type int64_t, which stores the shape information. <a id="shapevector"></a>
>
>   ```cpp
>   namespace op {
>   constexpr uint64_t MAX_DIM_NUM = 25;
>   using ShapeVector = FVector<int64_t, MAX_DIM_NUM>;
>   }
>   ```

## Basic Tensor Operation APIs <a id="table4"></a>

**Table 4** API list

| API| Description| Header File|
| --- | --- | --- |
| [Cast](L0/Cast.md) | Converts an input tensor to a specified data type.| aclnn_kernels/cast.h |
| [Contiguous](L0/Contiguous.md) | Converts a non-contiguous tensor into a contiguous tensor.| aclnn_kernels/contiguous.h |
| [ViewCopy](L0/ViewCopy.md) | Copies a contiguous tensor to a contiguous or non-contiguous tensor.| aclnn_kernels/contiguous.h |
| [Pad](L0/Pad.md) | Pads each dimension of an input tensor with 0 based on the paddings size.| aclnn_kernels/pad.h |
| [Reshape](L0/Reshape.md) | Converts the shape of the input tensor x to the shape specified by the second parameter.| aclnn_kernels/reshape.h |
| [Slice](L0/Slice.md) | Extracts the required slice from an input tensor.| aclnn_kernels/slice.h |
| [Transpose](L0/Transpose.md) | Transposes the shape of the input tensor x based on the dimension permutation order perm and outputs the result.| aclnn_kernels/transpose.h |
| [TransData](L0/TransData.md) | Converts the format of an input tensor to the specified dstPrimaryFormat.| aclnn_kernels/transdata.h |
| [TransDataSpecial](L0/TransDataSpecial.md) | Converts the format of an input tensor to the specified dstPrimaryFormat. Similar to TransData.| aclnn_kernels/transdata.h |
| [ReFormat](L0/ReFormat.md) | When the specified format is the same as the dimension of the input x, sets the input data format to the target format.| aclnn_kernels/transdata.h |
| [IsNullptr](L0/IsNullptr.md) | Determines whether the input pointer is null.| aclnn_kernels/op_error_check.h |

## Basic Concepts

### **Level0 API**

  Abbreviated as the L0 API. It indicates the host-side API that invokes a single kernel, and provides the fine-grained APIs (single-kernel dispatch), the basic structures required for operator API development (such as the Tensor definition), and the common basic capabilities (such as workspace reuse and engine scheduling). Upper-layer applications or the L2 API can implement high-performance computation through the rapid assembly of the L0 APIs.

  The return value type of the L0 API is a Tensor-type structure, such as aclTensor\*, std::tuple<aclTensor\*, aclTensor\*\>, or aclTensorList\*. The last parameter is fixed to aclOpExecutor \*executor, and the type and name cannot be changed. The following is an example:

    ```cpp
    aclTensor* AddNd(aclTensor *x1, aclTensor *x2, aclOpExecutor *executor)
    ```

  The namespace of the L0 API is "namespace l0op", and the API name is "$\{op\_type\}$\{format\}$\{dtype\}", where $\{op\_type\}$ indicates the operator name, $\{format\}$ indicates the input/output data format of the operator, and $\{dtype\}$ indicates the input/output data type of the operator. For unconventional input/output data types, the data type matching relationship must be added. The following are examples:

    ```cpp
    l0op::AddNd                               //The Add operator inputs are computed in ND format.
    l0op::MatMulNdFp162Fp32                    //The MatMul operator inputs and outputs are computed in ND format, and 2 indicates "To", meaning fp16 input and fp32 output.
    l0op::MatMulNzFp162Fp16                    //The MatMul operator inputs and outputs are computed in NZ format, and 2 indicates "To", meaning that both input and output are fp16.
    ```

### **Level2 API**

  Abbreviated as the L2 API. It is a high-level encapsulation of the L0 API and indicates a higher-level host-side API. Internally, it invokes one or more L0 APIs to implement more flexible functions. This type of API provides the single-operator direct invocation mode and shields the internal implementation logic of the operator. Users can directly call the L2 API to invoke the operator.

  The return value type of the L2 API is aclnnStatus. It generally includes the "two-phase API" for obtaining the workspaceSize and executing the operator:

    ```cpp
    aclnnStatus aclnnXxxGetWorkspaceSize(const aclTensor *src, ..., aclTensor *out, ..., uint64_t *workspaceSize, aclOpExecutor **executor);
    aclnnStatus aclnnXxx(void *workspace, uint64_t workspaceSize, aclOpExecutor *executor, aclrtStream stream);
    ```

  - The last two parameters of aclnnXxxGetWorkspaceSize are fixed to \(uint64\_t \*workspaceSize, aclOpExecutor \*\*executor\), and the names and types cannot be changed.
  - The parameters of the aclnnXxx API are fixed to \(void \*workspace, uint64\_t workspaceSize, aclOpExecutor \*executor, aclrtStream stream\).

    aclnnXxxGetWorkspaceSize is the first-phase API and is mainly used to calculate the amount of workspace memory required for this API call. After obtaining the required workspaceSize, apply for NPU memory based on workspaceSize, and then call the second-phase API aclnnXxx to perform the computation. "Xxx" indicates the corresponding operator type, such as the Add operator.

    > **Note:**
    > - workspace indicates the temporary memory required by an API to complete the computation on the AI processor in addition to the inputs and outputs. workspaceSize indicates the size of the workspace memory.
    > - The second-phase API aclnnXxx\(...\) cannot be called repeatedly. The following calling mode causes an exception:
    >
    >   ```cpp
    >   aclnnXxxGetWorkspaceSize(...)
    >   aclnnXxx(...)
    >   aclnnXxx(...)
    >   ```

### **In-place operator API**

  Indicates an operator API that updates at the original address, whose inputs and outputs use the same address during computation to reduce unnecessary memory occupation. The names of the aclnn in-place operator APIs are generally defined as aclnnInplaceXxxGetWorkspaceSize (first-phase API) and aclnnInplaceXxx (second-phase API).
