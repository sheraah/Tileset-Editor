#pragma once
#include "ResizableSprite.h"
#include "UIBase.h"

//this class draws a button with texture and text. It also handles all button events
class Button : public UIBase
{
public:
	//default contrcutor
	Button(WindowManager& window);
	//destructor
	~Button();
	//sets the position of the button
	void setPosition(sf::Vector2f position);
	//sets the size of the button
	void setSize(sf::Vector2f size);
	//sets the texture of the button
	void setTexture(const sf::Texture& texture);
	//sets the text of the button
	void setText(const std::string& text);
	//sets the font for the button text
	void setFont(const sf::Font& font);
	//sets the color of the button text
	void setTextColor(sf::Color color);
	//updates the button and its states
	void update() override;
	//sets the color of the button when you hover with your mouse over it
	void setHoverColor(sf::Color color);
	//sets the color of the button wehn you press the button
	void setPressedColor(sf::Color color);
	//returns true for 1 frame after the button is pressed
	bool isPressedOnce() const;
	//returns true when the button is pressed triggered every frame
	bool isPressed() const;
	//returns the top left position of the button
	sf::Vector2f getPosition() const;
	//returns the size of the button
	sf::Vector2f getSize() const;
	//returns the text that the button contains
	std::string getText() const;
	//returns the bounding box of the button
	sf::FloatRect getBounds() const;
	//returns the current texture of the button
	const sf::Texture& getTexture();
	//draws the button to the target
	void render() override;

private:
	//enum class that allows us the determine on what state our button is at the moment
	enum class ButtonStates { Idle, Hovered, PressedOnce, Pressed };
	//storing the button state 
	ButtonStates m_buttonState;
	//the actual button sprite as a pointer to use the forward delaration
	ResizableSprite m_buttonSprite;
	//the text that we draw over the button
	sf::Text m_text;
	//the color of the button when its hovered with the mouse
	sf::Color m_hoverColor;
	//the color of the button when the user pressed on the button
	sf::Color m_pressedColor;

private:
	//resets the size, position and more things of the text after resizing and repositioning the button
	void resetTextDimensions();
	//updates the color of the button sprite based on the button state
	void updateColor();
	//updates the button state
	void updateState();

};