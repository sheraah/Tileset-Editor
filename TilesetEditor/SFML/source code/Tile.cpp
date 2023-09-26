#include "stdafx.h"
#include "Tile.h"
#include "ResizableSprite.h"
#include "TextureManager.h"
#include "LayeredData.h"

void Tile::render(sf::Vector2f position, sf::Vector2f size, sf::RenderTarget& target)
{
	if (m_tileType == -1) return;

	ResizableSprite sprite;

	if (!TextureManager::textureExists(m_textureName))
	{
		reset();
		return;
	}

	sprite.setTextureRect(m_textureRect);
	sprite.setTexture(TextureManager::getTexture(m_textureName));
	sprite.setPosition(position);
	sprite.setSize(size);

	target.draw(sprite);
}

Tile::Tile()
{
	reset();
}

Tile::Tile(short tileType, const std::string& textureName, sf::IntRect textureRect, CollisionType collision)
	:m_tileType(tileType), m_textureName(textureName), m_textureRect(textureRect), m_collision(collision)
{

}

void Tile::reset()
{
	m_tileType = -1;
	m_textureName = std::string();
	m_textureRect = sf::IntRect(0, 0, 0, 0);
	m_collision = CollisionType::None;
}

bool Tile::isEmpty() const
{
	return m_tileType == -1;
}

std::ostream& operator<<(std::ostream& os, const Tile& tile)
{
	os << "TileType " << tile.m_tileType << '\n';
	os << "Collision " << static_cast<int>(tile.m_collision) << '\n';
	os << "TextureName " << tile.m_textureName << '\n';
	os << "TextureRect " << tile.m_textureRect << '\n';

	return os;
}

std::istream& operator>>(std::istream& in, Tile& tile)
{
	LayeredData data;
	
	in >> data;
	in >> data;
	in >> data;
	in >> data;

	tile.m_tileType = data.get<short>("TileType");
	tile.m_collision = static_cast<Tile::CollisionType>(data.get<int>("Collision"));
	tile.m_textureName = data.get<std::string>("TextureName");
	tile.m_textureRect = data.get<sf::IntRect>("TextureRect");

	return in;
}
