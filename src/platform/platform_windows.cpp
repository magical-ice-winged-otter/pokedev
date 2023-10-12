#if _WIN32

#include <shobjidl.h>
#include <string_view>
#include <windows.h>
#include "platform.hpp"

void Platform::init()
{
    // Initialize COM for stuff like dialogue boxes.
    if (!SUCCEEDED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
    {
        printf("WINDOWS: Failed to initialize COM!");
        return;
    }
}

void Platform::shutdown()
{
    CoUninitialize();
}

// Convert ansi strings to the windows UTF16 ones.
static std::wstring utf8To16(std::string_view utf8)
{
    std::wstring result {};
    int size = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);
    result.resize(size);
    MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), result.data(), size);
    return result;
}

// Safely try and release COM objects.
template<typename T>
static void safeDelete(T** comObject)
{
    if (*comObject)
    {
        (*comObject)->Release();
        *comObject = nullptr;
    }
}

static bool tryGetPathFromDialog(IFileOpenDialog* fileDialog, std::filesystem::path& outPath, const Platform::FilePickerOptions& options)
{
    // Set options
    if (options.defaultPath.has_value())
    {
        IShellItem* defaultPathItem;
        std::wstring wideDefaultPath {utf8To16(options.defaultPath.value().string())};
        SHCreateItemFromParsingName(wideDefaultPath.c_str(), nullptr, IID_PPV_ARGS(&defaultPathItem));
        fileDialog->SetFolder(defaultPathItem);
        safeDelete(&defaultPathItem);
    }

    if (options.filter.has_value())
    {
        std::wstring wideFilter {utf8To16(options.filter.value())};
        fileDialog->SetDefaultExtension(wideFilter.c_str());
    }

    // Show the dialog
    HRESULT result = fileDialog->Show(nullptr);

    // Try and get the picked path
    if (SUCCEEDED(result))
    {
        IShellItem* pickedItem;
        fileDialog->GetResult(&pickedItem);
        PWSTR filePath;
        pickedItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
        outPath = std::filesystem::path {filePath};
        CoTaskMemFree(filePath);
        safeDelete(&pickedItem);
    }

    return SUCCEEDED(result);
}

bool Platform::tryPickFile(std::filesystem::path& outPath, const FilePickerOptions& options)
{
    IFileOpenDialog* fileOpenDialog;
    CoCreateInstance(__uuidof(FileOpenDialog), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&fileOpenDialog));

    // Get and return the path the user selected
    bool result = tryGetPathFromDialog(fileOpenDialog, outPath, options);
    safeDelete(&fileOpenDialog);
    return result;
}

bool Platform::tryPickFolder(std::filesystem::path& outPath, const FilePickerOptions& options)
{
    IFileOpenDialog* fileOpenDialog;
    CoCreateInstance(__uuidof(FileOpenDialog), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&fileOpenDialog));

    // Configure to only pick folders
    FILEOPENDIALOGOPTIONS dialogOptions;
    fileOpenDialog->GetOptions(&dialogOptions);
    fileOpenDialog->SetOptions(dialogOptions | FOS_PICKFOLDERS);

    // Get and return the path the user selected
    bool result = tryGetPathFromDialog(fileOpenDialog, outPath, options);
    safeDelete(&fileOpenDialog);
    return result;
}

void Platform::openPath(const std::filesystem::path& path)
{
    ShellExecute(nullptr, "explore", path.string().c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
}

#endif