#pragma once
#include "Combo.h"
#include "GUI.h"

class WindowManager;
class Tilemap;
class ViewMovementComponent;

//this allows you to have a window where you can add and delete custom points on your map
class CustomPointEditor
{
public:
	//contruct the editor by passing in a reference to the Tileseteditor
	CustomPointEditor(Tilemap& tilemap);
	//destructor delete raw pointers
	~CustomPointEditor();
	//opens the window for 
	void open();
	//updates the nessesary stuff to add and delete custom points
	void update(float dt);
	//renders all editor stuff including the window
	void render();
	//closes the window
	void close();
	//reads in all the custom points given in the save file
	void loadFromSave(const std::string& filePath);
	//outputs all custom points to the save file
	void loadToSave(const std::string& filePath);
private:
	//the editor window
	WindowManager* m_window;
	//the tilemap instance to the main map
	Tilemap* m_tilemap;
	//this allows yout to move the view with a mouse button
	std::unique_ptr<ViewMovementComponent> m_viewMovement;
	//the view to draw the tilemap
	sf::View m_view;
	//define a GUI object where we can add different kinds of UI elements
	GUI m_gui;
	//the text object used to draw text
	sf::Text m_text;
	//storing a map of names and poisition vectors for the custom points
	std::unordered_map<std::string, std::vector<sf::Vector2f>> m_customPointsData;
private:
	//updates all nessesarry  window events
	void updateEvents();
	//initilizes the view and its components
	void initView();
	//renders the tilemap to the window
	void renderTilemap();
	//updates all keyboard inputs
	void updateKeyboardInput();
	//initilizes all Ui related stuff such as buttons and combos and more
	void initUI();
	//updates all UI related stuff
	void updateUI();
	//renders all UI related stuff
	void renderUI();
	//render background
	void renderBackground();
	//renders all texts 
	void renderTexts();
	//renders custom point based on the current selected point name
	void renderCustomPoints();
	//renders a preview of a custom point that will be drawn there
	void renderMouseCustomPoint();
	//renders a custom point at that position with the given color
	void renderCustomPoint(const std::string& text, sf::Vector2f position, sf::Color color);

	Combo<std::string>& getCustomPointCombo();
};