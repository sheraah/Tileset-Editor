#pragma once
#include<unordered_map>
#include<sstream>

//this class allows you to add some key string and a value so you have a map of keys and values that you can retreive 
class LayeredData
{

public:
    //add your string line something like position 100 100 
    //later on you can retreive it by calling get<sf::Vectorf>("position")
    void add(const std::string& line);
    //returns the value stored under the key 
    template <typename T>
    T get(const std::string& key) const;
    //reads in a whole line from the input stream
    friend std::istream& operator>>(std::istream& in, LayeredData& data);
private:
    //used to store a key and the rest of the line as a value
    std::unordered_map<std::string, std::string> m_dataMap;
};

template<typename T>
inline T LayeredData::get(const std::string& key) const
{
    if (m_dataMap.find(key) == m_dataMap.end())
    {
        std::cerr << "No key found: " << key << " in LayeredData\n";
    }
    //reading in the string as a T value
    std::stringstream ss(m_dataMap.at(key));
    T value;
    ss >> value;
    return value;
}
