#pragma once

class WindowManager;

//this is the base class of all UI elements 
class UIBase 
{
public:
	//contructor to init the window for updating and rendering
	UIBase(WindowManager& window);
	//this function is here to update certain events for the UI element. 
	//override this function if you need to update something that has to do with evnets
	virtual void updateEvents(const sf::Event& evnt);
	//updates the UI element override this function
	virtual void update() = 0;
	//render the UI element override this function
	virtual void render() = 0;

protected:
	//the window instance for rendering and updating
	WindowManager* m_window;

private:

};