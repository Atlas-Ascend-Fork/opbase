# Documentation Center

## Directory Structure

The structure of the Docs directory is described as follows:

```text
├── en
  ├── api                                # API documentation
  ├── appendix                           # Appendix documentation, such as basic concepts and directory structures
  ├── figures                            # Image directory
  ├── ...
  ├── install                            # Installation and deployment documentation
├── CONTRIBUTING_DOCS.md                 # Documentation contribution guide
├── QUICKSTART.md                        # Quick start
└── README.md
```

## Advanced Tutorials

### Guide Documentation

During operator invocation/development, the basic framework capabilities and public dependencies provided by this project are used. The guides related to the opbase framework are as follows:

| Document                                                         | Description                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [Operator Invocation Guide](https://gitcode.com/cann/ops-math/blob/master/docs/README.md) | Provides the invocation methods and invocation process of the operator library, including the aclnn API and graph mode invocation.<br>The invocation guides of the operator libraries math, nn, cv, and transformer are basically the same. The math repository is used as an example. |
| [Operator Development Guide](https://gitcode.com/cann/ops-math/blob/master/docs/README.md) | Provides the development process and deliverables of AI Core/AI CPU operators.<br>The development guides of the operator libraries math, nn, cv, and transformer are basically the same. The math repository is used as an example. |
| aclnn API Development Guide                                            | **Under construction**. You are welcome to follow and ask questions. |

### API Documentation

The APIs in the opbase library are framework APIs that all operator libraries depend on. They provide various operator base scheduling frameworks and public capabilities (such as log acquisition and Tiling development APIs).

| Category | Description | Header File |
| ------------- |-------|-------|
| [aclnn Public APIs](en/api/nnopbase/aclnn/00_aclnn_api_list.md) |When invoking the aclnn API, you depend on Meta APIs such as framework base APIs and structs, for example, the creation/release of aclTensor/aclScalar objects and the acquisition and setting of various attribute information.|`${INSTALL_DIR}/include/aclnn/acl_meta.h`|
| [nnopbase APIs](en/api/nnopbase/opdev/0_opdev_api_list.md) |When developing the aclnn API, you depend on framework public scheduling and management APIs, such as the aclnn cache and workspace reuse scenarios.|`${INSTALL_DIR}/include/aclnn/opdev/*.h`|
| [op_common APIs](en/api/op_common/0_op_common_list.md) |When invoking or developing operators, provides the public capabilities of the op_common framework, such as Metadef and log logging.|`${INSTALL_DIR}/include/op_common/*.h`|

When invoking opbase APIs, include the required header files with the reference paths listed in the table above. Replace `${INSTALL_DIR}` with the directory where CANN software is installed. For the root user, the default installation path is `/usr/local/Ascend/cann`.

### More Documentation

| Document                                                         | Description                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [Ecosystem Operator Precision Standard](en/ops_precision_standard/mixed_tolerance_standard.md) | For operators developed based on simple engineering, provides the operator precision specification that complies with the **ecosystem open-source contribution guidelines**. |
| [Nnopbase Error Codes](en/error_code/Nnopbase-Errors/Nnopbase-Errors.md)/[Operator Error Codes](en/error_code/Operator-Errors/Operator-Errors.md) | For operator O&M and diagnosis scenarios, summarizes the format definitions, triggering causes, and solutions of common operator error codes. |

## Appendix

### Basic Concepts

- **Level0 API**

  Abbreviated as the L0 API. It indicates the host-side API that invokes a single kernel, and provides the fine-grained APIs (single-kernel dispatch), the basic structures required for operator API development (such as the Tensor definition), and the common basic capabilities (such as workspace reuse and engine scheduling). Upper-layer applications or the L2 API can implement high-performance computation through the rapid assembly of the L0 APIs.

  The return value type of the L0 API is a Tensor-type structure, such as aclTensor\*, std::tuple<aclTensor\*, aclTensor\*\>, or aclTensorList\*. The last parameter is fixed to aclOpExecutor \*executor, and the type and name cannot be changed. The following is an example:

  ```Cpp
  aclTensor* AddNd(aclTensor *x1, aclTensor *x2, aclOpExecutor *executor)
  ```

  The namespace of the L0 API is "namespace l0op", and the API name is "$\{op\_type\}$\{format\}$\{dtype\}", where $\{op\_type\}$ indicates the operator name, $\{format\}$ indicates the input/output data format of the operator, and $\{dtype\}$ indicates the input/output data type of the operator (for unconventional input/output data types, the data type matching relationship must be added). The following are examples:

  ```Cpp
  l0op::AddNd                          //The Add operator inputs are computed in ND format.
  l0op::MatMulNdFp162Fp32              //The MatMul operator inputs and outputs are computed in ND format, and 2 indicates "To", meaning fp16 input and fp32 output.
  l0op::MatMulNzFp162Fp16              //The MatMul operator inputs and outputs are computed in NZ format, and 2 indicates "To", meaning that both input and output are fp16.
  ```

- **Level2 API**

  Abbreviated as the L2 API. It is a high-level encapsulation of the L0 API (internally, it invokes one or more L0 APIs to implement more flexible functions), and indicates a higher-level host-side API (**the aclnn API is usually called the L2 API**). This type of API provides the single-operator direct invocation mode and shields the internal implementation logic of the operator. Users can directly call the L2 API to invoke the operator.

  The return value type of the L2 API is aclnnStatus. It generally includes the "two-phase API" for obtaining the workspaceSize and executing the operator:

  ```Cpp
  aclnnStatus aclnnXxxGetWorkspaceSize(const aclTensor *src, ..., aclTensor *out, ..., uint64_t *workspaceSize, aclOpExecutor **executor);
  aclnnStatus aclnnXxx(void *workspace, uint64_t workspaceSize, aclOpExecutor *executor, aclrtStream stream);
  ```

  - The last two parameters of aclnnXxxGetWorkspaceSize are fixed to \(uint64\_t \*workspaceSize, aclOpExecutor \*\*executor\), and the names and types cannot be changed.
  - The parameters of the aclnnXxx API are fixed to \(void \*workspace, uint64\_t workspaceSize, aclOpExecutor \*executor, aclrtStream stream\).

  aclnnXxxGetWorkspaceSize is the first-phase API and is mainly used to calculate the amount of workspace memory required for this API call. After obtaining the required workspaceSize, apply for NPU memory based on workspaceSize, and then call the second-phase API aclnnXxx to perform the computation. "Xxx" indicates the corresponding operator type, such as the Add operator.

  > **Note**:
  >
  > - workspace indicates the temporary memory required by an API to complete the computation on the AI processor in addition to the inputs and outputs.
  >
  > - The second-phase API aclnnXxx\(...\) cannot be called repeatedly. The following calling mode causes an exception:
  >
  >   ```text
  >   aclnnXxxGetWorkspaceSize(...)
  >   aclnnXxx(...)
  >   aclnnXxx(...)
  >   ```

- **In-place Operator API**

  Indicates an operator API that updates at the original address, whose inputs and outputs use the same address during computation to reduce unnecessary memory occupation. The names of the aclnn in-place operator APIs are generally defined as aclnnInplaceXxxGetWorkspaceSize (first-phase API) and aclnnInplaceXxx (second-phase API).

### build Description

[build Parameters](en/appendix/build.md)