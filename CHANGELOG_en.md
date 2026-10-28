# CHANGELOG

> This document records the important changes in each version. Versions are listed in reverse chronological order.

## v8.5.0-beta.1

Release date: 2025-12-30

The first Beta version of opbase, v8.5.0-beta.1, is now available.
This version introduces multiple new features, bug fixes, and performance improvements, and is still in the testing phase.
We sincerely welcome feedback from the community to further improve the stability and functional completeness of opbase.
For usage instructions, refer to the [official documentation](https://gitcode.com/cann/opbase/blob/master/README_en.md).

### 🔗 Version Address

[CANN 8.5.0-beta 1](https://ascend.devcloud.huaweicloud.com/cann/run/software/8.5.0-beta.1/)

```text
The version directory structure is as follows:
├── aarch64                 # ARM CPU
│   ├── ops                  # ops operator package directory, used for archiving operator subpackages
│   ├── ...
├── x86_64                   # x86 CPU
│   ├── ops                  # ops operator package directory, used for archiving operator subpackages
│   ├── ...
```

### 📌 Version Compatibility

**CANN open-source subpackage version compatibility**

| CANN subpackage version              | Source code tag                                                                                                             | Compatible CANN version |
| ------------------------------------ | ------------------------------------------------------------ | ------------------- |
| cann-opbase 8.5.0-beta.1             | [v8.5.0-beta.1](https://gitcode.com/cann/opbase/tags/v8.5.0-beta.1) | CANN   8.5.0-beta.1 |
| cann-oam-tools   8.5.0-beta.1        | [v8.5.0-beta.1](https://gitcode.com/cann/oam-tools/tags/v8.5.0-beta.1) | CANN   8.5.0-beta.1 |
| cann-asc-tools   8.5.0-beta.1        | [v8.5.0-beta.1](https://gitcode.com/cann/asc-tools/tags/v8.5.0-beta.1) | CANN   8.5.0-beta.1 |
| cann-asc-devkit   8.5.0-beta.1       | [v8.5.0-beta.1](https://gitcode.com/cann/asc-devkit/tags/v8.5.0-beta.1) | CANN   8.5.0-beta.1 |
| cann-pto-isa   8.5.0-beta.1          | [v8.5.0-beta.1](https://gitcode.com/cann/pto-isa/tags/v8.5.0-beta.1) | CANN   8.5.0-beta.1 |
| cann-ge-compiler   8.5.0-beta.1      | [v8.5.0-beta.1](https://gitcode.com/cann/ge/tags/v8.5.0-beta.1) | CANN   8.5.0-beta.1 |
| cann-ge-executor   8.5.0-beta.1      | [v8.5.0-beta.1](https://gitcode.com/cann/ge/tags/v8.5.0-beta.1) | CANN   8.5.0-beta.1 |
| cann-graph-autofusion   8.5.0-beta.1 | [v8.5.0-beta.1](https://gitcode.com/cann/graph-autofusion/tags/v8.5.0-beta.1) | CANN   8.5.0-beta.1 |
| cann-metadef   8.5.0-beta.1          | [v8.5.0-beta.1](https://gitcode.com/cann/metadef/tags/v8.5.0-beta.1) | CANN   8.5.0-beta.1 |
| cann-dflow-executor   8.5.0-beta.1   | [v8.5.0-beta.1](https://gitcode.com/cann/ge/tags/v8.5.0-beta.1) | CANN   8.5.0-beta.1 |
| cann-hcomm   8.5.0-beta.1            | [v8.5.0-beta.1](https://gitcode.com/cann/hcomm/tags/v8.5.0-beta.1) | CANN   8.5.0-beta.1 |
| cann-npu-runtime   8.5.0-beta.1      | [v8.5.0-beta.1](https://gitcode.com/cann/runtime/tags/v8.5.0-beta.1) | CANN   8.5.0-beta.1 |

### 🚀 Key Features

- [Engineering capability] Offline compilation is supported. ([#57](https://gitcode.com/cann/opbase/pull/57))
- [Documentation optimization] Added the offline compilation description document. ([#56](https://gitcode.com/cann/opbase/pull/56))

### 🐛 Bug Fixes

- An error is reported when opbase is compiled on an on-site machine in the offline state. ([Issue6](https://gitcode.com/cann/opbase/issues/6))
- The error "non-constant-expression cannot be narrowed" is reported during obpase package compilation. ([Issue22](https://gitcode.com/cann/opbase/issues/22))
