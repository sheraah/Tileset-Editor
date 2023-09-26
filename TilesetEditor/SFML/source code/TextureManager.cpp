#include "stdafx.h"
#include "TextureManager.h"
#include <fstream>

void TextureManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
	bool wasLoaded = getInstance().m_textures.load(textureName, [&texturePath](TextureData& data)
	{	
		data.m_filePath = texturePath;
		data.m_texture.loadFromFile(texturePath); 
	});

	if (!wasLoaded) //if there is already a texture with this name(couldnt load)
	{
		getInstance().m_textures[textureName].m_filePath = texturePath;
		getTexture(textureName).loadFromFile(texturePath);
	}
}

void TextureManager::load(const std::string& saveFilePath)
{
	std::ifstream in(saveFilePath);

	std::string textureName;
	std::string textureFilePath;

	//using >> and then getline because the name cannot contain any spaces but the filepath can contain spaces
	while (in >> textureName && std::getline(in, textureFilePath))
	{
		loadTexture(textureName, textureFilePath.substr(1)); //ignoring the first space after using >> thats why substr(1)
	}

	in.close();
}

void TextureManager::save(const std::string& saveFilePath)
{
	std::ofstream out(saveFilePath);

	getInstance().m_textures.iterResources([&out](const std::string& name, TextureData& data)
	{
		out << name << " " << data.m_filePath << std::endl;
	});

	out.close();
}

sf::Texture& TextureManager::getTexture(const std::string& textureName)
{
	return getInstance().m_textures[textureName].m_texture;
}

const std::string& TextureManager::getTexturePath(const std::string& textureName)
{
	return getInstance().m_textures[textureName].m_filePath;
}

TextureManager& TextureManager::getInstance()
{
	static TextureManager instance;

	return instance;
}

bool TextureManager::textureExists(const std::string& textureName)
{
	return getInstance().m_textures.keyExists(textureName);
}

void TextureManager::erase(const std::string& textureName)
{
	getInstance().m_textures.erase(textureName);
}
