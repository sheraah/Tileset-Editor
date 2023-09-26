#pragma once

//Manages a main window as a Singleton
class WindowManager
{
public:
    //default constructor sets the Original resolution to 1500, 1000
    WindowManager();
    //sets the Original resoultion in the contructor
    WindowManager(const sf::Vector2u originalResolution);
    //initiliazes the window the parameters. If the window is alreadt initiliazed this function does nothing
    void open(sf::Vector2u resolution, const std::string& windowTitle, sf::Uint32 windowSettings = sf::Style::Close);
    //returns the window size
    sf::Vector2u getSize();
    //returns the window size as a Vector2f
    sf::Vector2f getSizeF();
    //returns the main window
    sf::RenderWindow& getWindow();
    //returns the window scale relative to the original resolution
    sf::Vector2f getWindowScale();
    //returns the original resolution used for sizes and positions
    sf::Vector2u getOriginalResolution();
    //returns the original resolution used for sizes and positions as a Vector2f
    sf::Vector2f getOriginalResolutionF();
    //sets the icon of the window to the given texture
    void setWindowIcon(const sf::Texture& texture);
    //rezizes the main window
    void resizeWindow(sf::Vector2u newResolution);
    //draw any drawable object
    void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
    //converts a world position to screen coordinates relative to the window using the provided view
    sf::Vector2f worldToScreen(sf::Vector2f worldPosition, const sf::View& view);
    //converts a world position to screen coordinates relative to the window using the view setted for the window
    sf::Vector2f worldToScreen(sf::Vector2f worldPosition);
    //converts a point on the screen to a actual world position taking into account the view and position
    sf::Vector2f screenToWorld(sf::Vector2f screenPosition, const sf::View& view);
    //converts a point on the screen to a actual world position taking into account the view of the window and position
    sf::Vector2f screenToWorld(sf::Vector2f screenPosition);
    //returns the position realtive to the viewport size and position from screenposition using the view
    sf::Vector2f screenToViewportPosition(sf::Vector2f screenPosition, const sf::View& view);
    //returns the position realtive to the viewport size and position from screenposition using the view of the window
    sf::Vector2f screenToViewportPosition(sf::Vector2f screenPosition);
    //returns the view zoom factor in the x and y axis
    sf::Vector2f getViewZoom();
    //returns the view zoom factor > 1 objects are bigger < 1 objects are smaller
    float getViewZoomF();
    //clears the window for drawing
    void clear();
    //displays the things drawn to the window
    void display(); 
    //sets the view back to the default window view;
    void resetView();
    //gets the viewport size scale relative to the window 
    sf::Vector2f getViewportSizeScale(const sf::View& view);
    //returns the left and top of the viewport
    sf::Vector2f getViewportPositionScale(const sf::View& view);
    //returns the size of the viewport of that view
    sf::Vector2f getViewportSize(const sf::View& view);
    //returns the position of the viewport
    sf::Vector2f getViewportPosition(const sf::View& view);
    //returns the avarage of the getViewportSizeScale function
    float getViewportSizeScaleF(const sf::View& view);
    //returns the bounds of the window
    sf::FloatRect getBounds();
    //updates events such as window closed and if the window is focused or not
    void updateEvents(const sf::Event& evnt);
    //returns true if the window is focused by the user false otherwise
    bool getHasFocus() const;
private:
    //the main window
    std::unique_ptr<sf::RenderWindow> m_window;
    //the offset after resizing the window from original Resolution
    sf::Vector2f m_windowScale;
    //storing the old window position when resizing the window to fullscreen the position is set to the top left
    //after resizing back we to keep track of the old window position
    sf::Vector2i m_prevWindowPosition;
    //detmines whether the window is focused or not
    bool m_hasFocus;
    //the resolution used for setting sizes and more
    const sf::Vector2u m_originalResolution;
private:
    //caclulates the window scale with that specified resolution
    void calculateWindowScale(sf::Vector2u newResolution);
    //returns the size of the titlebar
    sf::Vector2i getTitleBarSize();
};