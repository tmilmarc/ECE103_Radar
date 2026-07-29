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
    char buffer[64];

    DWORD bytesRead;

    if(!ReadFile(
        serialHandle,
        buffer,
        sizeof(buffer)-1,
        &bytesRead,
        nullptr))
    {
        return false;
    }

    if(bytesRead == 0)
    {
        return false;
    }


    buffer[bytesRead] = '\0';


    std::stringstream ss(buffer);

    char comma;


    ss >> angle;
    ss >> comma;
    ss >> distance;


    return true;
}

void SerialPort::Close()
{
    if(serialHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(serialHandle);
        serialHandle = INVALID_HANDLE_VALUE;
    }
}