/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "op_info_serialize.h"
#include "dump/adump_pub.h"
#include "mmpa/mmpa_api.h"
#include <elf.h>
#include <set>
#include <mutex>
#include <fstream>
#include <string_view>
#include <vector>
#include "opdev/op_log.h"
#include "opdev/op_errno.h"
#include "opdev/platform.h"
#include "platform/platform_info.h"
#include "tiling_context_to_json.h"
#include "ini_parse.h"

namespace aclnnOpInfoRecord {
enum class OpInfoType { OP_INFO = 0, OP_INFO_FOR_DEBUG, OP_INFO_FOR_OPCOMPILE };

namespace {
std::mutex g_opInfoStatisticsLck;
std::set<nlohmann::json> g_opInfoStatistics;
std::set<nlohmann::json> g_opInfoStatisticsDebug;
std::set<nlohmann::json> g_opInfoStatisticsOpcompile;
constexpr char FILE_TYPE_SPLIT_CHAR = '.';
constexpr std::string_view RESOURCE_ID_SECTION_NAME = ".ascend.meta";
constexpr uint16_t RESOURCE_ID_TLV_TYPE = 6U;
constexpr uint16_t RESOURCE_ID_TLV_LENGTH = 64U;

std::set<std::string> g_opTypeWhiteList = {
    "FlashAttentionScore",
    "FlashAttentionScoreGrad",
    "MatMulV2",
    "MatMul",
    "BatchMatMulV2",
    "BatchMatMul",
    "Transpose",
    "TransData",
    "GeluGrad",
    "Gelu",
    "FastGelu",
    "FastGeluGrad",
    "SoftmaxV2",
    "SoftmxGrad",
    "LayerNorm",
    "LayerNormXBackpropV2",
    "LayerNormXBackpropV3",
    "LayerNormBetaGammaBackpropV2",
    "LayerNormV3",
    "Renorm",
};

std::set<std::string> g_opTypeBlackList = {"ReduceSum", "ReduceMin",       "ReduceMax", "ArgMinWithValue",
                                           "GatherV2",  "ArgMaxWithValue", "Range",     "FusedInferAttentionScore"};

std::set<std::string> g_opTypeBlackList950 = {"FusedInferAttentionScore"};

constexpr int JSON_INDENT = 2;

int32_t ReadJsonInfo(const std::string& binaryInfoPath, nlohmann::json& binaryInfoConfig)
{
    char opInfoPath[MMPA_MAX_PATH] = {0};
    const int32_t ret = mmRealPath(binaryInfoPath.c_str(), opInfoPath, MMPA_MAX_PATH);
    if (ret != EN_OK) {
        OP_LOGW("Get jsonfile path for %s failed.", binaryInfoPath.c_str());
        return ACLNN_ERR_INNER_LOAD_JSON_FAILED;
    }
    OP_LOGI("Reading ascendc operator jsonfile path: %s", opInfoPath);
    std::ifstream ifs(opInfoPath);
    if (!ifs.is_open()) {
        return ACLNN_ERR_INNER_LOAD_JSON_FAILED;
    }
    try {
        ifs >> binaryInfoConfig;
        ifs.close();
    } catch (const nlohmann::json::exception& e) {
        OP_LOGE(ACLNN_ERR_INNER_LOAD_JSON_FAILED, "Read jsonfile [%s] failed, reason %s.", opInfoPath, e.what());
        ifs.close();
        return ACLNN_ERR_INNER_LOAD_JSON_FAILED;
    }
    OP_LOGI("Read jsonfile [%s] successfully.", opInfoPath);
    return 0;
}

std::string GetFirstShapeStr(const nlohmann::json& j)
{
    if (j.is_null()) {
        return "";
    }
    nlohmann::json tmpJ = j;
    if (j.is_array()) {
        if (j.size() < 1) {
            return "";
        }
        if (j[0].is_null()) {
            return "";
        }
        tmpJ = j[0];
    }
    OP_LOGD("[%s] try to get first shape str!", j.dump().c_str());
    std::string str = tmpJ["dtype"].get<std::string>() + "_" + tmpJ["format"].get<std::string>() + "_";
    for (size_t idx = 0UL; idx < tmpJ["shape"].size(); idx++) {
        str += std::to_string(tmpJ["shape"][idx].get<int64_t>()) + "_";
    }
    return str;
}

int32_t DumpJsonToFile(const nlohmann::json& opJson, const OpInfoType type)
{
    static uint32_t cnt = 0;
    static uint32_t cntDebug = 0;
    static uint32_t cntOpcompile = 0;
    if (opJson.is_null()) {
        return 0;
    }
    // input 首节点为dynamic需要处理
    if (!opJson["inputs"].is_array()) {
        OP_LOGE(ACLNN_ERR_INNER, "DumpJsonToFile cannot record node[%s] because the input quantity is null!",
                opJson["op_type"].get<std::string>().c_str());
        return 0;
    }
    std::string fileName = opJson["op_type"].get<std::string>() + "_";
    if (opJson["inputs"].size() >= 1) {
        fileName += GetFirstShapeStr(opJson["inputs"][0]);
    }
    if (type == OpInfoType::OP_INFO) {
        fileName += std::to_string(cnt++) + ".json";
        fileName = std::to_string(getpid()) + "/" + fileName;
    } else if (type == OpInfoType::OP_INFO_FOR_DEBUG) {
        fileName += std::to_string(cntDebug++) + ".json";
        fileName = std::to_string(getpid()) + "_debug/" + fileName;
    } else {
        fileName += std::to_string(cntOpcompile++) + ".json";
        fileName = std::to_string(getpid()) + "_opcompile/" + fileName;
    }
    OP_LOGI("DumpJsonToFile start to record json[%s]!", fileName.c_str());
    std::string jStr = opJson.dump(JSON_INDENT);
    if (Adx::AdumpSaveToFile(jStr.c_str(), jStr.size(), fileName.c_str(), Adx::SaveType::OVERWRITE) != 0) {
        OP_LOGE(ACLNN_ERR_INNER, "DumpJsonToFile dump node[%s] json file failed!",
                opJson["op_type"].get<std::string>().c_str());
        return -1;
    }

    OP_LOGI("DumpJsonToFile json[%s] success!", fileName.c_str());
    return 0;
}

int32_t DumpJson(const std::set<nlohmann::json>& gOpInfoStatistics, const OpInfoType type)
{
    int32_t ret = 0;
    int32_t tmpRes = 0;
    for (const auto& item : gOpInfoStatistics) {
        try {
            tmpRes = DumpJsonToFile(item, type);
        } catch (nlohmann::json::exception& e) {
            ret = -1;
        }
        if (ret == 0 && tmpRes == -1) {
            ret = -1;
        }
    }
    return ret;
}

void AddPlatformInfoToJson(nlohmann::json& opJson, const gert::TilingContext* ctx,
                           const aclnnOpInfoRecord::OpCompilerOption& opt)
{
    auto platformInfo = ctx->GetPlatformInfo();
    if (platformInfo != nullptr) {
        const std::string socInfoKey = "SoCInfo";
        const std::string vecCoreCntKey = "vector_core_cnt";
        const std::string cubeCoreCntKey = "cube_core_cnt";
        std::map<std::string, std::string> res;
        if (platformInfo->GetPlatformResWithLock(socInfoKey, res)) {
            // 不存在异常string转化int失败的场景, 依赖组件的fe::PlatFormInfos在保存值时已做约束
            opJson["platform_info"]["vector_core_cnt"] = std::stoi(res[vecCoreCntKey]);
            opJson["platform_info"]["cube_core_cnt"] = std::stoi(res[cubeCoreCntKey]);
            OP_LOGI("Save coreNum[%d, %d] to json.", opJson["platform_info"]["cube_core_cnt"].get<int32_t>(),
                    opJson["platform_info"]["vector_core_cnt"].get<int32_t>());
        }
    }
    opJson["platform_info"]["pcie_through_flag"] = opt.enablePcie;
}

void DumpJsonToOpInfoCompile(const nlohmann::json& opJson)
{
    static op::SocVersion socVersion = op::GetCurrentPlatformInfo().GetSocVersion();
    const std::set<std::string>& blackList = (socVersion != op::SocVersion::ASCEND910B &&
                                              socVersion != op::SocVersion::ASCEND910_93) ?
                                                 g_opTypeBlackList950 :
                                                 g_opTypeBlackList;
    if (blackList.find(opJson["op_type"]) == blackList.cend()) {
        g_opInfoStatisticsOpcompile.emplace(opJson);
    } else {
        OP_LOGD("TilingContextToJson node type[%s] is in black list!", opJson["op_type"].get<std::string>().c_str());
    }
}

void DumpJsonToOpInfo(const nlohmann::json& opJson)
{
    if (g_opTypeWhiteList.find(opJson["op_type"]) == g_opTypeWhiteList.cend()) {
        OP_LOGD("TilingContextToJson node type[%s] not in white list!", opJson["op_type"].get<std::string>().c_str());
    } else {
        g_opInfoStatistics.emplace(opJson);
    }
}

void DumpJsonToOpInfoDebug(const nlohmann::json& opJson) { g_opInfoStatisticsDebug.emplace(opJson); }

bool ReadElfHeaderAndNameTable(const void* const elfBytes, const size_t elfSize, Elf64_Ehdr& header, Elf64_Shdr& names,
                               const char*& nameData)
{
    if (elfBytes == nullptr || elfSize < sizeof(header)) {
        OP_LOGW("Cannot read resource id: image is null or smaller than an ELF64 header");
        return false;
    }
    const auto* const bytes = static_cast<const unsigned char*>(elfBytes);
    const auto containsRange = [elfSize](const uint64_t offset, const uint64_t length) {
        return offset <= elfSize && length <= elfSize - offset;
    };
    if (memcpy_s(&header, sizeof(header), bytes, sizeof(header)) != EOK) {
        OP_LOGW("Cannot read resource id: the ELF header could not be copied");
        return false;
    }
    if (std::memcmp(header.e_ident, ELFMAG, SELFMAG) != 0 || header.e_ident[EI_CLASS] != ELFCLASS64 ||
        header.e_ident[EI_DATA] != ELFDATA2LSB || header.e_ident[EI_VERSION] != EV_CURRENT ||
        header.e_version != EV_CURRENT || header.e_ehsize != sizeof(header) ||
        header.e_shentsize != sizeof(Elf64_Shdr) || header.e_shnum == 0U || header.e_shstrndx == SHN_UNDEF ||
        header.e_shstrndx >= header.e_shnum || header.e_shoff < sizeof(header) ||
        !containsRange(header.e_shoff, static_cast<uint64_t>(header.e_shnum) * sizeof(Elf64_Shdr))) {
        OP_LOGW("Cannot read resource id: not a little-endian ELF64 object with a usable section table");
        return false;
    }
    const auto* const source = bytes + header.e_shoff + header.e_shstrndx * sizeof(names);
    if (memcpy_s(&names, sizeof(names), source, sizeof(names)) != EOK) {
        OP_LOGW("Cannot read resource id: the section name table could not be copied");
        return false;
    }
    if (names.sh_type != SHT_STRTAB || !containsRange(names.sh_offset, names.sh_size)) {
        OP_LOGW("Cannot read resource id: section name table is not a readable string table");
        return false;
    }
    nameData = reinterpret_cast<const char*>(bytes + names.sh_offset);
    return true;
}

bool ParseResourceId(const unsigned char* const data, const size_t sectionSize, std::string_view& found)
{
    size_t offset = 0U;
    while (offset < sectionSize) {
        if (sectionSize - offset < sizeof(uint16_t) * 2U) {
            OP_LOGW("Cannot read resource id: trailing bytes are too short for a TLV head");
            return false;
        }
        uint16_t type = 0U;
        uint16_t length = 0U;
        if (memcpy_s(&type, sizeof(type), data + offset, sizeof(type)) != EOK ||
            memcpy_s(&length, sizeof(length), data + offset + sizeof(type), sizeof(length)) != EOK) {
            OP_LOGW("Cannot read resource id: a TLV head could not be copied");
            return false;
        }
        offset += sizeof(type) + sizeof(length);
        if (length > sectionSize - offset) {
            OP_LOGW("Cannot read resource id: a TLV value runs past the end of .ascend.meta");
            return false;
        }
        if (type == RESOURCE_ID_TLV_TYPE) {
            if (!found.empty()) {
                OP_LOGW("Cannot read resource id: more than one resource id entry");
                return false;
            }
            if (length != RESOURCE_ID_TLV_LENGTH) {
                OP_LOGW("Cannot read resource id: resource id entry has an unexpected length");
                return false;
            }
            const std::string_view value(reinterpret_cast<const char*>(data + offset), length);
            const bool isValidHex = std::all_of(value.begin(), value.end(), [](const char digit) {
                return (digit >= '0' && digit <= '9') || (digit >= 'a' && digit <= 'f');
            });
            if (!isValidHex) {
                OP_LOGW("Cannot read resource id: resource id is not lowercase hexadecimal");
                return false;
            }
            found = value;
        }
        offset += length;
    }
    return true;
}

bool ReadResourceId(const void* const elfBytes, const size_t elfSize, std::string& resourceId)
{
    Elf64_Ehdr header = {};
    Elf64_Shdr names = {};
    const char* nameData = nullptr;
    if (!ReadElfHeaderAndNameTable(elfBytes, elfSize, header, names, nameData)) {
        return false;
    }
    const auto* const bytes = static_cast<const unsigned char*>(elfBytes);
    const auto readSection = [&](const size_t index, Elf64_Shdr& section) {
        const auto* const source = bytes + header.e_shoff + index * sizeof(section);
        return memcpy_s(&section, sizeof(section), source, sizeof(section)) == EOK;
    };
    std::string_view found;
    for (size_t index = 0U; index < header.e_shnum; ++index) {
        Elf64_Shdr section = {};
        if (!readSection(index, section)) {
            OP_LOGW("Cannot read resource id: a section header could not be copied");
            return false;
        }
        if (section.sh_type == SHT_NULL || section.sh_type == SHT_NOBITS || section.sh_name >= names.sh_size) {
            continue;
        }
        const char* const name = nameData + section.sh_name;
        const auto* const end = static_cast<const char*>(std::memchr(name, '\0', names.sh_size - section.sh_name));
        if (end == nullptr || std::string_view(name, end - name) != RESOURCE_ID_SECTION_NAME) {
            continue;
        }
        if ((section.sh_type != SHT_NOTE && section.sh_type != SHT_PROGBITS) || section.sh_offset > elfSize ||
            section.sh_size > elfSize - section.sh_offset) {
            OP_LOGW("Cannot read resource id: section .ascend.meta is not readable metadata");
            return false;
        }
        if (!ParseResourceId(bytes + section.sh_offset, section.sh_size, found)) {
            return false;
        }
    }
    if (found.empty()) {
        OP_LOGI("No resource id: section=%s type=%u", std::string(RESOURCE_ID_SECTION_NAME).c_str(),
                static_cast<unsigned int>(RESOURCE_ID_TLV_TYPE));
        return false;
    }
    resourceId.assign(found);
    return true;
}

std::string ReadBinId(const std::string& binPath)
{
    std::ifstream binFile(binPath, std::ios::binary | std::ios::ate);
    if (!binFile.is_open()) {
        OP_LOGW("Cannot read bin id: failed to open binary file %s", binPath.c_str());
        return "";
    }
    const std::streamoff fileSize = binFile.tellg();
    if (fileSize < 0 || static_cast<uint64_t>(fileSize) > std::numeric_limits<size_t>::max()) {
        OP_LOGW("Cannot read bin id: binary file %s has an invalid size", binPath.c_str());
        return "";
    }
    std::string binBytes(static_cast<size_t>(fileSize), '\0');
    binFile.seekg(0U, std::ios::beg);
    binFile.read(binBytes.data(), static_cast<std::streamsize>(binBytes.size()));
    if (binFile.fail() && !binFile.eof()) {
        OP_LOGW("Cannot read bin id: failed to read binary file %s", binPath.c_str());
        return "";
    }
    std::string binId;
    return ReadResourceId(binBytes.data(), binBytes.size(), binId) ? binId : "";
}

void AddOpInfoToJson(nlohmann::json& opJson, const aclnnOpInfoRecord::OpCompilerOption& opt,
                     const aclnnOpInfoRecord::OpKernelInfo* kernelInfo,
                     const aclnnOpInfoRecord::RuntimeInfo* runtimeInfo)
{
    opJson["impl_mode"] = opt.impl_mode;
    opJson["deterministic"] = (opt.deterministic == 0U) ? "false" : "true";
    opJson["deterministic_level"] = opt.deterministic;
    opJson["bin_id"] = ReadBinId(kernelInfo->bin_info);
    opJson["bin_type"] = kernelInfo->bin_type;
    if (runtimeInfo != nullptr) {
        opJson["kernel_name"] = runtimeInfo->kernelName;
        opJson["tiling_args_index"] = runtimeInfo->tilingArgsIndex;
    }
}
} // namespace

int32_t OpInfoDump(void)
{
    int32_t ret = 0;
    std::lock_guard<std::mutex> lck(g_opInfoStatisticsLck);
    OP_LOGI("Op info record start to dump %zu json!", g_opInfoStatistics.size());
    ret = DumpJson(g_opInfoStatistics, OpInfoType::OP_INFO);
    g_opInfoStatistics.clear();
    OP_LOGI("Op info record start to dump %zu op compile json!", g_opInfoStatisticsOpcompile.size());
    ret = DumpJson(g_opInfoStatisticsOpcompile, OpInfoType::OP_INFO_FOR_OPCOMPILE);
    g_opInfoStatisticsOpcompile.clear();
    OP_LOGI("Op info record start to dump %zu debug json!", g_opInfoStatisticsDebug.size());
    ret = DumpJson(g_opInfoStatisticsDebug, OpInfoType::OP_INFO_FOR_DEBUG);
    g_opInfoStatisticsDebug.clear();
    return ret;
}

/*
 * @brief: Extract Op Info from TilingContext and complete serialization.
 * @param [in] ctx: TilingContext
 * @return int32_t: 0 == SUCCESS, others is FAILED
 */
int32_t OpInfoSerialize(gert::TilingContext* ctx, const aclnnOpInfoRecord::OpCompilerOption& opt,
                        const aclnnOpInfoRecord::OpKernelInfo* kernelInfo,
                        const aclnnOpInfoRecord::RuntimeInfo* runtimeInfo)
{
    OP_CHECK(ctx != nullptr, OP_LOGE(ACLNN_ERR_INNER, "OpInfoSerialize ctx is nullptr!"), return -1);
    OP_CHECK(kernelInfo != nullptr, OP_LOGE(ACLNN_ERR_INNER, "OpInfoSerialize kernelInfo is nullptr!"), return -1);
    IniParse iniInstance;
    std::map<std::string, std::string> iniConfigMap;
    if (iniInstance.GetIniParams(iniConfigMap) != 0) {
        OP_LOGE(ACLNN_ERR_INNER, "Get ini config map error.");
        return -1;
    }
    try {
        nlohmann::json builtInJsonConfig;
        const auto pos = kernelInfo->bin_info.rfind(FILE_TYPE_SPLIT_CHAR);
        if (pos == std::string::npos) {
            OP_LOGE(ACLNN_ERR_INNER, "Bin info is not a complete file path.");
            return -1;
        }
        std::string builtInJsonPath = kernelInfo->bin_info.substr(0, pos) + ".json";
        if (ReadJsonInfo(builtInJsonPath, builtInJsonConfig) != 0) {
            OP_LOGE(ACLNN_ERR_INNER, "Read json Info: %s failed.", builtInJsonPath.c_str());
            return -1;
        }
        if (!builtInJsonConfig.contains("supportInfo")) {
            OP_LOGE(ACLNN_ERR_INNER, "Json %s does not contain supportInfo keyword.", builtInJsonPath.c_str());
            return -1;
        }
        bool needTilingResult = runtimeInfo != nullptr;
        nlohmann::json jsonDebug = TilingContextToJson(ctx, iniConfigMap, builtInJsonConfig["supportInfo"],
                                                       kernelInfo->isMc2, needTilingResult);
        if (jsonDebug.is_null()) {
            return 0;
        }
        AddPlatformInfoToJson(jsonDebug, ctx, opt);
        AddOpInfoToJson(jsonDebug, opt, kernelInfo, runtimeInfo);
        std::lock_guard<std::mutex> lck(g_opInfoStatisticsLck);
        DumpJsonToOpInfoCompile(jsonDebug);
        jsonDebug["bin_info"] = kernelInfo->bin_info;
        DumpJsonToOpInfo(jsonDebug);
        DumpJsonToOpInfoDebug(jsonDebug);
    } catch (nlohmann::json::exception& e) {
        OP_LOGE(ACLNN_ERR_INNER, "OpInfoSerialize json exception:%s!", e.what());
        return -1;
    }
    return 0;
}
} // namespace aclnnOpInfoRecord
