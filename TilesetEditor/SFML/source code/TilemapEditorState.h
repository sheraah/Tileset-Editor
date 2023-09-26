#pragma once
#include "Tilemap.h"
#include "EditorUI.h"
#include "State.h"

//this class is the whole tilemap editor and handles it
class TilemapEditorState : public State
{
public:
	//contructor that initilizes all nessesary attributes
	TilemapEditorState(StateMachine& stateMachineInstance);
	//saves all important stuff
	~TilemapEditorState();
	//updates the Editor and its compenents
	void update(float dt) override;
	//updates all events such as input and more
	void updateEvents(const sf::Event& evnt) override;
	//renders The Whole Editor and its components
	void render() override;

private:
	//the actual tilemap
	std::unique_ptr<Tilemap> m_tilemap;
	//the UI with which the user can interact with
	std::unique_ptr<EditorUI> m_editorUI;

private:
	//initizializes all nessesary objects
	void initObjects();
};