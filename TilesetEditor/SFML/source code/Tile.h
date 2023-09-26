#pragma once

//a class that stores the needed data to render a Tile with the specified Tileset and Rect
class Tile
{
public:
	//the collision type for each tile
	enum class CollisionType { None = 0, Left = 1, Right = 2, Top = 4, Bottom = 8 };
	//renders the tile with the specified data.
	void render(sf::Vector2f position, sf::Vector2f size, sf::RenderTarget& target);
	//default contructor
	Tile();
	//conturctor to initialize all members
	Tile(short tileType, const std::string& textureName, sf::IntRect textureRect, CollisionType collision);
	//resets all memebers to defaul
	void reset();
	//returns true if the tiletype is -1
	bool isEmpty() const;
	//writing to a stream
	friend std::ostream& operator<<(std::ostream& os, const Tile& tile);
	//reading from a stream
	friend std::istream& operator>>(std::istream & os, Tile& tile);
private:
	//the tile type represented as numbers. -1 means there is no tile. 0 is the default tiletype
	short m_tileType;
	//the texture name of the tileset used to draw this tile
	std::string m_textureName;
	//the texutre rect on the texture to draw that tile 
	sf::IntRect m_textureRect;
	//an enum representing from which side the 
	CollisionType m_collision;
};