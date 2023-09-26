#include "stdafx.h"
#include "ViewMovementComponent.h"
#include "WindowManager.h"
#include "InputHandler.h"

ViewMovementComponent::ViewMovementComponent(WindowManager& window, sf::View& view)
	: m_window(&window), m_view(&view), m_viewMoveSpeed(800.f), m_triggerKey(sf::Mouse::Right), 
	m_zoomFactor(1.f), m_minZoom(0.5f), m_maxZoom(2.f), m_zoomSpeed(0.05f)
{
}

void ViewMovementComponent::updateEvents(const sf::Event& evnt)
{
	if (evnt.type == sf::Event::MouseWheelScrolled)
	{
		if (evnt.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
		{
			if (evnt.mouseWheelScroll.delta > 0)
			{
				m_zoomFactor -= m_zoomSpeed;
			}
			else if (evnt.mouseWheelScroll.delta < 0)
			{
				m_zoomFactor += m_zoomSpeed;
			}

			float newZoom = std::clamp(m_zoomFactor, m_minZoom, m_maxZoom);

			setZoomFactor(newZoom);
		}
	}
}

void ViewMovementComponent::update(float dt)
{
	if (!InputHandler::isPressed(m_triggerKey, *m_window)) return;

	//mouse screen position
	sf::Vector2f currentMousePosition = InputHandler::getMousePositionF(*m_window);
	//check if its in bounds
	if (!getViewportBounds().contains(currentMousePosition)) return;
	//convert the mouse position to the view position
	sf::Vector2f currentMousePositionView = m_window->screenToWorld(currentMousePosition, *m_view);
	//getting the direction of the mouse movement for moving the view
	sf::Vector2f mouseDirection = (m_previousMousePosition - currentMousePositionView).toUnit();

	m_view->setCenter(m_view->getCenter() + mouseDirection * m_viewMoveSpeed * dt);
	//set the previos mouse position to the current
	//calculating it again because the center has changed and the coordinates too
	m_previousMousePosition = m_window->screenToWorld(currentMousePosition, *m_view);
}

void ViewMovementComponent::setViewMoveSpeed(float speed)
{
	m_viewMoveSpeed = speed;
}

void ViewMovementComponent::setMoveTiggerKey(sf::Mouse::Button button)
{
	m_triggerKey = button;
}

void ViewMovementComponent::setMaxZoom(float maxZoom)
{
	m_maxZoom = maxZoom;
}

void ViewMovementComponent::setMinZoom(float minZoom)
{
	m_minZoom = minZoom;
}

void ViewMovementComponent::setZoomSpeed(float speed)
{
	m_zoomSpeed = speed;
}

float ViewMovementComponent::getZoomFactor() const
{
	return m_zoomFactor;
}

void ViewMovementComponent::resetZoom()
{
	m_zoomFactor = 1.f;
	m_view->setSize(m_window->getOriginalResolutionF());
}

void ViewMovementComponent::resetViewPosition()
{
	m_view->setCenter(m_window->getOriginalResolutionF() * 0.5f);
}

void ViewMovementComponent::setZoomFactor(float newZoom)
{
	m_zoomFactor = newZoom;

	m_view->setSize(m_window->getOriginalResolutionF());
	m_view->zoom(newZoom);
}

sf::FloatRect ViewMovementComponent::getViewportBounds() const
{
	sf::FloatRect viewport = m_view->getViewport();

	return sf::FloatRect(
		sf::Vector2f(viewport.left, viewport.top) * m_window->getOriginalResolutionF(),
		sf::Vector2f(viewport.width, viewport.height) * m_window->getOriginalResolutionF()
	);
}

WindowManager* ViewMovementComponent::getWindow()
{
	return m_window;
}

sf::View* ViewMovementComponent::getView()
{
	return m_view;
}
