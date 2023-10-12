#ifndef POKEDEV_SERIALIZER_HPP
#define POKEDEV_SERIALIZER_HPP

#include <filesystem>
#include <string>
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>

#define AUTO_NAME(var) CEREAL_NVP(var)
#define CUSTOM_NAME(name, var) cereal::make_nvp(name, var)

class ConfigFile
{
public:
    explicit ConfigFile(std::string_view filePath);

    template <class ... Types> inline
    void writeData(Types&& ... args)
    {
        std::ofstream configFile {m_configPath};
        cereal::JSONOutputArchive archive {configFile};
        archive(args...);
    }

    template <class ... Types> inline
    void readData(Types&& ... args)
    {
        if (std::filesystem::exists(m_configPath))
        {
            std::ifstream configFile {m_configPath};
            cereal::JSONInputArchive archive {configFile};
            archive(args...);
        }
    }

private:
    std::filesystem::path m_configPath {};
};

// Custom Serialization Definitions
namespace std::filesystem
{
    template<class Archive>
    inline std::string save_minimal(const Archive&, const std::filesystem::path& path)
    {
        return path.string();
    }

    template<class Archive>
    inline void load_minimal(const Archive&, std::filesystem::path& path, const std::string& value)
    {
        path = filesystem::path {value};
    }
}

#endif // POKEDEV_SERIALIZER_HPP