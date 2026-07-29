#include "FileReader.h"
#include <sstream>


FileReader::FileReader(const std::string& filename)
{
    this->filename = filename;
}


bool FileReader::Open()
{
    file.open(filename);

    return file.is_open();
}


bool FileReader::Read(float& angle, float& distance)
{
    if(!file.is_open())
    {
        return false;
    }


    std::string line;

    if(!std::getline(file, line))
    {
        file.clear();
        file.seekg(0);
    
        std::getline(file, line);
    }


    std::stringstream ss(line);

    char comma;


    ss >> angle;
    ss >> comma;
    ss >> distance;


    return true;
}

void FileReader::Close()
{
    if(file.is_open())
    {
        file.close();
    }
}