# CANN\_OPS\_OPB\_ASYN\_EXEC\_ACLNN

## Function

The aclnn API is a general execution API in graph scenarios (asynchronous API). It internally calls the first-phase API and registers the second-phase call with the GE context.

## Prototype

```cpp
CANN_OPS_OPB_ASYN_EXEC_ACLNN(ctx, aclnnApi, ...)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| ctx | Input| Operator execution context (OpExecuteContext), used to host operator parameters and set the workspace size.|
| aclnnApi | Input| Name of the ACLNN operator API to be invoked, for example, aclnnScatterList.|
| ... | Input| Operator inputs, outputs, and attributes, which support the gert::Tensor, gert::Tensor list, scalar, and attribute types.|

## Returns

Returns an int value. GRAPH_SUCCESS indicates that the Prepare phase succeeds, and GRAPH_FAILED indicates that the execution fails.

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
auto apiRet = CANN_OPS_OPB_ASYN_EXEC_ACLNN(hostApiCtx, aclnnScatterList, geTensorListVar, indices, update, mask,
                                           reduce, *axis);
OP_CHECK_IF(apiRet != GRAPH_SUCCESS, OP_LOGE(hostApiCtx->GetNodeName(), "apiRet failed:%d", apiRet),
            return GRAPH_FAILED);
```
