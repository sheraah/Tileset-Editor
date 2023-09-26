#include "stdafx.h"
#include "WindowManager.h"
#include <Windows.h>

WindowManager::WindowManager()
    :m_originalResolution({ 1500, 1000 }), m_hasFocus(true)
{
    m_window = std::make_unique<sf::RenderWindow>();
}

WindowManager::WindowManager(const sf::Vector2u originalResolution)
    : m_originalResolution(originalResolution), m_hasFocus(true)
{
    m_window = std::make_unique<sf::RenderWindow>();
}

void WindowManager::open(sf::Vector2u resolution, const std::string& windowTitle, sf::Uint32 windowSettings)
{
    if (m_window->isOpen()) return; // the the window is already created returns
   
    m_window->create(sf::VideoMode(getOriginalResolution().x, getOriginalResolution().y), windowTitle, windowSettings);
    
    resizeWindow(resolution);
}



sf::Vector2u WindowManager::getSize()
{
    return m_window->getSize();
}

sf::Vector2f WindowManager::getSizeF()
{
    return static_cast<sf::Vector2f>(m_window->getSize());
}

sf::RenderWindow& WindowManager::getWindow()
{
    return *m_window;
}

sf::Vector2f WindowManager::getWindowScale()
{
    return m_windowScale;
}

sf::Vector2u WindowManager::getOriginalResolution()
{
    return m_originalResolution;
}

sf::Vector2f WindowManager::getOriginalResolutionF()
{
    return static_cast<sf::Vector2f>(m_originalResolution);
}

void WindowManager::setWindowIcon(const sf::Texture& texture)
{
    sf::Image img = texture.copyToImage();

    m_window->setIcon(img.getSize().x, img.getSize().y, img.getPixelsPtr());
}

void WindowManager::resizeWindow(sf::Vector2u newResolution)
{
    if (sf::VideoMode(m_window->getSize().x, m_window->getSize().y) == sf::VideoMode::getDesktopMode())
    {
        m_window->setPosition(m_prevWindowPosition);
    }

    if (sf::VideoMode(newResolution.x, newResolution.y) == sf::VideoMode::getDesktopMode())
    {
        m_prevWindowPosition = m_window->getPosition();

        sf::Vector2i titleBarSize = getTitleBarSize();
        m_window->setPosition(-titleBarSize);
    }


    m_window->setSize(newResolution);

    calculateWindowScale(newResolution);
}

void WindowManager::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
    m_window->draw(drawable, states);
}

sf::Vector2f WindowManager::worldToScreen(sf::Vector2f worldPosition, const sf::View& view)
{
    //subtracts the view from the world position the top left of the view
    sf::Vector2f viewOffset = worldPosition - view.getCenter() + 0.5f * view.getSize();
    //caclculates the offset for the window by getting the view offset
    sf::Vector2f viewPosition = WindowManager::getOriginalResolutionF() * (viewOffset / view.getSize());

    sf::FloatRect viewport = view.getViewport();

    sf::Vector2f viewportPosition = sf::Vector2f(viewport.left, viewport.top) * getOriginalResolutionF();

    return viewportPosition + viewPosition * getViewportSizeScale(view);
}

sf::Vector2f WindowManager::worldToScreen(sf::Vector2f worldPosition)
{
    return worldToScreen(worldPosition, m_window->getView());
}

sf::Vector2f WindowManager::screenToWorld(sf::Vector2f screenPosition, const sf::View& view)
{
    screenPosition = screenToViewportPosition(screenPosition, view);

    //converts the screen position to viewposition
    sf::Vector2f viewOffset = screenPosition / getOriginalResolutionF() * view.getSize();
    //adds the top left position of the view to that offset
    sf::Vector2f worldPosition = viewOffset - 0.5f * view.getSize() + view.getCenter();

    return worldPosition;
}

sf::Vector2f WindowManager::screenToWorld(sf::Vector2f screenPosition)
{
    return screenToWorld(screenPosition, m_window->getView());
}

sf::Vector2f WindowManager::screenToViewportPosition(sf::Vector2f screenPosition, const sf::View& view)
{
    sf::Vector2f windowOffset = screenPosition / getOriginalResolutionF();
    sf::Vector2f viewPosition = getViewportPosition(view);

    return (screenPosition - viewPosition) / getViewportSizeScale(view);
}

sf::Vector2f WindowManager::screenToViewportPosition(sf::Vector2f screenPosition)
{
    return screenToViewportPosition(screenPosition, m_window->getView());
}

sf::Vector2f WindowManager::getViewZoom()
{
    //divides the window size by the view size to get the zoom
    sf::Vector2f Zoom = WindowManager::getOriginalResolutionF() / WindowManager::m_window->getView().getSize();

    return Zoom;
}

float WindowManager::getViewZoomF()
{
    sf::Vector2f Zoom = getViewZoom();

    //avarage zoom
    float ViewZoom = (Zoom.x + Zoom.y) * 0.5f;

    return ViewZoom;
}

void WindowManager::clear()
{
    m_window->clear();
}

void WindowManager::display()
{
    m_window->display();
}

void WindowManager::resetView()
{
    m_window->setView(m_window->getDefaultView());
}

sf::Vector2f WindowManager::getViewportSizeScale(const sf::View& view)
{
    sf::FloatRect viewport = view.getViewport();

    return sf::Vector2f(viewport.width, viewport.height);
}

sf::Vector2f WindowManager::getViewportPositionScale(const sf::View& view)
{
    sf::FloatRect viewport = view.getViewport();

    return sf::Vector2f(viewport.left, viewport.top);
}

sf::Vector2f WindowManager::getViewportSize(const sf::View& view)
{
    sf::FloatRect viewport = view.getViewport();
    return getViewportSizeScale(view) * getOriginalResolutionF();
}

sf::Vector2f WindowManager::getViewportPosition(const sf::View& view)
{
    return getViewportPositionScale(view) * getOriginalResolutionF();
}

float WindowManager::getViewportSizeScaleF(const sf::View& view)
{
    sf::Vector2f scale = getViewportSizeScale(view);
    return (scale.x + scale.y) * 0.5f;
}

sf::FloatRect WindowManager::getBounds()
{
    return sf::FloatRect({ 0.f, 0.f }, getOriginalResolutionF());
}

void WindowManager::updateEvents(const sf::Event& evnt)
{
    if (evnt.type == sf::Event::Closed) m_window->close();
    else if (evnt.type == sf::Event::LostFocus) m_hasFocus = false;
    else if (evnt.type == sf::Event::GainedFocus) m_hasFocus = true;
}

bool WindowManager::getHasFocus() const
{
    return m_hasFocus;
}



void WindowManager::calculateWindowScale(sf::Vector2u newResolution)
{
    //caculates the window scale by dividing the new resolution by the original resolution
    m_windowScale.x = static_cast<float>(newResolution.x) / getOriginalResolution().x;
    m_windowScale.y = static_cast<float>(newResolution.y) / getOriginalResolution().y;
}

sf::Vector2i WindowManager::getTitleBarSize()
{
    RECT Rect;
    GetWindowRect(m_window->getSystemHandle(), &Rect);
    POINT point = { 0, 0 };
    ClientToScreen(m_window->getSystemHandle(), &point);

    sf::Vector2i result;

    result.x = point.x - Rect.left;
    result.y = point.y - Rect.top;

    return result;
}
