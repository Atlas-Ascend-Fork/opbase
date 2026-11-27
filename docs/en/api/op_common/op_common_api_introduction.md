# op\_common API List

For CANN operator development or invocation scenarios (whether built-in or custom operators), the development and invocation processes depend on the public capabilities provided by the opbase library, including log acquisition and the APIs on which operator Tiling/InferShape development depends.

- **Key API Categories**:

  - log: Macros related to operator log printing.
  - math: APIs related to mathematical computation.
  - platform: APIs related to operator execution platform information.
  - infershape: APIs related to operator shape processing.
  - op_host: APIs related to registering AI Core configurations for operators.
  - op_graph: General execution APIs of the aclnn API in graph scenarios.

- **API Description**:

  - API positioning: APIs with detailed parameter descriptions are formal APIs that support backward compatibility. APIs without detailed parameter descriptions are reserved APIs that you can ignore for now.
  - Header file path: When calling the APIs in this section, include the required header files as needed. They are generally defined in the `${INSTALL_DIR}/include` directory, where ${INSTALL_DIR} indicates the path where the CANN software is installed. For example, if the software is installed as the root user, the installation path is`/usr/local/Ascend/cann`.

**Table 1** op\_common API List

<table><thead>
  <tr>
    <th>Category</th>
    <th>API</th>
    <th>Description</th>
    <th>Header File</th>
  </tr></thead>
