#pragma once

class StateMachine;

class State
{
public:
	//initilizes the stateMachine instance
	State(StateMachine& stateMachineInstance);
	//having a virual detructor is important to avoid memory leaks.
	virtual ~State();
	//the update function declared as a pure virual method that mean that every state needs to have an update function
	virtual void update(float dt) = 0;
	//the updateEvents function is declared as a virtual method that means you can override it but there is no need to
	virtual void updateEvents(const sf::Event& evnt);
	//the render function declared as a pure virual method that mean that every state needs to have a render function
	virtual void render() = 0;
	//storing the StateMachine innstance
	StateMachine* m_stateMachine;

private:
};