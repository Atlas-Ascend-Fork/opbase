# GetSimtMaxThreadNum

## Function

Obtains the maximum number of SIMT threads on the platform.

## Prototype

```cpp
template <typename T>
uint32_t GetSimtMaxThreadNum([[maybe_unused]] const T *context)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| context | Input| Tiling context, which is of type `TilingContext` or `TilingParseContext`.|

## Returns

`uint32_t`: maximum number of SIMT threads on the platform (currently 2048).

## Restrictions

None

## Examples

The following code is for reference only and should not be copied directly for execution:

```cpp
compileInfo->simtMaxThreadNum = GetSimtMaxThreadNum(context_);
```
