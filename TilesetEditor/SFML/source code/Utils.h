#pragma once
#include <sstream>

//some helper functions 
class Utils
{
public:
    //converts a string to a value of the type T
    template <typename T>
    static T StrToValue(const std::string& str);
    //sets up the text object with the specified parameters. alignment is the origin scale { 0.5, 0.5 } is the center { 1, 1 } bottom right
    static void setupText(sf::Text& text, const std::string& str, uint32_t characterSize, sf::Vector2f position,
        sf::Color textColor = sf::Color::White, sf::Vector2f alignment = { 0.f, 0.f });
};

template<typename T>
inline T Utils::StrToValue(const std::string& str)
{
    T value;
    std::istringstream ss(str);
    ss >> value;
    return value;
}
