#include "stdafx.h"
#include "UIBase.h"

UIBase::UIBase(WindowManager& window)
	:m_window(&window)
{
}

void UIBase::updateEvents(const sf::Event& evnt)
{
}
