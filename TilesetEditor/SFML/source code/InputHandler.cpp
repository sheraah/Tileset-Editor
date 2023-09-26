#include "stdafx.h"
#include "InputHandler.h"
#include "WindowManager.h"

InputHandler::InputHandler()
{

}

sf::Vector2f InputHandler::getMousePositionF(WindowManager& window)
{
	sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window.getWindow()));
	
	//getting the mousePosition based on the original resolution 
	mousePosition /= window.getWindowScale();

	return mousePosition;
}

sf::Vector2i InputHandler::getMousePosition(WindowManager& window)
{
	return static_cast<sf::Vector2i>(getMousePositionF(window));
}

void InputHandler::updateEvents(const sf::Event& evnt)
{
	getInstance().updateKeyboardEvents(evnt);
	getInstance().updateMouseEvents(evnt);
}

void InputHandler::update()
{
	//update keyboard input
	for (auto& [key, value] : getInstance().m_keyValues)
	{
		bool isPressed = sf::Keyboard::isKeyPressed(key);

		if (!isPressed) //if the key is not pressed set the state of that key to None
		{
			value = KeyStates::None;
			continue;
		}
		//if the keystate was None and the key is pressed add the flags HeldDown and PressedOnce
		if (value == KeyStates::None)
		{
			value |= KeyStates::HeldDown;
			value |= KeyStates::PressedOnce;
		}
		//if the key is pressed and the keystate was PressedOnce we set the state to HeldDown
		else if ((value & KeyStates::PressedOnce) != KeyStates::None)
		{
			value = KeyStates::HeldDown;
		}
	}

	//update mouse input
	for (auto& [button, value] : getInstance().m_mouseValues)
	{
		bool isPressed = sf::Mouse::isButtonPressed(button);

		if (!isPressed)
		{
			value = KeyStates::None;
			continue;
		}

		if (value == KeyStates::None)
		{
			value |= KeyStates::HeldDown;
			value |= KeyStates::PressedOnce;
		}

		else if ((value & KeyStates::PressedOnce) != KeyStates::None)
		{
			value = KeyStates::HeldDown;
		}
	}
}

bool InputHandler::isPressed(sf::Keyboard::Key keyboardKey, WindowManager& window, InputMode inputMode)
{
	if (!window.getHasFocus()) return false; //if the windows isnt focused by the user we dont want to update any key presses

	int value = getInstance().m_keyValues[keyboardKey];
	//the inputmode is once we check if the key state is pressed Once 
	if (inputMode == InputMode::Once) return (value & KeyStates::PressedOnce) != KeyStates::None; 

	return (value & KeyStates::HeldDown) != KeyStates::None;
}

bool InputHandler::isPressed(std::vector<sf::Keyboard::Key> keyboardKeys, WindowManager& window, InputMode inputMode)
{
	if (inputMode == InputMode::Loop)
	{
		//if one of the keys is not pressed return false
		for (size_t i = 0; i < keyboardKeys.size(); i++)
		{
			if (!isPressed(keyboardKeys[i], window, InputMode::Loop)) return false;
		}
		return true;
	}
	else if(inputMode == InputMode::Once)
	{
		//check if atleast one ket is pressed once and all other keys are pressed
		bool atLeastOneKeyPressedOnce = false;
		for (size_t i = 0; i < keyboardKeys.size(); i++)
		{
			if (!isPressed(keyboardKeys[i], window, InputMode::Loop)) return false;
			if (isPressed(keyboardKeys[i], window, InputMode::Once)) atLeastOneKeyPressedOnce = true;
		}
		return atLeastOneKeyPressedOnce;
	}
}


bool InputHandler::isPressed(sf::Mouse::Button mouseButton, WindowManager& window, InputMode inputMode)
{
	if (!window.getHasFocus()) return false;

	int value = getInstance().m_mouseValues[mouseButton];
	//if the mode is once return if the enum contains the PressedOnce
	if (inputMode == InputMode::Once) return (value & KeyStates::PressedOnce) != KeyStates::None;

	return (value & KeyStates::HeldDown) != KeyStates::None;
}

InputHandler& InputHandler::getInstance()
{
	static InputHandler instance; // static instnace(singleton)
	return instance;
}

void InputHandler::updateKeyboardEvents(const sf::Event& evnt)
{
	sf::Keyboard::Key key = evnt.key.code;

	//insert if the key doesnt exist
	if (evnt.type == sf::Event::KeyPressed)
	{
		getInstance().m_keyValues.insert({ key, KeyStates::None });
	}	
}

void InputHandler::updateMouseEvents(const sf::Event& evnt)
{
	sf::Mouse::Button button = evnt.mouseButton.button;

	//insert if the key doesnt exist
	if (evnt.type == sf::Event::MouseButtonPressed)
	{
		getInstance().m_mouseValues.insert({ button, KeyStates::None });
	}
}
