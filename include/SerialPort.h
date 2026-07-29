//Header for SerialPort.cpp
#pragma once
#include <string>
#include <windows.h>
#include <sstream>

class SerialPort
{
public:
    SerialPort();
    ~SerialPort();

    bool Open(const std::string& portName);
    bool Read(float& angle, float& distance);
    void Close();

private:
    HANDLE serialHandle;
    std::string buffer;
};