# GetVRegSize

## Function

Obtains the size of the platform vector register (Vector Register), in bytes.

## Prototype

```cpp
template <typename T>
uint32_t GetVRegSize(const T *context)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| context | Input| Tiling context, which is of type `TilingContext` or `TilingParseContext`.|

## Returns

`uint32_t`: size of the vector register on the current platform, in bytes.

- The DAV\_5162 architecture returns 32 bytes, and other architectures return 256 bytes.
- If the context is null or the platform information fails to be obtained, **0** is returned.

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
compileInfo->vRegSize = GetVRegSize(context_);
```
