#pragma once
#include<stack>
#include<functional>
#include "Tile.h"

class WindowManager;
class ViewMovementComponent;

class Tilemap
{
public:
	//default contructor for initilizing the window pointer
	Tilemap(WindowManager& window);
	//destructor where we save the data to the save file
	~Tilemap();
	//updates the needed events such as mouse wheel scrolled and more
	void updateEvents(const sf::Event& evnt);
	//updates the tilemap 
	void update(float dt);
	//renders each tile individually, draws the grids and more.
	void render();
	//sets size of the view size 
	void setViewSize(sf::Vector2f size);
	//this effects the tilemap how it is saved to a file. This is the resolution of the image 
	void setTilemapSize(sf::Vector2f size);
	//sets the texture to draw the background of the tilemap;
	void setBackgroundTexture(sf::Texture& texture);
	//resizes the tilemap width and height 
	void resize(sf::Vector2i newSize);
	//adds a layer on top of the last one
	void addLayer();
	//removes the top layer. If there is only 1 layer this function does nothing
	void removeLayer();
	//clear the data of rach tile 
	void clear();
	//contructs a Tile  with the given arguments and set the tile at the given index
	void setTile(sf::Vector3i index, int tileType, const std::string& textureName, sf::IntRect textureRect, Tile::CollisionType collision = Tile::CollisionType::None);
	//set the Tile at the given index with the given Tile
	void setTile(sf::Vector3i index, const Tile& tile);
	//contruct a tile from the given arguments and set it as the current draw Tile
	void setCurrentDrawTile(int tileType, const std::string& textureName, sf::IntRect textureRect, Tile::CollisionType collision = Tile::CollisionType::None);
	//set the Tile for drawing given the Tile in the argument
	void setCurrentDrawTile(const Tile& tile);
	//returns the tilemap size it also effects the resolution of the map image
	sf::Vector2f getTilemapSize();
	//sets the speed of the camera moved with the mouse
	void setCameraSpeed(float newSpeed);
	//returns the camera speed
	float getCameraSpeed();
	//sets the tile draw mode true to draw only once when the draw key is pressed  
	void setSingleTileDrawMode(bool drawMode);
	//returns the tile draw mode
	bool getSingleTileDrawMode();
	//sets the bool that determines whether the grid should be drawn or not
	void setDrawGrid(bool newDrawGrid);
	//returns the bool drawGrid
	bool getDrawGrid();
	//sets the size of the brush in tilemap size
	void setBrushSize(int brushSize);
	//returns the size of the brush in in tilemap size
	int getBrushSize();
	//setting this to true means when you draw you will draw regardless what tile on that position is
	//setting this to true will only draw a tile when there is no tile at that position means you cant replace you have to delete it. This helps with drawing
	void setOverrideTile(bool newOverrideMode);
	//returns whehter you override a tile when drawing
	bool getOverrideTile();
	//saves the data of whole map to folder with which you can load a map 
	void saveTilemapDataToFolder(const std::string& filePath);
	//loads the map saved to the given folder path
	void loadTilemapDataFromFolder(const std::string& filePath);
	//sets the width and height of the map(how many tiles Width x Height the map is)
	void setMapSize(sf::Vector2i newMapSize);
	//returns the map size 
	sf::Vector2i getMapSize();
	//set a new amount of layers 
	void setMapDepth(size_t newDepth);
	//return the amount of layers the map has
	size_t getMapDepth() const;
	//sets the value of what tile layers to draw -1 = all tiles, 0 only the 0 layer, 1 only the 1 layer ...
	void setDepthDrawValue(short depthDrawValue);
	//returns the bounds of the tilemap relative to the view
	sf::FloatRect getTilemapBounds();
	//returns the size scale factor(how much the tilemap was resized)
	sf::Vector2f getTilemapSizeScale();
private:
	//a nested class used to store the information needed to undo tile drawing
	class UndoManager
	{
	public:
		//adds to the top of the undo data stack a copy of the map index and a copy of the tile object
		void addUndoData(sf::Vector3i mapIndex, Tile tile);
		// removes the top undo data after iterating on it with the provided lambda function
		void undo(std::function<void(sf::Vector3i mapIndex, const Tile& tile)> func);
		//add a new element of the undo data to the top of the undo stack.
		//this means you will have a new undo frame so when you call undo it will pop the data added after this call 
		void addNewUndoFrame();
	private:
		//a hash function to use hash map with sf::Vector3i
		struct Vector3iHash
		{
			size_t operator()(const sf::Vector3i& vector) const;

