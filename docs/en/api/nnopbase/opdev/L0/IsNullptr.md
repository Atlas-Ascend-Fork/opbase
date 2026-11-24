# IsNullptr

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Not supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference products: Supported
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas training products: Supported
<!-- end id6 -->

## Function

Checks whether the input pointer is null. Returns `true` and prints an error log if the pointer is null; otherwise, returns `false`.

## Prototype

```cpp
static inline bool IsNullptr(const aclTensor *tensor, const char *name)
```

```cpp
static inline bool IsNullptr(const aclTensorList *tensorList, const char *name)
```

```cpp
static inline bool IsNullptr(const aclScalar *scalar, const char *name)
```

```cpp
static inline bool IsNullptr(const aclIntArray *intArr, const char *name)
```

```cpp
static inline bool IsNullptr(const aclBoolArray *boolArr, const char *name)
```

```cpp
static inline bool IsNullptr(const aclFloatArray *floatArr, const char *name)
```

## Parameters

| Parameter| Input/Output| Description|
| --- | --- | --- |
| tensor | Input| Pointer to be checked. The supported types are aclTensor*, aclTensorList*, aclScalar*, aclIntArray*, aclBoolArray*, and aclFloatArray*. |
| name | Input| Identifier of the pointer to be checked. If the checked pointer is null, the identifier (name) is output in the printed error log. |

## Returns

Returns a `bool` value: `true` if the pointer is determined to be `nullptr`, and `false` otherwise.

## Restrictions

None

## Examples

```cpp
#define OP_CHECK_NULL(param, retExpr) \
  if (IsNullptr(param, #param)) { \
    retExpr; \
  }
```
