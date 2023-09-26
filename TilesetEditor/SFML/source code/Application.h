#pragma once
#include "StateMachine.h"
#include "WindowManager.h"

//the class where the whole application starts setting up all the stuff
class Application
{
public:
	//contructor that initilizes all nessesary attributes
	Application();
	//the entry point for the editor app
	void run();

private:
	//flag that shows if the game loop is active
	bool m_isRunning;
	//the state machine unuque_ptr because we want to contruct it only when all ohter object
	std::unique_ptr<StateMachine> m_stateMachine;
	//the main window
	WindowManager m_window;
	//making the StateMachine class a friend so we can use functions such as stop and other
	friend class StateMachine;
private:
	//updates all events in the app
	void updateEvents();
	//updates all objects needed every frame
	void update(float dt);
	//draw all objects to the screen
	void render();
	//load all fonts using the FontManager class
	void loadFonts();
	//initizializes all nessesary objects
	void initObjects();
	//loads all things need from save files
	void loadFromSaveFile();
	//saves all things needed after the window is closed
	void save();
	//stops the game loop and ends the app correctly
	void stop();
};