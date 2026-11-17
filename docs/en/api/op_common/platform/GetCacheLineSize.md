# GetCacheLineSize

## Function

Obtains the size of the platform cache line, in bytes.

## Prototype

```cpp
template <typename T>
uint32_t GetCacheLineSize([[maybe_unused]] const T *context)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| context | Input| Tiling context, which is of type `TilingContext` or `TilingParseContext`.|

## Returns

`uint32_t`: size of the platform cache line (currently 256 bytes).

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
compileInfo->cacheLineSize = GetCacheLineSize(context_);
```
