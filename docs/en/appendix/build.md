# build Parameters

## Introduction

build.sh is the build script of this project. It resides in the project root directory by default and automatically compiles, links, and configures the source code, and finally generates executable files, library files, or other targets that can be installed or run directly. Specifically, the script implements multiple functions by configuring different parameters, including building the opbase package and running unit tests.

## Usage

1. **Setting environment variables**

   Ensure that the environment variables take effect before running the build command, especially in a newly opened terminal window.

   ```bash
   # Default-path installation, with the root user as an example
   source /usr/local/Ascend/cann/set_env.sh
   ```

2. **Command format**

   Taking the source code build scenario as an example, the build command is as follows:

   ```bash
   bash build.sh
   ```

   For details about all parameters, see [Parameters](#parameters). Choose proper parameters as required.

## Parameters

build.sh supports multiple functions. Run the following command to view all function parameters:

```bash
bash build.sh --help
```

The parameters are described in the following table:

| Parameter              | Optional/Required| Description                                                       |
|---------------------|-------|------------------------------------------------------------|
| -j${n}              | Optional    | Specifies the number of build threads. ${n} indicates the number of threads, with the default value 8 (for example, -j8). If the number of threads exceeds the number of CPU cores, it is automatically adjusted to the number of CPU cores. |
| --verbose,-v        | Optional    | Displays CMake build configuration information.                                             |
| -O${n}              | Optional    | Specifies the build optimization level. The options are O0/O1/O2/O3 (for example, -O3). ${n} indicates the optimization level.                 |
| -u                  | Optional    | Enables the unit test (UT) compilation mode and compiles all UT targets.                                   |
| --help,-h           | Optional    | Prints the script usage help information.                                                |
| --build-type        | Optional    | Enables the debug mode. The options are Release/Debug, with the default value Release.                                                    |
| --cov               | Optional    | Runs UT and generates a code coverage report. This parameter depends on lcov. For details, see [QUICKSTART](../../QUICKSTART.md#执行ut).                    |
| --noexec            | Optional    | Compiles only the unit test binary files and does not automatically execute the compiled UT executables.                             |
| --make_clean    | Optional    | Performs a full cleanup (deletes all build-related files). After the execution, the script exits.                              |
| --cann_3rd_lib_path | Optional    | Specifies the directory storing third-party libraries in the offline build scenario.                            |
| --pkg               | Optional    | Generates installation packages.                           |
| --pkg-type=`<TYPE>` | Optional    | Specifies the installation package type. The options are run/rpm/deb/all, with the default value run. |
| --asan              | Optional    | Enables AddressSanitizer (ASAN) memory detection on the host side.                            |
