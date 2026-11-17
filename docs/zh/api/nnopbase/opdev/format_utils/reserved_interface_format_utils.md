# 预留接口

本文档列出format相关的预留接口及其功能说明，此类接口后续有可能变更或废弃，不建议开发者使用，开发者无需关注。

**表 1** 接口列表

| 接口名称 | 功能说明 |
| --- | --- |
| ToFormat(const std::string &formatStr) | 将字符串转换为Format。 |
| ToString(Format format) | 将Format转换为字符串。 |
| GetSubFormat(int32_t format) | 获取给定format的sub format。 |
| GetSubFormat(Format format) | 获取给定format的sub format。 |
| HasSubFormat(int32_t format) | 获取给定format的sub format。 |
| GetC0Format(int32_t format) | 获取给定format的C0 format。 |
| HasC0Format(int32_t format) | 判断给定format是否包含C0 format。 |
| GetFormatFromSub(int32_t primaryFormat, int32_t subFormat) | 用给定的primary format和sub format生成format。 |
