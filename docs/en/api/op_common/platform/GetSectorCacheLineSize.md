# GetSectorCacheLineSize

## Function

Obtains the size of the platform sector cache line, in bytes. The value is half of the cache line size.

## Prototype

```cpp
template <typename T>
uint32_t GetSectorCacheLineSize([[maybe_unused]] const T *context)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| context | Input| Tiling context, which is of type `TilingContext` or `TilingParseContext`.|

## Returns

`uint32_t`: size of the platform sector cache line (currently 128 bytes).

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
compileInfo->sectorCacheLineSize = GetSectorCacheLineSize(context_);
```
