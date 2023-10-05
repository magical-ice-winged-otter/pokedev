#include <cstdio>
#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <filesystem>
#include "serializer.hpp"

using namespace std;

namespace Serializer
{
    static filesystem::path s_configPath;
    nlohmann::json g_config {};

    void init(char** argv)
    {
        s_configPath = filesystem::path{argv[0]}.remove_filename() / "porytiles_gui_config.json";

        if (filesystem::exists(s_configPath))
        {
            printf("loading serializer data from disk\n");

            fstream fileStream{};
            fileStream.open(s_configPath);
            fileStream >> g_config;
            fileStream.close();
        }
    }

    void shutdown()
    {
        printf("writing serializer data to disk\n");

        ofstream fileStream {};
        fileStream.open(s_configPath);
        fileStream << std::setw(4) << g_config;
        fileStream.close();
    }

    template<>
    std::filesystem::path load<std::filesystem::path>(std::string id, std::filesystem::path defaultValue)
    {
        return std::filesystem::path {load<std::string>(id, defaultValue.string())};
    }

    template<>
    void store<std::filesystem::path>(std::string id, std::filesystem::path value)
    {
        return store<std::string>(id, value.string());
    }
}