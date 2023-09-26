#pragma once

class WindowManager;

//this class allows you to move the view in a direction with a specified speed. 
//sprecify the mouse button with which you want to move the view default is right mouse button
//it also allows you to zoom in and out with the mouse wheel with a specified zoom and speed
class ViewMovementComponent
{
public:
	//initizlizes the window and view 
	ViewMovementComponent(WindowManager& window, sf::View& view);
	//updates events for zooming in and out
	void updateEvents(const sf::Event& evnt);
	//updates the view movement
	void update(float dt);
	//sets the speed for the view movement default is 800
	void setViewMoveSpeed(float speed);
	//sets the key that triggers the view movement
	void setMoveTiggerKey(sf::Mouse::Button button);
	//sets the maximum zoom out 1.f is standart 2.f is 2 times as far . . .
	void setMaxZoom(float maxZoom);
	//sets the minimum zoom in 1.f is standart 0.5f is 2 times more zoomed in . . .
	void setMinZoom(float minZoom);
	//the speed of the zoom 0.1 is default
	void setZoomSpeed(float speed);
	//return the zoom factor
	float getZoomFactor() const;
	//resets the zoom to default
	void resetZoom();
	//resets the view position to the center
	void resetViewPosition();
	//sets the zoom factor < 1 means the objects appear bigger > 1 smaller
	void setZoomFactor(float newZoom);
	//returns the bounds of the viewport on the window
	sf::FloatRect getViewportBounds() const;
	//retruns the window instance
	WindowManager* getWindow();
	//returns the view instance
	sf::View* getView();
private:
	//the speed of the view movement
	float m_viewMoveSpeed;
	//the window instance
	WindowManager* m_window;
	//the view instnace
	sf::View* m_view;
	//the key which trggers view movement
	sf::Mouse::Button m_triggerKey;
	//storing the previousMousePosition for calculation the direction
	sf::Vector2f m_previousMousePosition;
	//determines how much the zoom is zoomed in < 1 Objects are bigger > 1 smaller
	float m_zoomFactor;
	//detmines how much the view can be zoomed out
	float m_maxZoom;
	//determines how much the view can be zoomed in
	float m_minZoom;
	//the zoom speed for every mouse scroll 
	float m_zoomSpeed;
private:

};