		private:
			//helper function to combine multiple hash values
			void hash_combine(size_t& seed, const size_t value) const;
		};
	private:
		//every undo data frame is stored using a hash map of indexes and its corresponding tile
		using undoDataType = std::unordered_map<sf::Vector3i, Tile, Vector3iHash>;
		//using a stack top efficiently store and use only the top element of all drawing actions the user has done.
		//it stores copies of the map index and the tile object
		std::stack<undoDataType> m_undoTileData;
	};

private:
	//using a 3D map to have width, height and depth
	using mapType = std::vector<std::vector<std::vector<Tile>>>;
	//the actual map storing the tiles
	mapType m_map;
	//the current tile the user is drawing with the mouse
	Tile m_currentMouseTile;
	//the index of current mouse layer to draw the tile
	int m_currentMouseLayer;
	//it shows the position of the tilemap
	//sf::Vector2f m_tilemapPosition;
	//the size of the view
	sf::Vector2f m_viewSize;
	//used to store the first size of the view for switching from fullscreen back to it
	sf::Vector2f m_firstViewSize;
	//used to store the first position of the tilemap for switching from fullscreen back to it 
	//sf::Vector2f m_firstPosition;
	//the size of each grid representing a tile
	sf::Vector2f m_gridSize;
	//storing the previous mouse index for the line algorithm between 2 points
	sf::Vector2i m_previousMosueGridIndex;
	//the view of the map the we can zoom in and out
	sf::View m_view;
	//the coponent to move the view
	std::unique_ptr<ViewMovementComponent> m_viewMovement;
	//shows if the fullscreen mode is enabled unsing the 'f' key
	bool m_isInFullscreen;
	//determines whether the user is drawing tiles using the Left mouse button
	bool m_isPlacingTile;
	//determines whether the user is deleting tiles using the space key
	bool m_isDeletingTile;
	//the texture to render the background of the tilemap
	sf::Texture* m_backgroundTexture;
	//using a stack to store the tile data to undo with Ctrl + Z keys. Only needing the top so stack is more efficient
	UndoManager m_undoManager;
	//determines which layers to draw in the render tiles functions -1 = all layers, 0 layer with index 0 . . .  
	short m_depthDrawValue;
	//determines how many tiles should be drawn or deleted on the map
	int m_brushSizeInPixels;
	//stores the points of the outline of the shape so that we can draw the outline of the brush
	sf::ConvexShape m_brushOutlineShape;
	//the window to draw to and update the mouse position
	WindowManager* m_window;
	//making the EditorUI a friend so it can access and modify the tilemap
	friend class EditorUI;
	//same as with the EditorUI
	friend class SettingsState;
	//making the custom point editor class a friend so that it can access 
	friend class CustomPointEditor;
private:
	//returns the width of the tilemap array
	size_t getWidth() const;
	//returns the height of the tilemap array
	size_t getHeight() const;
	//recalculates the outline points for the convex shape
	void resetBrushOutlines();
	//returns whether the given index is a valid index in the map array
	bool isValidMapIndex(sf::Vector2i index) const;
	//renders the tilemap the arguement is used to specify where to draw to. 
	//later when we save the tilemap we want to draw the map to another window. 
	//it used the window and view from the given ViewMovementComponent
	void renderTilemap(ViewMovementComponent& viewMovement);
	//draw all tiles from the map array
	void renderTiles(ViewMovementComponent& viewMovement);
	//draw the mouse tile 
	void renderMouseTile();
	//renders the background
	void renderTilemapBackground(ViewMovementComponent& viewMovement);
	//renders the black background behind the tilemap
	void renderBlackBackground(ViewMovementComponent& viewMovement);
	//returns the current mouse grid position(array index of the map)
	sf::Vector2i getCurrentMouseGridPosition() const;
	//paints tiles using the current mouse tile at the given index with the brush size at the current layer of the mouse 
	void paintAtIndex(sf::Vector2i mouseIndex);
	//return whether the value is positive = 1, null = 0, negative = -1
	int sign(int value);
	//executes the function and passes every point between point1 and point2 into the function
	void forEachPointOnLine(sf::Vector2i point1, sf::Vector2i point2, std::function<void(sf::Vector2i point)> function);
	//updates input for the mouse
	void updateMouseInput(float dt);
	//updates the keyboard inputs
	void updateKeyboardInput();
	//updates the input for the left mouse button such as tile placing
	void updateLeftMousebuttonInput();
	//resetting the viewport of the map taking in account the m_size and m_position
	void resetViewport();
	//toggles the bool m_isFullscreen true and false after each call and sets the sizes respectively
	void toggleFullscreen();
	//calculates the gridSize stored in 'm_gridSize'
	void calculateGridSize();
	//saves the tilemap to the folder path specified path as a .png file
	void saveTilemapPNG(const std::string& filePath);
	//clears the tile where is mouse is currently located on
	void deleteMouseTiles();
	//draws the grids for the tiles
	void renderGrids();
	//restores all the data the user was drawing recently
	void undoTileDrawing();
	//loads all the saved tilemap data from the given txt filepath 
	void loadMapDataFromSave(const std::string& filePath);
	//saves all the tilemap data to the given txt file path.
	void saveMapDataToSave(const std::string& filePath);
	//returns true if at least 1 tile is empty at map index [x][y][from 0 - mapIndex.z] 
	bool tilesUnderAreEmpty(const sf::Vector3i& mapIndex);
	//returns true if at least 1 tile is empty at map index [x][y][from mapIndex.z - mapDeapth] 
	bool tilesAboveAreEmpty(const sf::Vector3i& mapIndex);
	//determines whehter a tile is valid for drawing taking into account the override tile boolean  
	bool isValidTileDrawIndex(const sf::Vector3i& mapIndex);
	//iterates over the all tiles and executes the lambda function in the brush bounds and only the valid ones
	void forEachPointInBrush(const sf::Vector3i& centerMapIndex, std::function<void(const sf::Vector3i& mapIndex)> function);
	//renders a preview of the tiles that will be drawn when you press the draw key
	void renderPreviewMouseTiles();
	//renders the outline of the brush shape
	void renderBrushOutlines();
};