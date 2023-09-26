#pragma once
#include "ResourceManager.h"

//this class allows you to load fonts to store them with a name to use them
class FontManager
{
public:
	//load a font
	static void loadFont(const std::string& fontName, const std::string& fontPath);
	//gets the font stored under the fontName as a key value
	static sf::Font& getFont(const std::string& fontName);

private:
	//stores all fonts with a name as a key value
	ResourceManager<sf::Font> m_fonts;
	//getting the static instance(Singleton)
	static FontManager& getInstance();
};