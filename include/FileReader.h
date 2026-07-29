#pragma once

#include "DataSource.h"
#include <fstream>
#include <string>


class FileReader : public DataSource
{
public:

    FileReader();
    bool Open(const std::string& filename) override;


    bool Read(float& angle, float& distance) override;
    void Close() override;


private:


    std::ifstream file;
};