#pragma once

#include "DataSource.h"
#include <fstream>
#include <string>


class FileReader : public DataSource
{
public:

    FileReader(const std::string& filename);

    bool Open() override;

    bool Read(float& angle, float& distance) override;


private:

    std::string filename;

    std::ifstream file;
};