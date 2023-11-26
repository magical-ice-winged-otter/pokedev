#include <fstream>
#include <regex>
#include "mon_data.hpp"

static std::vector<std::string> parseDefines(const std::filesystem::path& filePath, const char* regex)
{
    std::ifstream stream {filePath};
    std::string line {};
    std::vector<std::string> results {};
    std::smatch match {};

    while (std::getline(stream, line))
    {
        bool hasDefine = line.find("#define") != std::string::npos;

        if (hasDefine && std::regex_search(line, match, std::regex(regex)))
        {
            size_t start = line.find("#define") + 8;
            size_t end = line.find(' ', start);
            results.emplace_back(line.substr(start, end - start));
        }
    }

    return results;
}

void MonAbilityLoader::init(const std::filesystem::path& abilityFilePath)
{
    abilityNames = parseDefines(abilityFilePath, ".*#define ABILITY_");
}
