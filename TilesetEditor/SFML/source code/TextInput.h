#pragma once
#include "Button.h"

class WindowManager;

//allows the user to type in text and retrive its text
class TextInput : public UIBase
{
public:
	TextInput(WindowManager& windowInstance);
	//sets the maximum amount of characters that the user can type in
	void setTextLimit(uint32_t maxCharacters);
	//sets the button for the input box
	void setButton(const Button& button);
	//updates the input text
	void update();
	//updates the events when a key is pressed then add the key to that input text
	void updateEvents(const sf::Event& evnt) override;
	//the text that is displayed when the Text input bar is empty
	void setBlankText(const std::string& text);
	//sets the text
	void setText(const std::string& text);
	//returns wheater the text has changed after the update function
	bool textUpdated();
	//returns the text that the user has typed in
	//returns always an empty string if there is no conent of a user
	std::string getText();
	//draws the text to the target
	void render() override;
	//returns the button used as a inputbox
	Button& getButton();

private:
	//the text that is displayed when the text input bar is empty
	std::string m_blankText;
	//storing the currnet text without the | character and if its empty it is empty 
	std::string m_currentText;
	//the button used to as a inputbox
	Button m_button;
	//the maximum amount of characters the user can type in
	uint32_t m_characterLimit;
	//keeps track if the text was changed 
	bool m_wasTextUpdated;
	//determines weather the user has clicked on the input box 
	bool m_isActive;
	//the index of the charcter the user is currently appending to the user can change the index with the arrow keys
	size_t m_currnetWriteIndex;
private:
	//deletes the character before the write index. Returns true if it deleted the character successfully false otherwise
	bool deleteCharacter();
	//inserts the character after the current write index 
	void input(char character);
	//sets the inputbox unactive 
	void toggleOff();
	//updates if the input box was clicked or not
	void updateState();
	//updates the content of the text
	void updateText();
};