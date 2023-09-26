#pragma once
#include "SettingsState.h"
#include "TilesetEditor.h"
#include <array>
#include "GUI.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "StateMachine.h"

class Tilemap;
class CustomPointEditor;

//this class handles the UI for the tilemap editor
class EditorUI
{
public:
	//contructor the whole UI
	EditorUI(Tilemap& map, StateMachine& stateMachine);
	//dersturctor
	~EditorUI();
	//updates all needed events for the UI
	void updateEvents(const sf::Event& evnt);
	//updates the whole UI
	void update(float dt);
	//renders the whole UI
	void render();

private:
	//we will have some buttons on the right side of the UI
	//and when we click we will be able to set the current tileset editor this struct stores the button and the tileset editor
	struct TilesetEditorButtonData
	{
		//unique_ptr because i dont want to default contruct it
		std::unique_ptr<TilesetEditor> m_tilesetEditor;
		//the button used to display the tileset texture as a selcection option
		std::unique_ptr<Button> m_button;
	};

private:
	//used tp know where to draw the UI relative to the tilemap.
	//storing the instnace of the tilemap in the app
	Tilemap* m_tilemap;
	//the GUI object where we can add different kinds of UI elements that inherit from UIBase
	GUI m_gui;
	//text used to display text on the screen
	sf::Text m_text;
	//storing instances or tileset editors so that we can have muliple tileset where we can choose from and draw
	std::array<TilesetEditorButtonData, 6> m_tilesetEditors;
	//the editor where we add custom points and it also handles the whole editor window
	std::unique_ptr<CustomPointEditor> m_customPointEditor;
	//storing a pointer to the current tileset editor that we are working with at the moment
	TilesetEditorButtonData* m_currentTilesetEditor;
	//stateMachine instance
	StateMachine* m_stateMachine;
private:
	//render the background UI
	void renderBackground();
	//renders all texts to the screen
	void renderTexts();
	//updates all buttons
	void updateButtons();
	//loads a background from a file dialog
	void loadBackground();
	//initializes all buttons
	void initGUI();
	//initilizes all Tileset editors saved in the save file
	void loadTilesetEditors(const std::string& filePath);
	//adds a button to the GUI with the given parameters
	void addButton(const std::string& name, const std::string& buttonText, sf::Vector2f position, sf::Vector2f size);
	//adds a textinputbox to the GUI with the given parameters
	void addTextInputBox(const std::string& name, sf::Vector2f position, sf::Vector2f size, const std::string& blankText, size_t textLimit, const std::string& text);
	//adds a new combo to the GUI with the given parameters and the template argument for the combo value type
	template<typename ComboType>
	void addCombo(const std::string& name, sf::Vector2f position, sf::Vector2f elementSize);
	//saves the whole map including the tileseteditors and more using a filedialog to save it
	void saveMap();
	//opens a filedialog and lets the user to load a tileset and add it to the workspace
	void loadTilesetFromFile();
	//updates the current tileset editor and all its components
	void updateTilesetEditor(float dt);
	//renders the current tileset editor and all its components
	void renderTilesetEditor();
	//takes in an index of which the tilesetEditor data we should reset and load from TextureName Tileset + tostr(index)
	void resetTilesetEditorObject(size_t index);
	//returns the first index of a TilesetEditorButton that is empty 
	size_t getNextAviableTilesetEditorIndex();
	//deletes the current seleceted tileset editor instance including its button
	void deleteTilesetEditorInstance(size_t index);
	//saves all the important data to saves files such as Tilesets and more
	void saveTilesetDataToFile(const std::string& filePath);
	//loads whole map from a folder path that the user can select using the fileDialog clas
	void loadMap();
};

template<typename ComboType>
inline void EditorUI::addCombo(const std::string& name, sf::Vector2f position, sf::Vector2f elementSize)
{
	m_gui.add(name, Combo<ComboType>(*m_stateMachine->getWindow()));

	Button ComboButton(*m_stateMachine->getWindow());

	ComboButton.setTexture(TextureManager::getTexture("BlackButton"));
	ComboButton.setFont(FontManager::getFont("woodUI"));
	ComboButton.setSize(elementSize);
	ComboButton.setTextColor(sf::Color(200, 200, 200));
	ComboButton.setHoverColor(sf::Color(150, 117, 87, 100));


	m_gui.get<Combo<int>>(name).setPosition(position);
	m_gui.get<Combo<int>>(name).setButton(ComboButton);
}
