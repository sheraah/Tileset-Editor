#include "stdafx.h"
#include "TextInput.h"
#include "InputHandler.h"
#include "WindowManager.h"

#define BACKSPACE 8
#define ESCAPE 27
#define MINPRINTABLEACSII 32
#define MAXPRINTABLEACSII 126

TextInput::TextInput(WindowManager& window)
	: UIBase(window), m_wasTextUpdated(false), m_characterLimit(20), m_currnetWriteIndex(0), m_button(*m_window), m_isActive(false)
{
	setBlankText("Enter...");
}

void TextInput::setTextLimit(uint32_t maxCharacters)
{
	m_characterLimit = maxCharacters;
}

void TextInput::setButton(const Button& button)
{
	m_button = button;
	//if the text is empty set the text to the blankText string
	if (m_currentText.length() < 1) m_button.setText(m_blankText);
}

void TextInput::update()
{
	m_button.update();

	updateState();

	if (!m_isActive) return; //is the inputBox isnt active(not clicked) dont update the text

	updateText();
}

void TextInput::updateState()
{
	if (InputHandler::isPressed(sf::Mouse::Left, *m_window, InputHandler::InputMode::Once))
	{
		if (m_button.getBounds().contains(InputHandler::getMousePositionF(*m_window)))
		{
			m_isActive = true;
		}
		else
		{
			toggleOff();
		}
	}
}

void TextInput::updateText()
{
	std::string buttonText = m_currentText;
	buttonText.insert(buttonText.begin() + m_currnetWriteIndex, '|');

	m_button.setText(buttonText);
}

void TextInput::updateEvents(const sf::Event& evnt)
{
	if (!m_isActive) return;

	if (evnt.type == sf::Event::KeyPressed)
	{
		//update left and right arrow keys 
		if (evnt.key.code == sf::Keyboard::Left && m_currnetWriteIndex > 0) m_currnetWriteIndex--;
		else if (evnt.key.code == sf::Keyboard::Right && m_currnetWriteIndex < m_currentText.length()) m_currnetWriteIndex++;
	}

	if (evnt.type == sf::Event::TextEntered)
	{
		input(static_cast<char>(evnt.text.unicode));
	}
}

void TextInput::setBlankText(const std::string& text)
{
	m_blankText = text;

	if (m_currentText.length() < 1) m_button.setText(m_blankText);
}

void TextInput::setText(const std::string& text)
{
	m_currentText = text;
	m_currnetWriteIndex = m_currentText.length();

	if (m_currentText.length() > 0) m_button.setText(m_currentText);
}

bool TextInput::textUpdated()
{
	bool temp = m_wasTextUpdated;

	m_wasTextUpdated = false;

	return temp;
}

std::string TextInput::getText()
{
	return m_currentText;
}

void TextInput::render()
{
	m_button.render();
}

Button& TextInput::getButton()
{
	return m_button;
}

bool TextInput::deleteCharacter()
{
	if (m_currentText.length() < 1 || m_currnetWriteIndex < 1) return false;

	m_currentText.erase(m_currentText.begin() + m_currnetWriteIndex - 1);

	m_currnetWriteIndex--;

	return true;
}

void TextInput::input(char character)
{
	if (character == BACKSPACE)
	{
		m_wasTextUpdated = deleteCharacter();
		return;
	}

	if (character == ESCAPE)
	{
		toggleOff();
		return;
	}

	if (character < MINPRINTABLEACSII || character > MAXPRINTABLEACSII) return;

	if (m_currentText.length() >= m_characterLimit) return;

	m_currentText.insert(m_currentText.begin() + m_currnetWriteIndex, character);

	m_currnetWriteIndex++;

	m_wasTextUpdated = true;
}

void TextInput::toggleOff()
{
	if (m_isActive)
	{
		m_button.setText(m_currentText.length() > 0 ? m_currentText : m_blankText);
	}

	m_isActive = false;
}
