#pragma once

#include <string>

class FileDialog
{
public:
    static std::string OpenFile();
    static std::string SaveFile(const std::string& defaultName = "radar_capture.txt");
};