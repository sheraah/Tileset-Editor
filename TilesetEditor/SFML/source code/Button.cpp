#include "stdafx.h"
#include "Button.h"
#include "InputHandler.h"
#include "WindowManager.h"

Button::Button(WindowManager& window)
	: UIBase(window)
{
	//default color values
	setHoverColor(sf::Color(30, 30, 30, 100));
	setPressedColor(sf::Color(30, 30, 30, 150));
}

Button::~Button()
{

}

void Button::setPosition(sf::Vector2f position)
{
	m_buttonSprite.setPosition(position);

	resetTextDimensions();
}

void Button::setSize(sf::Vector2f size)
{
	m_buttonSprite.setSize(size);

	resetTextDimensions();
}

void Button::setTexture(const sf::Texture& texture)
{
	m_buttonSprite.setTexture(texture, true);
}

void Button::setText(const std::string& text)
{
	m_text.setString(text);

	resetTextDimensions();
}

void Button::setFont(const sf::Font& font)
{
	m_text.setFont(font);
}

void Button::setTextColor(sf::Color color)
{
	m_text.setFillColor(color);
}

void Button::update()
{
	updateState();
	updateColor();
}

void Button::setHoverColor(sf::Color color)
{
	m_hoverColor = color;
}

void Button::setPressedColor(sf::Color color)
{
	m_pressedColor = color;
}

bool Button::isPressedOnce() const
{
	return m_buttonState == ButtonStates::PressedOnce;
}

bool Button::isPressed() const
{
	//true either if the button is pressed once or it is pressed
	return isPressedOnce() || m_buttonState == ButtonStates::Pressed;
}

sf::Vector2f Button::getPosition() const
{
	return m_buttonSprite.getPosition();
}

sf::Vector2f Button::getSize() const
{
	return m_buttonSprite.getSize();
}

std::string Button::getText() const
{
	return m_text.getString();
}

sf::FloatRect Button::getBounds() const
{
	return m_buttonSprite.getBounds();
}

const sf::Texture& Button::getTexture()
{
	return *m_buttonSprite.get().getTexture();
}

void Button::render()
{
	m_window->draw(m_buttonSprite);
	m_window->draw(m_text);
}

void Button::updateColor()
{
	sf::Color newButtonColor;

	if (m_buttonState == ButtonStates::Idle) newButtonColor = sf::Color::White; // white if button is idle
	else if (m_buttonState == ButtonStates::Hovered) newButtonColor = m_hoverColor; // hover color if button is hovered
	else newButtonColor = m_pressedColor; // pressed color if button is pressed

	m_buttonSprite.get().setColor(newButtonColor);
}

void Button::updateState()
{
	m_buttonState = ButtonStates::Idle;  // first reset the state to idle

	if (!m_buttonSprite.contains(InputHandler::getMousePositionF(*m_window))) return; 

	m_buttonState = ButtonStates::Hovered; //if the mouse is on the button update it to hovered

	if (InputHandler::isPressed(sf::Mouse::Left, *m_window)) //after its state is hovered and the mouse is pressed update it to pressed
	{
		m_buttonState = ButtonStates::Pressed;
	}

	if (InputHandler::isPressed(sf::Mouse::Left, *m_window, InputHandler::InputMode::Once)) //if pressed once update it pressed once
	{
		m_buttonState = ButtonStates::PressedOnce;
	}
}

void Button::resetTextDimensions()
{
	//set text ogigin to mid
	m_text.setOrigin(
		m_text.getLocalBounds().width * 0.5f,
		m_text.getLocalBounds().height * 0.5f
	);
	// character size relative to the button height
	m_text.setCharacterSize(m_buttonSprite.getSize().y * 0.6f);

	//this variable determines how much space the text can take on the button 
	//0.1f means there should be at least 10% of the button size not filled with text 
	//5% left 5% right
	float remainingSizePercent = 0.1f;
	//resize the button is the text is greater than the button size 
	if (m_text.getLocalBounds().width >= m_buttonSprite.getSize().x * (1.f - remainingSizePercent))
	{	
		sf::Vector2f oldButtonCenter = m_buttonSprite.getPosition() + m_buttonSprite.getSize() * 0.5f;
		m_buttonSprite.setSize({ m_text.getLocalBounds().width * (1.f + remainingSizePercent), m_buttonSprite.getSize().y });

		m_buttonSprite.setPosition(oldButtonCenter - m_buttonSprite.getSize() * 0.5f);
	}

	m_text.setPosition(m_buttonSprite.getPosition() + m_buttonSprite.getSize() * 0.5f);
}
