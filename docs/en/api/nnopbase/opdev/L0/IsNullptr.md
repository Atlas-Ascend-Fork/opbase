# IsNullptr

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