<tbody>
  <tr>
    <td rowspan="44">log</td>
    <td><a href='log/OP_LOGE.md'>OP_LOGE</a></td>
    <td>Prints operator ERROR-level logs.</td>
    <td rowspan="44">op_common/log/log.h</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGD.md'>OP_LOGD</a></td>
    <td>Prints operator DEBUG-level logs.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGI.md'>OP_LOGI</a></td>
    <td>Prints operator INFO-level logs.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGW.md'>OP_LOGW</a></td>
    <td>Prints operator WARNING-level logs.</td>
  </tr>
  <tr>
    <td><a href='log/OP_CHECK_IF.md'>OP_CHECK_IF</a></td>
    <td>When condition is true, outputs logs and executes the return expression.</td>
  </tr>
  <tr>
    <td><a href='log/OP_CHECK_NULL_WITH_CONTEXT.md'>OP_CHECK_NULL_WITH_CONTEXT</a></td>
    <td>Checks whether the passed pointer is nullptr based on the passed context.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_WITH_INVALID_INPUT_SHAPE.md'>OP_LOGE_WITH_INVALID_INPUT_SHAPE</a></td>
    <td>Records and reports the input shape check error of an operator and reports the EZ0001 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_WITH_INVALID_ATTR.md'>OP_LOGE_WITH_INVALID_ATTR</a></td>
    <td>Records and reports the attribute value check error of an operator and reports the EZ0002 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_WITH_INVALID_ATTR_SIZE.md'>OP_LOGE_WITH_INVALID_ATTR_SIZE</a></td>
    <td>Records and reports the attribute size check error of an operator and reports the EZ0003 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_WITH_INVALID_INPUT.md'>OP_LOGE_WITH_INVALID_INPUT</a></td>
    <td>Records and reports the error of an empty operator input parameter and reports the EZ0004 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_WITH_INVALID_INPUT_SHAPESIZE.md'>OP_LOGE_WITH_INVALID_INPUT_SHAPESIZE</a></td>
    <td>Records and reports the input shape size check error of an operator and reports the EZ0005 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_WITH_INVALID_INPUT_FORMAT.md'>OP_LOGE_WITH_INVALID_INPUT_FORMAT</a></td>
    <td>Records and reports the input format check error of an operator and reports the EZ0006 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_WITH_INVALID_INPUT_DTYPE.md'>OP_LOGE_WITH_INVALID_INPUT_DTYPE</a></td>
    <td>Records and reports the input data type check error of an operator and reports the EZ0007 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_SHAPE.md'>OP_LOGE_FOR_INVALID_SHAPE</a></td>
    <td>Records and reports the parameter shape check error and reports the EZ0008 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_SHAPE_WITH_REASON.md'>OP_LOGE_FOR_INVALID_SHAPE_WITH_REASON</a></td>
    <td>Records and reports the parameter shape check error (with reason) and reports the EZ0009 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_SHAPES_WITH_REASON.md'>OP_LOGE_FOR_INVALID_SHAPES_WITH_REASON</a></td>
    <td>Records and reports the multi-parameter shape check error (with reason) and reports the EZ0010 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_SHAPEDIM.md'>OP_LOGE_FOR_INVALID_SHAPEDIM</a></td>
    <td>Records and reports the parameter shape dimension check error and reports the EZ0011 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_SHAPEDIM_WITH_REASON.md'>OP_LOGE_FOR_INVALID_SHAPEDIM_WITH_REASON</a></td>
    <td>Records and reports the parameter shape dimension check error (with reason) and reports the EZ0012 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_SHAPEDIMS_WITH_REASON.md'>OP_LOGE_FOR_INVALID_SHAPEDIMS_WITH_REASON</a></td>
    <td>Records and reports the multi-parameter shape dimension check error (with reason) and reports the EZ0013 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_SHAPESIZE.md'>OP_LOGE_FOR_INVALID_SHAPESIZE</a></td>
    <td>Records and reports the parameter shape size check error and reports the EZ0014 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_SHAPESIZE_WITH_REASON.md'>OP_LOGE_FOR_INVALID_SHAPESIZE_WITH_REASON</a></td>
    <td>Records and reports the parameter shape size check error (with reason) and reports the EZ0015 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_SHAPESIZES_WITH_REASON.md'>OP_LOGE_FOR_INVALID_SHAPESIZES_WITH_REASON</a></td>
    <td>Records and reports the multi-parameter shape size check error (with reason) and reports the EZ0016 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_FORMAT.md'>OP_LOGE_FOR_INVALID_FORMAT</a></td>
    <td>Records and reports the parameter format check error and reports the EZ0017 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_FORMATS_WITH_REASON.md'>OP_LOGE_FOR_INVALID_FORMATS_WITH_REASON</a></td>
    <td>Records and reports the multi-parameter format check error (with reason) and reports the EZ0018 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_DTYPE.md'>OP_LOGE_FOR_INVALID_DTYPE</a></td>
    <td>Records and reports the parameter data type check error and reports the EZ0019 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_DTYPE_WITH_REASON.md'>OP_LOGE_FOR_INVALID_DTYPE_WITH_REASON</a></td>
    <td>Records and reports the parameter data type check error (with reason) and reports the EZ0020 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_DTYPES_WITH_REASON.md'>OP_LOGE_FOR_INVALID_DTYPES_WITH_REASON</a></td>
    <td>Records and reports the multi-parameter data type check error (with reason) and reports the EZ0021 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_TENSORNUM.md'>OP_LOGE_FOR_INVALID_TENSORNUM</a></td>
    <td>Records and reports the parameter tensor quantity check error and reports the EZ0022 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_TENSORNUMS_WITH_REASON.md'>OP_LOGE_FOR_INVALID_TENSORNUMS_WITH_REASON</a></td>
    <td>Records and reports the multi-parameter tensor quantity check error (with reason) and reports the EZ0023 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_VALUE.md'>OP_LOGE_FOR_INVALID_VALUE</a></td>
    <td>Records and reports the parameter value check error and reports the EZ0024 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_LISTSIZE.md'>OP_LOGE_FOR_INVALID_LISTSIZE</a></td>
    <td>Records and reports the parameter list size check error and reports the EZ0025 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_VALUE_WITH_REASON.md'>OP_LOGE_FOR_INVALID_VALUE_WITH_REASON</a></td>
    <td>Records and reports the parameter value check error (with reason) and reports the EZ0026 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_VALUES_WITH_REASON.md'>OP_LOGE_FOR_INVALID_VALUES_WITH_REASON</a></td>
    <td>Records and reports the multi-parameter value check error (with reason) and reports the EZ0027 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_STRIDE.md'>OP_LOGE_FOR_INVALID_STRIDE</a></td>
    <td>Records and reports the parameter stride check error and reports the EZ0028 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_FILE_PATH.md'>OP_LOGE_FOR_FILE_PATH</a></td>
    <td>Records and reports the invalid file path error and reports the EZ0029 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_FILE_OPEN.md'>OP_LOGE_FOR_FILE_OPEN</a></td>
    <td>Records and reports the file opening failure and reports the EZ0030 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_FILE_PARSE.md'>OP_LOGE_FOR_FILE_PARSE</a></td>
    <td>Records and reports the file parsing failure and reports the EZ0031 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_CONFIG.md'>OP_LOGE_FOR_INVALID_CONFIG</a></td>
    <td>Records and reports the invalid configuration item value error and reports the EZ0032 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_CONFIG_WITH_REASON.md'>OP_LOGE_FOR_INVALID_CONFIG_WITH_REASON</a></td>
    <td>Records and reports the invalid configuration item value error (with reason) and reports the EZ0033 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_CONFIGS_WITH_REASON.md'>OP_LOGE_FOR_INVALID_CONFIGS_WITH_REASON</a></td>
    <td>Records and reports the multi-configuration-item invalid value error (with reason) and reports the EZ0034 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_FORMAT_WITH_REASON.md'>OP_LOGE_FOR_INVALID_FORMAT_WITH_REASON</a></td>
    <td>Records and reports the parameter format check error (with reason) and reports the EZ0035 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_GRAPH_NODE.md'>OP_LOGE_FOR_INVALID_GRAPH_NODE</a></td>
    <td>Records and reports the invalid graph node error and reports the EZ0036 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_ARGUMENT_WITH_REASON.md'>OP_LOGE_FOR_INVALID_ARGUMENT_WITH_REASON</a></td>
    <td>Records and reports the invalid parameter error (with reason) and reports the EZ0037 error code.</td>
  </tr>
  <tr>
    <td><a href='log/OP_LOGE_FOR_INVALID_LISTSIZE_WITH_REASON.md'>OP_LOGE_FOR_INVALID_LISTSIZE_WITH_REASON</a></td>
    <td>Records and reports the parameter list size check error (with reason) and reports the EZ0038 error code.</td>
  </tr>
  <tr>
    <td rowspan="5">math</td>
    <td><a href='math/FloorDiv.md'>FloorDiv</a></td>
    <td>Division rounded down.</td>
    <td rowspan="5">op_common/op_host/util/math_util.h</td>
  </tr>
  <tr>
    <td><a href='math/FloorAlign.md'>FloorAlign</a></td>
    <td>Aligns downward with align as the unit.</td>
  </tr>
  <tr>
    <td><a href='math/CeilDiv.md'>CeilDiv</a></td>
    <td>Division rounded up.</td>
  </tr>
  <tr>
    <td><a href='math/CeilAlign.md'>CeilAlign</a></td>
    <td>Aligns upward with align as the unit.</td>
  </tr>
  <tr>
    <td><a href='math/IsFloatEqual.md'>IsFloatEqual</a></td>
    <td>Determines whether two float- or double-type values are equal.</td>
  </tr>
  <tr>
    <td rowspan="11">platform</td>
    <td><a href='platform/GetAivCoreNum.md'>GetAivCoreNum</a></td>
    <td>Obtains the number of AI Vector cores on the platform.</td>
    <td rowspan="11">op_common/op_host/util/platform_util.h</td>
  </tr>
  <tr>
    <td><a href='platform/GetAicCoreNum.md'>GetAicCoreNum</a></td>
    <td>Obtains the number of AI Cube cores on the platform.</td>
  </tr>
  <tr>
    <td><a href='platform/GetUbSize.md'>GetUbSize</a></td>
    <td>Obtains the size of the platform UB (Unified Buffer) space, in bytes.</td>
  </tr>
  <tr>
    <td><a href='platform/GetUbBlockSize.md'>GetUbBlockSize</a></td>
    <td>Obtains the block unit size of the platform UB (Unified Buffer), in bytes.</td>
  </tr>
  <tr>
    <td><a href='platform/GetVRegSize.md'>GetVRegSize</a></td>
    <td>Obtains the size of the platform vector register, in bytes.</td>
  </tr>
  <tr>
    <td><a href='platform/GetSimtMaxThreadNum.md'>GetSimtMaxThreadNum</a></td>
    <td>Obtains the maximum number of SIMT threads on the platform.</td>
  </tr>
  <tr>
    <td><a href='platform/GetSimtMaxDCacheSize.md'>GetSimtMaxDCacheSize</a></td>
    <td>Obtains the maximum SIMT DCache size on the platform, in bytes.</td>
  </tr>
  <tr>
    <td><a href='platform/GetCacheLineSize.md'>GetCacheLineSize</a></td>
    <td>Obtains the size of the platform cache line, in bytes.</td>
  </tr>
  <tr>
    <td><a href='platform/GetSectorCacheLineSize.md'>GetSectorCacheLineSize</a></td>
    <td>Obtains the size of the platform sector cache line, in bytes.</td>
  </tr>
  <tr>
    <td><a href='platform/GetNddmaDcacheSize.md'>GetNddmaDcacheSize</a></td>
    <td>Obtains the size of the platform NDDMA DCache, in bytes.</td>
  </tr>
  <tr>
    <td><a href='platform/GetWorkspaceSize.md'>GetWorkspaceSize</a></td>
    <td>Obtains the size of the platform LibApi workspace, in bytes.</td>
  </tr>
  <tr>
    <td rowspan="8">infershape</td>
    <td><a href='infershape/SetUnknownRank.md'>SetUnknownRank</a></td>
    <td>Sets the input shape to a dynamic shape with unknown dims.</td>
    <td rowspan="4">op_common/op_host/util/shape_util.h</td>
  </tr>
  <tr>
    <td><a href='infershape/IsUnknownRank.md'>IsUnknownRank</a></td>
    <td>Checks whether the input shape has unknown dims.</td>
  </tr>
  <tr>
    <td><a href='infershape/SetUnknownShape.md'>SetUnknownShape</a></td>
    <td>Sets the dims of the input shape to rank, with the length of each axis being an unknown value.</td>
  </tr>
  <tr>
    <td><a href='infershape/IsUnknownShape.md'>IsUnknownShape</a></td>
    <td>Checks whether the length of each axis of the input shape is an unknown value.</td>
  </tr>
  <tr>
    <td><a href='infershape/InferShape4Broadcast.md'>InferShape4Broadcast</a></td>
    <td>InferShape method for broadcast-type operators.</td>
    <td rowspan="2">op_common/op_host/infershape_broadcast_util.h</td>
  </tr>
  <tr>
    <td><a href='infershape/BroadcastShape.md'>BroadcastShape</a></td>
    <td>Deduces the output shape after broadcast from the shape of the input tensors.</td>
  </tr>
  <tr>
    <td><a href='infershape/InferShape4Elewise.md'>InferShape4Elewise</a></td>
    <td>InferShape method for elewise-type operators.</td>
    <td rowspan="1">op_common/op_host/infershape_elewise_util.h</td>
  </tr>
  <tr>
    <td><a href='infershape/InferShape4Reduce.md'>InferShape4Reduce</a></td>
    <td>InferShape method for reduce-type operators.</td>
    <td rowspan="1">op_common/op_host/infershape_reduce_util.h</td>
  </tr>
  <tr>
    <td rowspan="2">op_graph</td>
    <td><a href='op_graph/CANN_OPS_OPB_SYN_EXEC_ACLNN.md'>CANN_OPS_OPB_SYN_EXEC_ACLNN</a></td>
    <td>General execution API of the aclnn API in graph scenarios (synchronous API), which serially invokes two-phase execution internally.</td>
    <td rowspan="2">op_common/op_graph/op_fallback.h</td>
  </tr>
  <tr>
    <td><a href='op_graph/CANN_OPS_OPB_ASYN_EXEC_ACLNN.md'>CANN_OPS_OPB_ASYN_EXEC_ACLNN</a></td>
    <td>General execution API of the aclnn API in graph scenarios (asynchronous API), which invokes the first-phase API internally and registers the second phase with the GE context.</td>
  </tr>
  <tr>
    <td rowspan="1">op_host</td>
    <td><a href='op_host/AddConfigByArch.md'>AddConfigByArch</a></td>
    <td>Registers the AI Core configuration of operators in batches by NPU architecture.</td>
    <td rowspan="1">op_common/op_host/util/op_arch_helper.h</td>
  </tr>
  <tr>
    <td><a href='reserved_interface_op_common.md'>Reserved APIs</a></td>
    <td>-</td>
    <td>Reserved APIs. You can ignore them for now.</td>
    <td>
      <ul>
        <li>op_common/op_host/*.h</li>
        <li>op_common/op_kernel/*.h</li>
        <li>op_common/op_graph/*.h</li>
      </ul></td>
  </tr>
</tbody>
</table>
