# EZ3005 Not\_Supported\_Operator

## Symptom

The following is error format. The meanings of the placeholders %s in sequence are: operator name, operator type.

```text
No operator information library that supports operator %s (operator type: %s) can be found.
```

Error example:

```text
No operator information library that supports operator Conv2D_1 (operator type: Conv2D) can be found.
```

## Possible Cause

The operator is not supported by the system. Therefore, no hit is found in any operator information library.

## Solution

1. If a built-in operator is used, upgrade to the CANN operator package that supports this operator.
2. If a custom operator is used, develop and register the operator.
