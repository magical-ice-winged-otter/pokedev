#ifndef PORYTILES_GUI_SERIALIZER_HPP
#define PORYTILES_GUI_SERIALIZER_HPP

#include <filesystem>
#include <string>
#include <nlohmann/json.hpp>

namespace Serializer
{
    extern nlohmann::json g_config;

    void init(char** argv);
    void shutdown();

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

