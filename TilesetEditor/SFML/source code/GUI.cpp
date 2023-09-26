#include "stdafx.h"
#include "GUI.h"

void GUI::updateEvents(const sf::Event& evnt)
{
	m_GUIElements.iterResources([&evnt](UIBase& element)
	{
		element.updateEvents(evnt);
	});
}

void GUI::update()
{
	m_GUIElements.iterResources([](UIBase& element)
	{
		element.update();
	});
}

void GUI::render()
{
	m_GUIElements.iterResources([](UIBase& element)
	{
		element.render();
	});
}
