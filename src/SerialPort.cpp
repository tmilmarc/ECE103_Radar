//Connects to ESP32, reads serial data, updates the radar object
#include "SerialPort.h"
#include <iostream>
#include <sstream>

SerialPort::SerialPort()
{
    serialHandle = INVALID_HANDLE_VALUE;
}

SerialPort::~SerialPort()
{
    Close();
}

bool SerialPort::Open(const std::string& portName)
{
    serialHandle = CreateFileA(
        portName.c_str(),
        GENERIC_READ,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );

    if(serialHandle == INVALID_HANDLE_VALUE)
    {
        std::cerr << "CreateFile failed. Error code: "
              << GetLastError()
              << "\n";

        return false;
    }

    DCB dcb = {0};
    dcb.DCBlength = sizeof(DCB);

    if(!GetCommState(serialHandle, &dcb))
    {
        Close();
        return false;
    }

    dcb.BaudRate = CBR_115200;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    if(!SetCommState(serialHandle, &dcb))
    {
        Close();
        return false;
    }

    COMMTIMEOUTS timeouts = {0};

    timeouts.ReadIntervalTimeout = 20;
    timeouts.ReadTotalTimeoutConstant = 20;
    timeouts.ReadTotalTimeoutMultiplier = 1;


    if(!SetCommTimeouts(serialHandle, &timeouts))
    {
        Close();
        return false;
    }

    return true;
}

bool SerialPort::Read(float& angle, float& distance)
{
    if(serialHandle == INVALID_HANDLE_VALUE)
    {
        return false;
    }


    char data[128];
    DWORD bytesRead = 0;


    bool success = ReadFile(
        serialHandle,
        data,
        sizeof(data),
        &bytesRead,
        nullptr
    );


    if(!success || bytesRead == 0)
    {
        return false;
    }


    for(DWORD i = 0; i < bytesRead; i++)
    {
        char c = data[i];


        if(c == '\n')
        {
            std::stringstream ss(buffer);

            char comma;

            ss >> angle;
            ss >> comma;
            ss >> distance;


            buffer.clear();

            return true;
        }
        else
        {
            buffer += c;
        }
    }


    return false;
}

void SerialPort::Close()
{
    if(serialHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(serialHandle);
        serialHandle = INVALID_HANDLE_VALUE;
    }
}