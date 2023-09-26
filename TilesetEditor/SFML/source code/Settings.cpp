#include "stdafx.h"
#include "Settings.h"

void Settings::save(const std::string& filePath)
{
	std::ofstream out(filePath);

	getInstance().saveToFile<sf::Vector2u, sf::Vector2f, float, bool, bool, bool, bool, sf::Vector2i, int>(out);

	out.close();
}

void Settings::load(const std::string& filePath)
{
	std::ifstream in(filePath);

	getInstance().loadFromFile<sf::Vector2u, sf::Vector2f, float, bool, bool, bool ,bool, sf::Vector2i, int>(in);

	in.close();
}

Settings& Settings::getInstance()
{
	static Settings instance; //static instance(singleton)
	return instance;
}
