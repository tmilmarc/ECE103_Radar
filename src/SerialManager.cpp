#include "SerialManager.h"
#include <windows.h>


std::vector<std::string> SerialManager::GetAvailablePorts()
{
    std::vector<std::string> ports;


    for(int i = 1; i <= 255; i++)
    {
        std::string portName = "COM" + std::to_string(i);


        HANDLE handle = CreateFileA(
            portName.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            0,
            nullptr
        );


        if(handle != INVALID_HANDLE_VALUE)
        {
            ports.push_back(portName);

            CloseHandle(handle);
        }
    }


    return ports;
}