# GetNddmaDcacheSize

## Function

Obtains the size of the platform NDDMA DCache, in bytes.

## Prototype

```cpp
template <typename T>
uint32_t GetNddmaDcacheSize([[maybe_unused]] const T *context)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| context | Input| Tiling context, which is of type `TilingContext` or `TilingParseContext`.|

## Returns

`uint32_t`: size of the platform NDDMA DCache (currently 8192 bytes).

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
compileInfo->nddmaDcacheSize = GetNddmaDcacheSize(context_);
```
