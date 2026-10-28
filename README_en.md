<div align="center">

# opbase

English | [简体中文](./README.md)

[![CANNcommunity](https://img.shields.io/badge/docs-Community_Operator_List-brightgreen.svg?style=flat)](https://www.hiascend.com/document/redirect/CannCommunityOplist)
[![repo](https://img.shields.io/badge/docs-Project_Documentation-green.svg?style=flat)](docs)
[![license](https://img.shields.io/badge/license-CANN_Open_2.0-blue.svg)](LICENSE)
[![contributing](https://img.shields.io/badge/Contribution_Guide-teal)](CONTRIBUTING_en.md)
[![SIG](https://img.shields.io/badge/SIG-ops_basic-yellow)](https://gitcode.com/cann/community/blob/master/CANN/sigs/ops-basic/README.md)
[![Zread](https://img.shields.io/badge/Zread-Ask_AI-_.svg?style=flat&color=0052D9&labelColor=000000&logo=data%3Aimage%2Fsvg%2Bxml%3Bbase64%2CPHN2ZyB3aWR0aD0iMTYiIGhlaWdodD0iMTYiIHZpZXdCb3g9IjAgMCAxNiAxNiIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTQuOTYxNTYgMS42MDAxSDIuMjQxNTZDMS44ODgxIDEuNjAwMSAxLjYwMTU2IDEuODg2NjQgMS42MDE1NiAyLjI0MDFWNC45NjAxQzEuNjAxNTYgNS4zMTM1NiAxLjg4ODEgNS42MDAxIDIuMjQxNTYgNS42MDAxSDQuOTYxNTZDNS4zMTUwMiA1LjYwMDEgNS42MDE1NiA1LjMxMzU2IDUuNjAxNTYgNC45NjAxVjIuMjQwMUM1LjYwMTU2IDEuODg2NjQgNS4zMTUwMiAxLjYwMDEgNC45NjE1NiAxLjYwMDFaIiBmaWxsPSIjZmZmIi8%2BCjxwYXRoIGQ9Ik00Ljk2MTU2IDEwLjM5OTlIMi4yNDE1NkMxLjg4ODEgMTAuMzk5OSAxLjYwMTU2IDEwLjY4NjQgMS42MDE1NiAxMS4wMzk5VjEzLjc1OTlDMS42MDE1NiAxNC4xMTM0IDEuODg4MSAxNC4zOTk5IDIuMjQxNTYgMTQuMzk5OUg0Ljk2MTU2QzUuMzE1MDIgMTQuMzk5OSA1LjYwMTU2IDE0LjExMzQgNS42MDE1NiAxMy43NTk5VjExLjAzOTlDNS42MDE1NiAxMC42ODY0IDUuMzE1MDIgMTAuMzk5OSA0Ljk2MTU2IDEwLjM5OTlaIiBmaWxsPSIjZmZmIi8%2BCjxwYXRoIGQ9Ik0xMy43NTg0IDEuNjAwMUgxMS4wMzg0QzEwLjY4NSAxLjYwMDEgMTAuMzk4NCAxLjg4NjY0IDEwLjM5ODQgMi4yNDAxVjQuOTYwMUMxMC4zOTg0IDUuMzEzNTYgMTAuNjg1IDUuNjAwMSAxMS4wMzg0IDUuNjAwMUgxMy43NTg0QzE0LjExMTkgNS42MDAxIDE0LjM5ODQgNS4zMTM1NiAxNC4zOTg0IDQuOTYwMVYyLjI0MDFDMTQuMzk4NCAxLjg4NjY0IDE0LjExMTkgMS42MDAxIDEzLjc1ODQgMS42MDAxWiIgZmlsbD0iI2ZmZiIvPgo8cGF0aCBkPSJNNCAxMkwxMiA0TDQgMTJaIiBmaWxsPSIjZmZmIi8%2BCjxwYXRoIGQ9Ik00IDEyTDEyIDQiIHN0cm9rZT0iI2ZmZiIgc3Ryb2tlLXdpZHRoPSIxLjUiIHN0cm9rZS1saW5lY2FwPSJyb3VuZCIvPgo8L3N2Zz4K&logoColor=ffffff)](https://zread.ai/hicann/opbase)

</div>

---

## 🔥Latest News

- [2025/12] Supported KirinX90 and offline compilation.
- [2025/09] Released the opbase project for the first time.

## 🚀 Overview

opbase is a basic framework library that the Compute Architecture for Neural Networks ([CANN](https://hiascend.com/software/cann)) operator library depends on. It provides basic scheduling capabilities and common dependencies, including common header files, structures, and scheduling frameworks. For details about the CANN operator library, visit [ops-math](https://gitcode.com/cann/ops-math), [ops-nn](https://gitcode.com/cann/ops-nn), [ops-cv](https://gitcode.com/cann/ops-cv), and [ops-transformer](https://gitcode.com/cann/ops-transformer) to obtain the operator source code.

<img src="docs/en/figures/docs_en_figures_architecture.png" alt="Architecture" width="700px" height="320px">

## 📌 Version Mapping

The source code of this project is released with the CANN software version. For details about the mapping between the CANN software version and the project tag, see the version description in the [release repository](https://gitcode.com/cann/release-management).
To ensure smooth source code customization, select the matching CANN version and GitCode tag source code. Using the master branch may cause version mismatch.

## 🛠️ Environment Setup

Before using this project, you need to install the NPU driver, CANN package, and other required components. For details, see [Environment Deployment](docs/en/install/quick_install.md).

## ⬇️ Source Code Download

After the environment is set up, run the command below to download the branch source code that matches the CANN version. Replace \$\{tag\_version\} with the branch tag name.

> Note: If the matching branch source code already exists in the environment, **skip this step**.

```bash
git clone -b ${tag_version} https://gitcode.com/cann/opbase.git
```

## 📖 Tutorials

- [Quick Start](docs/QUICKSTART.md): You can quickly experience project source code build and local verification (such as UT) from scratch.
- [Advanced Tutorials](docs/README.md): To learn more about the framework APIs, guidelines, and other information required for operator calling and development, visit the Documentation Center.

## 📝 Related Information

- [Directory Structure](docs/en/appendix/dir_structure.md)
- [Contribution Guide](CONTRIBUTING_en.md)
- [Security Statement](SECURITY_en.md)
- [Licenses](LICENSE)
- [SIG](https://gitcode.com/cann/community/tree/master/CANN/sigs/ops-basic)

------

This project's functionality and documentation are under continuous development and refinement. We invite you to follow the latest releases.

- **Feedback**: Submit queries or report bugs through GitCode [Issues](https://gitcode.com/cann/opbase/issues).
- **Interaction**: Participate in [discussion](https://gitcode.com/cann/opbase/discussions) on GitCode.
