#pragma once
#include "Button.h"
#include "InputHandler.h"
#include "ResourceManager.h"


//a class that allows you to have buttons with values and changing them
template<typename T>
class Combo : public UIBase
{
public:
	//default contructor
	Combo(WindowManager& window);
	//destructor
	~Combo();
	//sets the top left position of the Comobo
	void setPosition(sf::Vector2f position);
	//sets the button that is used to display every single Comobo Element
	void setButton(const Button& button);
	//adds an element to the Combo
	void addElement(const std::string& buttonText, const T& element);
	//removes all elements contaning the button text
	void removeElement(const std::string& buttonTextElement);
	//updates the Combo
	void update() override;
	//returns the value of the element that is currently choosen by the user 
	const T& getActiveValue() const;
	//sets the active value of the combo
	void setActiveValue(const std::string& buttonText);
	//returns wheater the user has 'opened' the combo
	const bool isActive() const;
	//returns true when a element was swapped with the first one.
	const bool wasChanged();
	//returns the bounds of the combo including all buttons
	sf::FloatRect getBounds() const;
	//returns the top left position of the combo
	sf::Vector2f getPosition() const;
	//renders the Combo and all its buttons
	void render() override;

private:
	//a struct that stores information about every element in a combo
	struct ElementData
	{
		//the text that is displayed when this combo element button is drawn
		std::string m_buttonText;
		//the value stored under the combo element
		T m_value;
	};

private:
	//determines wheater the Combo is active.
	//a combo is active when the user has pressed the Combo to see all other elements
	bool m_isActive;
	//shows if the first elements was changed(swapped with another one)
	bool m_wasChanged;
	//the top left position of the combo
	sf::Vector2f m_position;
	//the Button we use to draw each Combo element
	Button m_elementButton;
	//the array of all elements storing their data
	std::vector<ElementData> m_elements;
private:
	//this function takes in the mouse position. 
	//It swaps the button data of the first element and the elements which the mousePosition is on
	void swap(sf::Vector2f mousePosition);
	//swaps the active value with the element that has the given button text
	void swap(const std::string& buttonText);
	//returns the bounds of the first element
	sf::FloatRect getFirstElementBounds() const;
};

template<typename T>
inline Combo<T>::Combo(WindowManager& window)
	: UIBase(window), m_isActive(false), m_wasChanged(false), m_elementButton(*m_window)
{

}

template<typename T>
inline Combo<T>::~Combo()
{

}

template<typename T>
inline void Combo<T>::setPosition(sf::Vector2f position)
{
	m_position = position;
}

template<typename T>
inline void Combo<T>::setButton(const Button& button)
{
	m_elementButton = button;
}

template<typename T>
inline void Combo<T>::addElement(const std::string& buttonText,  const T& element)
{
	//if a combo elemenment has the same value as the given value to add then return
	auto it = std::find_if(m_elements.begin(), m_elements.end(), [&element](const ElementData& data) { return data.m_value == element; });

	if (it != m_elements.end()) return;
	
	m_elements.emplace_back(buttonText, element); // push element to bottom of the combo
}

template<typename T>
inline void Combo<T>::removeElement(const std::string& buttonTextElement)
{
	auto findLambda = [&buttonTextElement](const ElementData& data) { return data.m_buttonText == buttonTextElement; };

	m_elements.erase(std::remove_if(m_elements.begin(), m_elements.end(), findLambda), m_elements.end());
}

template<typename T>
inline void Combo<T>::update()
{
	sf::Vector2f mousePosition = InputHandler::getMousePositionF(*m_window);

	if (!InputHandler::isPressed(sf::Mouse::Left, *m_window, InputHandler::InputMode::Once)) return; // if the mouse wasnt pressed return
	
	if (!(getBounds().contains(mousePosition))) //if whole bounds doenst cotain the mouse position and its pressed then returns and set active to false
	{
		m_isActive = false;
		return;
	}

	if (getFirstElementBounds().contains(mousePosition)) // if the mouse intersects with the first button and mouse is pressed then just toggle the active bool
	{
		m_isActive = !m_isActive;
		return;	
	}
	else
	{
		if (!m_isActive) return;
	}

	//if mouse was pressed and its inside bounds and not the first element
	//then swap the first element with the mouse element that belongs to the mouse position
	swap(mousePosition);
	m_wasChanged = true;
	m_isActive = false;
}

template<typename T>
inline const T& Combo<T>::getActiveValue() const
{
	return m_elements[0].m_value;
}

template<typename T>
inline void Combo<T>::setActiveValue(const std::string& buttonText)
{
	swap(buttonText);
}

template<typename T>
inline const bool Combo<T>::isActive() const
{
	return m_isActive;
}

template<typename T>
inline const bool Combo<T>::wasChanged()
{
	bool result = m_wasChanged;
	m_wasChanged = false;

	return result;
}

template<typename T>
inline sf::FloatRect Combo<T>::getFirstElementBounds() const
{
	return sf::FloatRect(m_position, m_elementButton.getSize());
}

template<typename T>
inline void Combo<T>::swap(sf::Vector2f mousePosition)
{
	//offset of the mousePosition
	float height = mousePosition.y - m_position.y;
	//index of the button based on the mouse offset
	size_t mouseButtonIndex = static_cast<size_t>(height / m_elementButton.getSize().y);

	if (mouseButtonIndex == 0 || mouseButtonIndex >= m_elements.size()) return;
	//swap the element with the first one
	std::swap(m_elements[0], m_elements[mouseButtonIndex]);
}

template<typename T>
inline void Combo<T>::swap(const std::string& buttonText)
{
	auto it = std::find_if(m_elements.begin(), m_elements.end(), 
		[&buttonText](const ElementData& data) { return data.m_buttonText == buttonText; });

	if (it == m_elements.end()) return;

	std::swap(m_elements[0], *it);
}

template<typename T>
inline sf::FloatRect Combo<T>::getBounds() const
{
	return sf::FloatRect(m_position, { m_elementButton.getSize().x, m_elementButton.getSize().y * m_elements.size() });
}

template<typename T>
inline sf::Vector2f Combo<T>::getPosition() const
{
	return m_position;
}

template<typename T>
inline void Combo<T>::render()
{
	if(m_elements.size() < 1) return;
	
	size_t buttonsToDraw = m_elements.size();

	if (!m_isActive) buttonsToDraw = 1; // sets the buttons to draw to 1 so we draw only 1 button if the combo isn't active

	for (size_t i = 0; i < buttonsToDraw; i++) // render all Combo element buttons
	{
		sf::Vector2f position{
			m_position.x,
			m_position.y + m_elementButton.getSize().y * i
		};

		m_elementButton.setPosition(position);
		m_elementButton.setText(m_elements[i].m_buttonText);

		m_elementButton.update();

		m_elementButton.render();
	}
}
