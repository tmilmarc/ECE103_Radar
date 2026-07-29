#pragma once

class DataSource
{
public:

    virtual ~DataSource() {}

    virtual bool Open() = 0;

    virtual bool Read(float& angle, float& distance) = 0;
};