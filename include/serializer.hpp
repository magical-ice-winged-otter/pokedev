#ifndef PORYTILES_GUI_SERIALIZER_HPP
#define PORYTILES_GUI_SERIALIZER_HPP

#include <filesystem>
#include <string>

namespace Serializer
{
    void init(char** argv);
    void shutdown();

    std::filesystem::path readPath(std::string id, std::filesystem::path defaultValue = {});
    void writePath(std::string id, std::filesystem::path path);
}

#endif // PORYTILES_GUI_SERIALIZER_HPP

