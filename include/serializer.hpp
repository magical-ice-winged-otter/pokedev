#ifndef POKETOOLS_SERIALIZER_HPP
#define POKETOOLS_SERIALIZER_HPP

#include <filesystem>
#include <string>
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>

namespace Serializer
{
    template<typename T>
    cereal::NameValuePair<T&> nvp(const char* name, T&& value)
    {
        return cereal::make_nvp(name, value);
    }

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

} // namespace Serializer

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

#endif