# Quick Start

Before quickly experiencing the project, refer to the README of this project to complete environment preparation and source code download. This section does not repeat those steps.

Note: View all command parameters through `bash build.sh --help`. For the meanings of the parameters, see [build Parameters](./en/appendix/build.md#parameters).

## Source Code Build

### Install Build Tools

> **Note**: For CANNLab or Docker environments, the build tools required for source code compilation are installed by default. You do not need to install them manually and can skip this section.

First, ensure that the base library dependencies of the compilation environment are installed and meet the version requirements.

- python >= 3.7.0
- gcc >= 7.3.0
- g++ >= 7.3.0
- cmake >= 3.16.0
- pigz (optional; installing it improves the packaging speed. Version 2.4 or later is recommended.)
- patch >= 2.7 (used to patch third-party source code, such as protobuf)
- dos2unix
- git

The dependencies above can be installed in one click through install_deps.sh in the project root directory. The command is as follows. If your system is not supported, adapt the file accordingly.

```bash
bash install_deps.sh
```

### Install Third-Party Dependencies

> **Note**: For CANNLab or Docker environments, the network is available by default, and third-party dependencies are automatically downloaded during compilation. You do not need to install them manually and can skip this section.

The third-party open-source software that this project depends on during compilation is listed as follows:

| Open-Source Software | Version    | Download Address                                                   |
| -------------------- | ---------- | ------------------------------------------------------------------ |
| json                 | 3.11.3     | [json-3.11.3.tar.gz](https://cann-3rd.obs.cn-north-4.myhuaweicloud.com/json/json-3.11.3.tar.gz) |
| makeself             | 2.5.0      | [makeself-release-2.5.0-patch1.tar.gz](https://gitcode.com/cann-src-third-party/makeself/releases/download/release-2.5.0-patch1.0/makeself-release-2.5.0-patch1.tar.gz) |
| eigen                | 5.0.0      | [eigen-5.0.0.tar.gz](https://gitcode.com/cann-src-third-party/eigen/releases/download/5.0.0-h0.trunk/eigen-5.0.0.tar.gz) |
| protobuf             | 25.1       | [protobuf-25.1.tar.gz](https://cann-3rd.obs.cn-north-4.myhuaweicloud.com/protobuf/protobuf-25.1.tar.gz) |
| abseil-cpp           | 20230802.1 | [abseil-cpp-20230802.1.tar.gz](https://cann-3rd.obs.cn-north-4.myhuaweicloud.com/abseil-cpp/abseil-cpp-20230802.1.tar.gz) |
| cann-cmake           | master-057 | [cmake-master-057.tar.gz](https://raw.gitcode.com/cann/cmake/archive/refs/heads/master-057.tar.gz) |

If your compilation environment can access the network, see [Online Compilation](#online-compilation). The compilation script automatically downloads third-party software online. Otherwise, see [Offline Compilation](#offline-compilation) to manually download third-party software.

### Online Compilation

If you compile in an environment with Internet access, third-party dependencies are automatically installed during compilation, and no manual installation is required. The specific process is as follows:

1. **Compile the opbase Package**

    Go to the project root directory and run the following compilation command:

    ```bash
    bash build.sh --pkg [-j${n}]
    # Example: bash build.sh --pkg -j16
    ```

    - -j (optional): specifies the number of compilation threads. The default value is 8, which speeds up compilation. If the value exceeds the number of CPU cores, it is automatically adjusted to the number of CPU cores.

    If the following message is displayed, the compilation is successful.

    ```bash
    Self-extractable archive "cann-opbase_${cann_version}_linux-${arch}.run" successfully created.
    ```

    After successful compilation, the run package is stored in the build_out directory.

2. **Install the opbase Package**

    ```bash
    ./build_out/cann-opbase_${cann_version}_linux-${arch}.run --full --install-path=${install_path}
    ```

    ${install_path} indicates the specified installation path. If you do not specify it, the default installation path is `/usr/local/Ascend`. If you specify it, the package is generally installed in the ${install_path} directory.

3. **(Optional) Uninstall the opbase Package**

    ```bash
    # Uninstall command
    "${install_path}/cann/share/info/opbase/script/uninstall.sh"
    ```

### Offline Compilation

If you compile in an environment without Internet access, prepare the required third-party software in advance and then compile the source code. The specific process is as follows:

1. **Download Third-Party Dependencies**

    Download the third-party software in an environment with Internet access in advance. The following methods are available. Choose one as needed.

    - Method 1: Manually download based on the table in [Third-Party Dependencies](#install-third-party-dependencies). If downloading from other addresses, ensure that the version numbers are consistent.

    - Method 2: Download in one click through the [third_lib_download.py](../scripts/tools/third_lib_download.py) script. The script is in the `scripts/tools/` directory of this project. Download the script and run the following command:

        ```bash
        python ${scripts_dir}/third_lib_download.py
        ```

        ${scripts_dir} indicates the path where the script is stored. The downloaded third-party software packages are stored in the directory where the script is located by default.

2. **Compile the Operator Package**

    Upload the downloaded third-party software to the offline environment. You can store it in the `third_party` directory or a custom directory. **The former is recommended, as its compilation command is the same as that in the online compilation scenario.**

    - **third\_party Directory** (Recommended)

      Create the `third_party` directory in the project root directory (skip this step if it already exists), and copy the third-party software to this specified directory. In this case, the compilation command is the same as the online compilation command:

      ```bash
      bash build.sh --pkg [-j${n}]
      # Example: bash build.sh --pkg -j16
      ```

    - **Custom Directory**

      Create the `${cann_3rd_lib_path}` directory at any location in the offline environment, copy the third-party software to this directory, and ensure that the directory is accessible.

        ```bash
      mkdir -p ${cann_3rd_lib_path}
        ```

      In this case, additionally add `--cann_3rd_lib_path=${cann_3rd_lib_path}` to the online compilation command to specify the third-party software path. Assume that the path is `/path/cann_3rd_lib_path`. The compilation command is as follows:

      ```bash
      bash build.sh --pkg --cann_3rd_lib_path=${cann_3rd_lib_path} [-j${n}]
      # bash build.sh --pkg --cann_3rd_lib_path=/path/cann_3rd_lib_path -j16
      # bash build.sh --pkg --cann_3rd_lib_path=/path/cann_3rd_lib_path
      ```

3. **Install/Uninstall the Operator Package**

    The operator package compiled in the offline and online scenarios is the same. By default, it is stored in the build_out directory under the project root directory, and the installation and uninstallation commands are also the same. For details, see [Online Compilation](#online-compilation).

## Local Verification

After the source code package is deployed, you can run UT/ST test cases through build.sh in the project root directory to verify that the project functions are normal.

> **Note**: Running UT test cases depends on the googletest unit test framework. For details, see the [googletest website](https://google.github.io/googletest/advanced.html#running-a-subset-of-the-tests).

### Run UT

#### Compile and Run UT

```bash
# Compile and run all UT test cases.
bash build.sh -u
# Compile all UT test cases but do not run them.
bash build.sh -u --noexec
# Run UT and view the coverage.
bash build.sh -u --cov
```

> **Note**: `--cov` is used to generate code coverage reports and depends on the lcov tool (including genhtml). If it is not installed, install it through `sudo apt-get install -y lcov` (Debian/Ubuntu) or `sudo dnf install -y lcov` (RHEL/openEuler), or install it in one click through install_deps.sh in the project root directory. For details about base dependencies, see [Install Basic Dependencies](./en/install/quick_install.md#install-basic-dependencie).

If the following content is displayed after execution, the execution is successful.

```bash
Global Global test environment tear-down
[==========] ${n} tests from ${m} test suites ran. (${x} ms total)
[  PASSED  ] ${n} tests.
Execute ops_base_ut successful.
```

#### Run Compiled UT Tests Separately

If UT compilation is complete (through `bash build.sh -u` or `bash build.sh -u --noexec`), you can directly run the generated test binary without recompiling.

```bash
# Set environment variables.
source ${ASCEND_HOME_PATH}/bin/setenv.bash
export LD_LIBRARY_PATH="$(pwd)/build/:$LD_LIBRARY_PATH"

# Run nnopbase UT tests.
(cd build/tests/nnopbase/ut && ./nnopbase_utest)

# Run op_common UT tests.
(cd build/tests/op_common && ./op_common_utest)
```

`${ASCEND_HOME_PATH}` is the CANN installation path, which is `/usr/local/Ascend/cann` by default.

> **Note**: googletest supports filtering specified test cases through command-line parameters. For example: `./nnopbase_utest --gtest_filter=TestSuiteName.TestCaseName`. For more usage, see the [googletest documentation](https://google.github.io/googletest/advanced.html#running-a-subset-of-the-tests).
