#include "stdafx.h"
#include "TilemapEditorState.h"
#include "StateMachine.h"
#include <fstream>

TilemapEditorState::TilemapEditorState(StateMachine& stateMachineInstance)
	:State(stateMachineInstance)
{
	initObjects();
}

TilemapEditorState::~TilemapEditorState()
{
}

void TilemapEditorState::initObjects()
{
	m_tilemap = std::make_unique<Tilemap>(*m_stateMachine->getWindow());

	m_editorUI = std::make_unique<EditorUI>(*m_tilemap.get(), *m_stateMachine);
}

void TilemapEditorState::update(float dt)
{
	m_tilemap->update(dt);

	m_editorUI->update(dt);
}

void TilemapEditorState::updateEvents(const sf::Event& evnt)
{
	m_tilemap->updateEvents(evnt);

	m_editorUI->updateEvents(evnt);
}

void TilemapEditorState::render()
{
	m_editorUI->render();

	m_tilemap->render();
}
