#include "stdafx.h"
#include "StateMachine.h"
#include "Application.h"

StateMachine::StateMachine(Application& app)
	:m_appInstance(&app), m_needsToPopState(false)
{
}

State* StateMachine::getTopState()
{
	if (m_states.empty())
	{
		std::cout << "trying to access the top element of an empty stack in StateMachine";
	}

	return m_states.top().get();
}

void StateMachine::popState()
{
	m_needsToPopState = true;
}

void StateMachine::popAllStates()
{
	// we dont need to call pop because when the stack gets out of scope it will manage it since we are using smart pointers
	//so only need to stop the game loop
	m_appInstance->stop();
}

void StateMachine::pushState(State* newState)
{
	m_states.emplace();
	m_states.top().reset(newState);
}

void StateMachine::updateEvents(const sf::Event& evnt)
{
	getTopState()->updateEvents(evnt);
}

void StateMachine::update(float dt)
{
	getTopState()->update(dt);
}

void StateMachine::render()
{
	getTopState()->render();
}

void StateMachine::updatePopState()
{
	if (m_needsToPopState) //if the user said that there is a state to pop, pop it and reset the boolean
	{
		m_states.pop();
		m_needsToPopState = false;
	}

	if (m_states.empty())
	{
		m_appInstance->stop();
	}
}

WindowManager* StateMachine::getWindow()
{
	return &m_appInstance->m_window;
}
