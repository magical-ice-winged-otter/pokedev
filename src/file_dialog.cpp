#include <nfd.h>
#include "file_dialog.hpp"

using namespace std;

namespace FileDialog
{
    bool tryPickFile(filesystem::path& outFilePath, const char* defaultPath, const char* filter)
    {
        nfdchar_t *outPath = nullptr;
        nfdresult_t result = NFD_OpenDialog(filter, defaultPath, &outPath);

        if (result == NFD_OKAY)
        {
            outFilePath = filesystem::path {outPath};
            delete outPath;
            return true;
        }

        return false;
    }

    bool tryPickFolder(filesystem::path& outFolderPath, const char* defaultPath)
    {
        nfdchar_t *outPath = nullptr;
        nfdresult_t result = NFD_PickFolder(defaultPath, &outPath);

        if (result == NFD_OKAY)
        {
            outFolderPath = filesystem::path {outPath};
            delete outPath;
            return true;
        }

        return false;
    }
}