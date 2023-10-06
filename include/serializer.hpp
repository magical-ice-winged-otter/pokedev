#ifndef PORYTILES_GUI_SERIALIZER_HPP
#define PORYTILES_GUI_SERIALIZER_HPP

#include <filesystem>
#include <string>
#include <nlohmann/json.hpp>
#include <vector>

// todo: allow saving + loading from specific files, not just at startup

namespace Serializer
{
    extern nlohmann::json g_config;
    extern std::vector<std::function<void()>> g_serializeCallbacks;

    void init(char** argv);
    void shutdown();

    template<typename T>
    void registerValue(const std::string& id, T& value)
    {
        if (g_config.contains(id))
            value = g_config[id];

        g_serializeCallbacks.emplace_back([id, &value]() {
            g_config[id] = value;
        });
    }

    // Specializations for serializing more complicated types

    template<> void registerValue(const std::string& id, std::filesystem::path& value);
}

#endif // PORYTILES_GUI_SERIALIZER_HPP

