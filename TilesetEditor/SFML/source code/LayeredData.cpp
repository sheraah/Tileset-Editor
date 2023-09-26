#include "stdafx.h"
#include "LayeredData.h"

void LayeredData::add(const std::string& line)
{
    std::stringstream ss(line);
    std::string key;

    ss >> key;

    m_dataMap[key] = line.substr(key.length() + 1); // get string after the key + the 1 space charcter
}

std::istream& operator>>(std::istream& in, LayeredData& data)
{
    std::string line;

    if (std::getline(in, line))
    {
        data.add(line);
    }

    return in;
}
