#include "SerialManager.h"
#include <windows.h>
#include <vector>

std::vector<std::string> SerialManager::GetAvailablePorts()
{
    std::vector<std::string> ports;

    HKEY key = nullptr;
    LONG status = RegOpenKeyExA(
        HKEY_LOCAL_MACHINE,
        "HARDWARE\\DEVICEMAP\\SERIALCOMM",
        0,
        KEY_READ,
        &key
    );

    if(status != ERROR_SUCCESS || key == nullptr)
    {
        return ports;
    }

    DWORD index = 0;
    char valueName[256] = {0};
    BYTE valueData[256] = {0};
    DWORD valueNameSize = sizeof(valueName);
    DWORD valueDataSize = sizeof(valueData);

    while(true)
    {
        valueNameSize = sizeof(valueName);
        valueDataSize = sizeof(valueData);

        LONG enumStatus = RegEnumValueA(
            key,
            index,
            valueName,
            &valueNameSize,
            nullptr,
            nullptr,
            valueData,
            &valueDataSize
        );

        if(enumStatus != ERROR_SUCCESS)
        {
            break;
        }

        std::string port(reinterpret_cast<const char*>(valueData), valueDataSize);
        if(port.rfind("COM", 0) == 0)
        {
            ports.push_back("\\\\.\\" + port);
        }

        ++index;
    }

    RegCloseKey(key);
    return ports;
}