#include "stdafx.h"
#include "Application.h"
#include "Settings.h"
#include "TilemapEditorState.h"

Application::Application()
	:m_isRunning(true)
{
	loadFromSaveFile();
	loadFonts();
	initObjects();
}

void Application::loadFonts()
{
	FontManager::loadFont("woodUI", "resources/fonts/woodUI.ttf");
}

void Application::initObjects()
{
	m_window.open(Settings::get<sf::Vector2u>("WindowSize"), "Tilemap Editor");

	m_window.setWindowIcon(TextureManager::getTexture("WindowIcon"));

	m_stateMachine = std::make_unique<StateMachine>(*this); // contruct the state machine with the application instance

	m_stateMachine->pushState(new TilemapEditorState(*m_stateMachine.get())); // push the editor state
}

void Application::loadFromSaveFile()
{
	Settings::load("resources/saves/Settings.txt");
	TextureManager::load("resources/saves/TexturesSave.txt");
}

void Application::save()
{
	Settings::save("resources/saves/Settings.txt");
	TextureManager::save("resources/saves/TexturesSave.txt");
}

void Application::run()
{
	sf::Clock clock;

	//the game loop
	while (m_isRunning)
	{
		float deltaTime = clock.restart().asSeconds();

		updateEvents();

		update(deltaTime);

		render();

		m_stateMachine->updatePopState();
	}

	m_window.getWindow().close();

	save();
}

void Application::stop()
{
	m_isRunning = false;
}

void Application::updateEvents()
{
	sf::Event evnt;
	//updates all events needed and for all objects 
	while (m_window.getWindow().pollEvent(evnt))
	{
		m_window.updateEvents(evnt);

		m_stateMachine->updateEvents(evnt);
	}

	m_isRunning = m_window.getWindow().isOpen();
}

void Application::update(float dt)
{
	InputHandler::update();

	m_stateMachine->update(dt);
}

void Application::render()
{
	m_window.clear();

	m_stateMachine->render();

	m_window.display();
}