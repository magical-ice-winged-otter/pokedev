#ifndef POKETOOLS_SERIALIZER_HPP
#define POKETOOLS_SERIALIZER_HPP

#include <filesystem>
#include <string>
#include <cereal/cereal.hpp>

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