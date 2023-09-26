#pragma once
#include "State.h"
#include "GUI.h"
#include "Combo.h"

class Tilemap;

class SettingsState : public State
{
public:
	//contrcutor which initializes the tilemap instance
	SettingsState(StateMachine& stateMachine, Tilemap& map);
	//updates the whole Settings and its UI components
	void update(float dt) override;
	//updates all events such as input and other
	void updateEvents(const sf::Event& evnt) override;
	//renders all things needed for this whole state
	void render() override;
private:
	//initializes all buttons
	void initButtons();
	//initializes all Combos
	void initCombos();
	//initializes all checkboxes
	void initCheckboxes();
	//initializes all TextInput boxes
	void initTextInputBoxes();
	//renders the background for the UI
	void renderBackground();
	//renders all texts to the screen
	void renderTexts();
	//updates all buttons
	void updateButtons();
	//updates all Combos;
	void updateCombos();
	//updates all textinput boxes and all related stuff
	void updateTextInputBoxes();
	//updates all checkboxes
	void updateCheckboxes();
private:
	//define a GUI object where we add UI elements that inherit from UIBase
	GUI m_gui;
	//text used to display text on the screen. Resusing a singe text instance can be efficient for memory
	sf::Text m_text;
	//tilemap instance
	Tilemap* m_tilemap;

private:
	//returns the combo for resolution
	Combo<sf::Vector2u>& getResolutionCombo();
};