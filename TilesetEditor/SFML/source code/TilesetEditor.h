#pragma once
#include "Tile.h"
#include "ResourceManager.h"
#include "GUI.h"

class WindowManager;
class ViewMovementComponent;

//a class that allows you to edit the loaded Tileset image, add collision and more. 
//this all happens in a different window where you can edit all these things.
class TilesetEditor
{
public:
	//default contructor
	TilesetEditor(WindowManager& window);
	//sets the texture of the tileset
	void setTilesetTexture(const std::string& tilesetTextureName);
	//sets the size of a tile(in pixles) on the tileset texture
	void setTileSize(sf::Vector2i tileSizeInPixels);
	//sets amount of tiles that are inside the tileset texture width x height
	//note that this function does nothing if the texture wasnt set
	void setTileDimension(sf::Vector2i tileDimension);
	//sets the position of the Tileset from where the user can select tiles with the mouse
	void setPosition(sf::Vector2f position);
	//sets the size of the Tileset from where the user can select 
	//note that this function does nothing if the texture wasnt set
	void setTilesetSize(sf::Vector2f tilesetSize);
	//returns a contructed Tile with the properties of the current selected Tile on the Tileset
	//if there is no Tile selected it will return a default contructed Tile
	Tile getActiveTile() const;
	//returns true if the user has changed the selceted tile false otherwise
	bool selectedTileChanged();
	//returns true if the delete buttons is pressed so that we can delete this editor
	bool needsToBeDeleted();
	//returns true if the load button is pressed so that we can update the button's texture outside of this class
	bool loadButtonPressed();
	//updates the Tileset and all its properties
	void update();
	//renders the preview Tileset and the editing window if it was activated
	void render();
	//renders the tileset editor window if opened
	void renderWindow();
	//closes the tileset editor window if open
	void closeWindow();
	//updates the editor window if open
	void updateWindow(float dt);
	//operator >> for reading in 
	friend std::istream& operator>>(std::istream& in, TilesetEditor& editor);
	//operator << for writing out
	friend std::ostream& operator<<(std::ostream& os, const TilesetEditor& editor);
private:
	//the class that manages the window where you edit the tileset
	class TilesetEditorWindow
	{
	public:
		//contruct the editor by passing in a reference to the Tileseteditor
		TilesetEditorWindow(TilesetEditor* editorInstance);
		//destructor delete raw pointers
		~TilesetEditorWindow();
		//opens the window for editing the tileset
		void open();
		//updates the nessesary stuff to edit the tileset
		void update(float dt);
		//renders all editor stuff including the window
		void render();
		//closes the window
		void close();
	private:
		//the window where we will edit the tilemap 
		WindowManager* m_window;
		//the instance of the actual editor
		TilesetEditor* m_editorInstance;
		//this view is used to draw the tileset with a viewport
		sf::View m_tilesetView;
		//the GUI object to add UI elements
		GUI m_gui;
		//storing the previous mouse position for view movement getting the mouse direction
		sf::Vector2f m_previousMousePosition;
		//the componenet that allows you to move your view
		std::unique_ptr<ViewMovementComponent> m_viewMovement;
		//represents the current grid position on the tileset map
		sf::Vector2i m_currentMouseGrid;
		//stores the index of the current selected tile
		sf::Vector2i m_selectedMouseTile;
		//a single text instance to save memory
		sf::Text m_text;
	private:
		//initilized the view for rendering
		void initView();
		//initilizes all UI related stuff such as buttons checkboxes and more
		void initUI();
		//updates all events such as window closed and mouse wheel scrolled and others
		void updateEvents();
		//updates all Ui related stuff such as buttons checkboxes and more 
		void updateUI();
		//updates the tileset related stuff such as selecting a tile with the mouse and more
		void updateTileset();
		//renders the background of the tileset editor
		void renderBackground();
		//renders all UI related stuff such as buttons checkboxes and more
		void renderUI();
		//renders the Tileset including all collision Tiles 
		void renderTileset();
		//updates the view movement with the mouse
		void updateViewMovement(float dt);
		//updates the mouse input so that you can select a tile
		void updateMouseInput();
		//updates the keyboard input such as key 'Q' key 'W' for the view
		void updateKeyboardInput();
		//resets the values showed on the text boxes
		void resetTextBoxValues();
		//renders all collision boxes over all tiles taking into account the collision flag of that tile
		void renderCollisionBoxes();
	};
	//we could use the Tile class as the Tile class
	//but it stores information such as the texture name and rect we dont need that so we can save memory
	//we only need to know the collision type from which side and the tile type from that we can contruct a tile by using these informations
	class TileData
	{
	public:
		//default contrcutor
		TileData();
		//the tiletype is a number that represents the type of the tile selceted by the user
		short m_tileType;
		//add the collision flag to the collision enum
		void addCollisionFlag(Tile::CollisionType collision);
		//removes the given enum flag from the collision enum
		void removeCollisionFlag(Tile::CollisionType collision);
		//return true if the collision flag is present in the enum, false otherwise
		bool containsCollisionFlag(Tile::CollisionType collision) const;
		//resets the value of the collision enum to none
		void resetCollisionFlags(Tile::CollisionType collision = Tile::CollisionType::None);
		//returns the collision enum
		Tile::CollisionType getCollision() const;
	private:		
		//the collision enum which determines from which side a object can collide with the tile
		int m_collision;
	};
private:
	//the size of each tile in pixels on the original tileset image
	sf::Vector2i m_tileSize;
	//the size of each tile in pixels on the view we draw with
	sf::Vector2f m_tileSizeOnView;
	//storing the amount of tiles that are inside the tileset texture width x height
	sf::Vector2i m_tileDimensions;
	//determines whether the seleceted tile was chagned
	bool m_selecetedTileChanged;
	//the texture name for the tileset texture
	std::string n_tilesetTextureName;
	//the view where is tileset is going to be drawn using this to draw the same tileset image by only changing the viewport size
	sf::View m_view;
	//the gui object where we add UI elements
	GUI m_gui;
	//storing the information of each tile in a 2d vector
	std::vector<std::vector<TileData>> m_tileData;
	//the editor window instance
	TilesetEditorWindow m_editorWindow;
	//represents the currnet index of the mouse on the tileset in the tiledata vector
	sf::Vector2i m_currentMouseTileIndex;
	//represents the index of the seleceted Tile by the user with the mouse
	sf::Vector2i m_selectedTileIndex;
	//the main window
	WindowManager* m_window;
	//making the Editor window class a friend so that it can modify all properties of the TilesetEditor instance
	friend class TilesetEditorWindow;
	
private:
	//renders the tileset and its components
	void renderTileset();
	//updates all UI related stuff such as buttons and other stuff
	void updateUI();
	//renders all UI related stuff such as Buttons and other stuff
	void renderUI();
	//initializes all buttons that we will use
	void initButtons();
	//initilizes the view and its size
	void initView();
	//loads a new tileset texture from files using filedialog
	void loadNewTileset();
	//resets the data of each tile to default
	void resetTileData();
	//resizes to tileData vector to the current tileDimension width x height
	void resizeTileDataVector();
	//reset the UI position after the tileset size and position has been set
	void resetUIPosition();
	//updates the tile index of the mouse in the m_tileData vector
	void updateMouseTileIndex();
};