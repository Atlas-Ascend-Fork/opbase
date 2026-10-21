# EZ3004 Not\_Supported\_Operator

## Symptom

The following is error format. The meanings of the placeholders %s in sequence are: operator information library, operator name, operator type, error cause.

```text
The operator information library %s does not support the operator %s (operator type: %s). Cause: %s.
```

Error example:

```text
The operator information library AIcoreEngine does not support the operator Conv2D_1 (operator type: Conv2D). Cause: data type DT_FLOAT of input x is not supported.
```

## Solution

1. If a built-in operator is used, upgrade to the CANN operator package that supports this specification.
2. If a custom operator is used, register and use the custom operator with the required specification.
