#pragma once
#include "ResourceManager.h"

class TextureManager
{
public:
	//loads the textures from that save file
	static void load(const std::string& saveFilePath);
	//saves all textures to that save file
	static void save(const std::string& saveFilePath);
	//gets the texture stored under the textureName as a key value
	static sf::Texture& getTexture(const std::string& textureName);
	//returns the textures filePath stored under that name
	static const std::string& getTexturePath(const std::string& textureName);
	//load a texture from a file path and stored using the name as a key value
	static void loadTexture(const std::string& textureName, const std::string& texturePath);
	//checks if a texture with the given name exists
	static bool textureExists(const std::string& textureName);
	//removes the texture with the given name
	static void erase(const std::string& textureName);
private:
	//a struct used to store a texture and the filePath where it was loaded from
	struct TextureData
	{
		//the texture source
		sf::Texture m_texture;
		//the filepath of the texture
		std::string m_filePath;
	};

private:
	//stores all textures with a name as a key value
	ResourceManager<TextureData> m_textures;
	//getting the static instance(Singleton)
	static TextureManager& getInstance();
};