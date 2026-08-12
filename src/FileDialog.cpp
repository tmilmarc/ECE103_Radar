#include "FileDialog.h"
#include <windows.h>

namespace
{
    std::string NormalizePath(const char* rawPath)
    {
        if(rawPath == nullptr)
        {
            return "";
        }

        return std::string(rawPath);
    }
}

std::string FileDialog::OpenFile()
{
    OPENFILENAMEA file;
    char filename[MAX_PATH] = "";

    ZeroMemory(&file, sizeof(file));

    char filter[] = "Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0";

    file.lStructSize = sizeof(file);
    file.lpstrFile = filename;
    file.nMaxFile = MAX_PATH;
    file.lpstrFilter = filter;
    file.nFilterIndex = 1;
    file.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if(GetOpenFileNameA(&file))
    {
        return NormalizePath(filename);
    }

    return "";
}

std::string FileDialog::SaveFile(const std::string& defaultName)
{
    OPENFILENAMEA file;
    char filename[MAX_PATH] = "";

    ZeroMemory(&file, sizeof(file));

    std::string defaultFileName = defaultName;
    if(defaultFileName.empty())
    {
        defaultFileName = "radar_capture.txt";
    }

    strncpy_s(filename, sizeof(filename), defaultFileName.c_str(), _TRUNCATE);

    char filter[] = "Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0";

    file.lStructSize = sizeof(file);
    file.lpstrFile = filename;
    file.nMaxFile = MAX_PATH;
    file.lpstrFilter = filter;
    file.nFilterIndex = 1;
    file.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

    if(GetSaveFileNameA(&file))
    {
        return NormalizePath(filename);
    }

    return "";
}