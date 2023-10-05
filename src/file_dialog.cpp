#include <nfd.h>
#include "file_dialog.hpp"

using namespace std;

namespace FileDialog
{
    bool tryPickFile(filesystem::path& outFilePath, const char* filter)
    {
        nfdchar_t *outPath = nullptr;
        nfdresult_t result = NFD_OpenDialog(filter, nullptr, &outPath);

        if (result == NFD_OKAY)
        {
            outFilePath = filesystem::path {outPath};
            delete outPath;
            return true;
        }

        return false;
    }

    bool tryPickFolder(filesystem::path& outFolderPath)
    {
        nfdchar_t *outPath = nullptr;
        nfdresult_t result = NFD_PickFolder(nullptr, &outPath);

        if (result == NFD_OKAY)
        {
            outFolderPath = filesystem::path {outPath};
            delete outPath;
            return true;
        }

        return false;
    }
}