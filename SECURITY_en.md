# Security Statement

## Recommendations on the Running User

For security reasons, you are advised not to use the root account or other administrator accounts to run any commands, and follow the least privilege principle.

## File Permission Control

- Set the umask value of the running system to 0027 or later on the host (including the host machine) and in containers. This ensures that the default maximum permission is **750** for newly created folders and **640** for newly created files.
- Apply proper permission control and other security measures to sensitive content such as personal privacy data, commercial assets, source files, and various files saved during operator development. For example, for permission control on the installation directory of this project and on public input data files, refer to the recommended maximum permissions in [A-Recommended Maximum Permissions for Files (Folders) in Different Scenarios](#a-recommended-maximum-permissions-for-files-folders-in-different-scenarios).
- During running, operators may cache operator compilation files in the `kernel_meta_*` folder under the running directory to accelerate subsequent operator calls. You can perform permission control on the generated files as required.
- Perform proper permission control during installation and usage. Set permissions by referring to [A-Recommended Maximum Permissions for Files (Folders) in Different Scenarios](#a-recommended-maximum-permissions-for-files-folders-in-different-scenarios).

## Build Security Statement

When building and installing this project from source code, you need to compile it by yourself, and intermediate files are generated during compilation. After the compilation is complete, perform proper permission control on these intermediate files to ensure file security.

## Running Security Statement

- Write operator invocation scripts based on the resource status of the running environment. If the script does not match the resource status, errors may occur and the process may exit unexpectedly. Examples include the space used for generating input data or benchmark calculation results exceeding the memory capacity limit, and the data saved locally by the script exceeding the disk space.
- When an operator runs abnormally, the process exits and error information is printed. Locate the specific error cause based on the error message, for example, by setting operators to run in synchronous mode or viewing log files.
- When operators are invoked through [PyTorch](https://gitee.com/ascend/pytorch), version mismatch may cause running errors. For details, see the [PyTorch security statement](https://gitee.com/ascend/pytorch#%E5%AE%89%E5%85%A8%E5%A3%B0%E6%98%8E).

## Public Network Address Statement

The public network addresses used in the code of this project are declared as follows:

|      Type  |      Open-source code address   |       File name    |   Public IP address/URL/domain name/email address/archive address   |  Purpose   |
| :------------: |:------------:|:----------------------------| :----------------------------------------------- |:-----------------------------------------|
|  Dependency  | N/A  | cann-cmake (makeself-fetch.cmake) | <https://gitcode.com/cann-src-third-party/makeself/releases/download/release-2.5.0-patch1.0/makeself-release-2.5.0-patch1.tar.gz> | Download the makeself source code from GitCode as a build dependency |
|  Dependency  | N/A  | cann-cmake (protobuf.cmake) | <https://cann-3rd.obs.cn-north-4.myhuaweicloud.com/protobuf/protobuf-25.1.tar.gz> | Download the protobuf source code from the OBS public repository as a build dependency |
|  Dependency  | N/A  | cann-cmake (gtest.cmake) | <https://cann-3rd.obs.cn-north-4.myhuaweicloud.com/googletest/googletest-1.14.0.tar.gz> | Download the googletest source code from the OBS public repository as a build dependency |
|  Dependency  | N/A  | cann-cmake (eigen.cmake) | <https://gitcode.com/cann-src-third-party/eigen/releases/download/5.0.0-h0.trunk/eigen-5.0.0.tar.gz> | Download the eigen source code from GitCode as a build dependency |
|  Dependency  | N/A  | cann-cmake (json.cmake) | <https://cann-3rd.obs.cn-north-4.myhuaweicloud.com/json/json-3.11.3.tar.gz> | Download the json source code from the OBS public repository as a build dependency |
|  Dependency  | N/A  | cann-cmake (boost.cmake) | <https://cann-3rd.obs.cn-north-4.myhuaweicloud.com/boost/boost_1_87_0.tar.gz> | Download the boost source code from the OBS public repository as a build dependency |
|  Dependency  | N/A  | cann-cmake (mockcpp.cmake) | <https://cann-3rd.obs.cn-north-4.myhuaweicloud.com/mockcpp/mockcpp-2.7.tar.gz> | Download the mockcpp source code from the OBS public repository as a build dependency |
|  Dependency  | N/A  | cmake | <https://apt.kitware.com/keys/kitware-archive-latest.asc> | Download the cmake software from kitware as a build dependency |
|  Dependency  | N/A  | cmake | <https://apt.kitware.com/ubuntu/> | Download the cmake software from kitware as a build dependency |

---

## Vulnerability Mechanism Description

[Vulnerability management](https://gitcode.com/cann/community/blob/master/security/security.md)

## Appendix

### A-Recommended Maximum Permissions for Files (Folders) in Different Scenarios

| Type           | Maximum Linux permission (reference) |
| -------------- | ---------------  |
| User home directory                        |   750 (rwxr-x---)            |
| Program files (including script files and library files)       |   550 (r-xr-x---)             |
| Program file directory                      |   550 (r-xr-x---)            |
| Configuration file                          |  640 (rw-r-----)             |
| Configuration file directory                      |   750 (rwxr-x---)            |
| Log files (logging completed or archived)        |  440 (r--r-----)             |
| Log files (being written)                |    640 (rw-r-----)           |
| Log file directory                      |   750 (rwxr-x---)            |
| Debug files                         |  640 (rw-r-----)         |
| Debug file directory                     |   750 (rwxr-x---)  |
| Temporary file directory                      |   750 (rwxr-x---)   |
| Maintenance and upgrade file directory                  |   770 (rwxrwx---)    |
| Service data files                      |   640 (rw-r-----)    |
| Service data file directory                  |   750 (rwxr-x---)      |
| Key components, private keys, certificates, and ciphertext file directory    |  700 (rwx—----)      |
| Key components, private keys, certificates, and encrypted ciphertext        | 600 (rw-------)      |
| Encryption/decryption interfaces and scripts            |   500 (r-x------)        |
