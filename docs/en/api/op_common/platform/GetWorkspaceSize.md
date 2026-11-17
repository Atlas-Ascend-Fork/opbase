# GetWorkspaceSize

## Function

Obtains the size of the platform LibApi workspace, in bytes.

## Prototype

```cpp
template <typename T>
uint32_t GetWorkspaceSize(const T *context)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| context | Input| Tiling context, which is of type `TilingContext` or `TilingParseContext`.|

## Returns

`uint32_t`: size of the platform LibApi workspace. The value **0** indicates that the size failed to be obtained.

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
compileInfo->workspaceSize = GetWorkspaceSize(context_);
```
