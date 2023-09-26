#include "stdafx.h"
#include "FontManager.h"

void FontManager::loadFont(const std::string& fontName, const std::string& fontPath)
{
	getInstance().m_fonts.load(fontName, [&fontPath](sf::Font& font)
	{
		font.loadFromFile(fontPath);
	});
}

sf::Font& FontManager::getFont(const std::string& fontName)
{
	return getInstance().m_fonts[fontName];
}

FontManager& FontManager::getInstance()
{
	static FontManager instance; // a static instance(singleton)
	return instance;
}
