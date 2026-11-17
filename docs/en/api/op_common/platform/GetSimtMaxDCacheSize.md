# GetSimtMaxDCacheSize

## Function

Obtains the maximum SIMT DCache size on the platform, in bytes.

## Prototype

```cpp
template <typename T>
uint32_t GetSimtMaxDCacheSize([[maybe_unused]] const T *context)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| context | Input| Tiling context, which is of type `TilingContext` or `TilingParseContext`.|

## Returns

`uint32_t`: maximum SIMT DCache size on the platform (currently 128 \* 1024B = 131072 bytes).

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
compileInfo->simtMaxDCacheSize = GetSimtMaxDCacheSize(context_);
```
