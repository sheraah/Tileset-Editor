#include "stdafx.h"
#include "State.h"
#include "StateMachine.h"


State::State(StateMachine& stateMachineInstance)
	: m_stateMachine(&stateMachineInstance)
{
}

void State::updateEvents(const sf::Event& evnt)
{

}

State::~State()
{

}