#include "serializer.hpp"

ConfigFile::ConfigFile(std::string_view filePath)
        : m_configPath {std::filesystem::path{filePath}}
{
}
