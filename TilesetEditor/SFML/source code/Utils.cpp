#include "stdafx.h"
#include "Utils.h"

void Utils::setupText(sf::Text& text, const std::string& str, uint32_t characterSize, sf::Vector2f position, sf::Color textColor, sf::Vector2f alignment)
{
	text.setString(str);
	text.setCharacterSize(characterSize);
	text.setPosition(position);
	text.setFillColor(textColor);
	text.setOrigin(sf::Vector2f(text.getLocalBounds().width, text.getLocalBounds().height) * alignment);
}
