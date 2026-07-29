#include "FileDialog.h"
#include <windows.h>

std::string FileDialog::OpenFile()
{
    OPENFILENAMEA file;
    char filename[MAX_PATH] = "";

    ZeroMemory(&file, sizeof(file));

    file.lStructSize = sizeof(file);
    file.lpstrFile = filename;
    file.nMaxFile = MAX_PATH;
    file.Flags = OFN_FILEMUSTEXIST;

    if(GetOpenFileNameA(&file))
    {
        return filename;
    }

    return "";
}