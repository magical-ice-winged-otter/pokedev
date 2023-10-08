#ifndef PORYTILES_GUI_FILE_DIALOG_HPP
#define PORYTILES_GUI_FILE_DIALOG_HPP

#include <filesystem>

namespace FileDialog
{
    bool tryPickFile(std::filesystem::path& outFilePath, const char* defaultPath = nullptr, const char* filter = nullptr);
    bool tryPickFolder(std::filesystem::path& outFolderPath, const char* defaultPath = nullptr);
}

#endif // PORYTILES_GUI_FILE_DIALOG_HPP