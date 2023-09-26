#pragma once
#include <stack>
#include "State.h"

class WindowManager;
class Application;

//a StateMachine class implemented as a Singleton 
//this class allows you to have states in your app using a stack
class StateMachine
{
public:
	//initlizes the StateMachine with the app instance
	StateMachine(Application& app);
	//returns a pointer to the top of the state stack
	State* getTopState();
	//pops the top state
	//notice that it pops it only after the current frame ends
	//if it pops the last state it ends the Application
	void popState();
	//pops all states and ends the application
	void popAllStates();
	//add a new state to the top
	void pushState(State* newState);
	//updates the events of the top state
	void updateEvents(const sf::Event& evnt);
	//updates the top state
	void update(float dt);
	//renders the top state
	void render();
	//call this function after the frame is done so we can pop the state if needed
	void updatePopState();
	//retruns the main window of the application instance
	WindowManager* getWindow();
private:
	//the instance of the Application class
	Application* m_appInstance;
	//storing a stack of states. This is useful because we only need the top element.
	std::stack<std::unique_ptr<State>> m_states;
	//determines whether the top state should be popped after the frame is done
	bool m_needsToPopState;
};