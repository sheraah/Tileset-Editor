#pragma once
#include <unordered_map>

class WindowManager;

//this class is a Singleton class that helps you to handle key and mouse presses. It also helps with getting the mouse position
class InputHandler
{
public:
	//determines if you want the key to trigger once or loop
	enum class InputMode { Loop, Once };

public:

	//gets the mosue position on the window as a Vector2f
	static sf::Vector2f getMousePositionF(WindowManager& window);
	//gets the mouse position on the window given in the parameters
	static sf::Vector2i getMousePosition(WindowManager& window);
	
	//update all nessesary events for keys
	static void updateEvents(const sf::Event& evnt);
	//updates all key inputs
	static void update();
	//checks if the key is held down or pressed based on the inputMode
	static bool isPressed(sf::Keyboard::Key keyboardKey, WindowManager& window, InputMode inputMode = InputMode::Loop);
	//checks if the keys are held down or preessed based on the inputMode
	static bool isPressed(std::vector<sf::Keyboard::Key> keyboardKeys, WindowManager& window, InputMode inputMode = InputMode::Loop);
	//check if the button is help down or pressed based on the inputMode
	static bool isPressed(sf::Mouse::Button mouseButton, WindowManager& window, InputMode inputMode = InputMode::Loop);

private:
	//stores the state for every to see what state they are on
	enum KeyStates { None = 0, HeldDown = 1, PressedOnce = 2 };
	//uses a keyboard key as a key and a keystate as a value to store the current state of that key.
	std::unordered_map<sf::Keyboard::Key, int> m_keyValues;
	//uses a mouse button as a key and a keystate as a value to store the current state of that button.
	std::unordered_map<sf::Mouse::Button, int> m_mouseValues;

private:
	//default constructor
	InputHandler();
	//getting the static instance(Singleton)
	static InputHandler& getInstance();
	//updating the keyboard events for the m_keyValues
	void updateKeyboardEvents(const sf::Event& evnt);
	//updating the mouse events for the m_mouseValues
	void updateMouseEvents(const sf::Event& evnt);
}; 