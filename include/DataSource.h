#pragma once
#include <string>

class DataSource
{
public:

    virtual ~DataSource() {}

    virtual bool Open(const std::string& source) = 0;

    virtual bool Read(float& angle, float& distance) = 0;

    virtual void Close() = 0;

};