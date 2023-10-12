#include <cstdio>
#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <filesystem>
#include "serializer.hpp"

using namespace std;

namespace Serializer
{
    nlohmann::json g_config {};
    std::vector<std::function<void()>> g_serializeCallbacks {};

    static filesystem::path s_configPath;

    void init(const std::filesystem::path& configPath)
    {
        s_configPath = configPath;

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

        for (const std::function<void()>& callback : g_serializeCallbacks)
            callback();

        ofstream fileStream {};
        fileStream.open(s_configPath);
        fileStream << std::setw(4) << g_config;
        fileStream.close();
    }

    template<> void registerValue(const string& id, filesystem::path& value)
    {
        if (g_config.contains(id))
            value = filesystem::path {static_cast<string>(g_config[id])};

        g_serializeCallbacks.emplace_back([id, &value]() {
            g_config[id] = value.string();
        });
    }
}