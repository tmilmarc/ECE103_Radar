//Header for SerialPort.cpp
#pragma once

class SerialPort
{
public:
    SerialPort();
    ~SerialPort();

    bool Connect();
    void Disconnect();

    bool ReadData();
};