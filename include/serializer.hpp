#ifndef PORYTILES_GUI_SERIALIZER_HPP
#define PORYTILES_GUI_SERIALIZER_HPP

#include <filesystem>
#include <string>
#include <nlohmann/json.hpp>
#include <vector>

namespace Serializer
{
    typedef void (*SerializeValueCallback)();

    extern nlohmann::json g_config;
    extern std::vector<SerializeValueCallback> g_serializeCallbacks;

    void init(char** argv);
    void shutdown();

    template<typename T>
    void registerValue(const std::string& id, T& value)
    {
        if (g_config.contains(id))
            value = g_config[id];

        g_serializeCallbacks.push_back([id, value]() {
            g_config[id] = value;
        });
    }

    template<typename T>
    T load(std::string id, T defaultValue)
    {
        if (g_config.contains(id))
            return g_config[id];

        return defaultValue;
    }

    template<typename T>
    void store(std::string id, T value)
    {
        g_config[id] = value;
    }

    // Specializations for serializing more complicated types

    // Filesystem paths
    template<> std::filesystem::path load<std::filesystem::path>(std::string id, std::filesystem::path defaultValue);
    template<> void store<std::filesystem::path>(std::string id, std::filesystem::path value);
}

#endif // PORYTILES_GUI_SERIALIZER_HPP

