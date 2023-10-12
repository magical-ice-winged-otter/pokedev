#include "serializer.hpp"

Serializer::ConfigFile::ConfigFile(std::string_view filePath)
        : m_configPath {std::filesystem::path{filePath}}
{
}
