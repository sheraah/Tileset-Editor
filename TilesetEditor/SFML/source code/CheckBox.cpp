#include "stdafx.h"
#include "Checkbox.h"
#include "WindowManager.h"

Checkbox::Checkbox(WindowManager& window, sf::Texture& checkedTexture)
	: UIBase(window), m_checked(false), m_checkedTexture(&checkedTexture), m_button(*m_window)
{
}

bool Checkbox::isChecked() const
{
	return m_checked;
}

void Checkbox::setButton(const Button& button)
{
	m_button = button;
}

void Checkbox::setCheckedTexture(sf::Texture& checkedTexture)
{
	m_checkedTexture = &checkedTexture;
}

void Checkbox::setChecked(bool newChecked, bool executeFunction)
{
	m_checked = newChecked;

	if (!executeFunction) return;

	if (m_checked && m_checkedFunction) m_checkedFunction();
	if (!m_checked && m_uncheckedFunction) m_uncheckedFunction();
}

void Checkbox::update()
{
	m_button.update();

	if (m_button.isPressedOnce())
	{
		setChecked(!m_checked);
	}
}

Button& Checkbox::getButton()
{
	return m_button;
}

void Checkbox::render()
{
	m_button.render();

	if (m_checked) renderCheckedTexture();
}

void Checkbox::setCheckedFunction(std::function<void()> function)
{
	m_checkedFunction = function;
}

void Checkbox::setUncheckedFunction(std::function<void()> function)
{
	m_uncheckedFunction = function;
}

void Checkbox::renderCheckedTexture() const
{
	ResizableSprite checkedSprite;
	checkedSprite.setSize(m_button.getSize());
	checkedSprite.setPosition(m_button.getPosition());
	checkedSprite.setTexture(*m_checkedTexture);

	m_window->draw(checkedSprite);
}
