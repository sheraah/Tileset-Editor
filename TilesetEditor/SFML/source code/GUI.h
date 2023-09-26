#pragma once
#include "ResourceManager.h"
#include "UIBase.h"

class GUI
{
public:
	//updates all GUI elements of all types using the updateEvents function of UIBase
	void updateEvents(const sf::Event& evnt);
	//updates all GUI elements of all types using the update function of UIBase
	void update();
	//render all GUI elements using render function of UIBase
	void render();
	//add a new UI element to GUI to update and render
	template<typename GUIType>
	void add(const std::string& name, const GUIType& element);
	//returns the UI element at the name and casts it to the subclass provided by the template
	template<typename GUIType>
	GUIType& get(const std::string& name);

private:
	//storing a resourcemanager of the UIBase class so that we can add new elements that derrive from UIBase
	ResourceManager<UIBase> m_GUIElements;
};

template<typename GUIType>
inline void GUI::add(const std::string& name, const GUIType& element)
{
	if (!std::is_base_of<UIBase, GUIType>::value) 
	{
		std::cerr << "Invalid GUI element type" << std::endl;
		return;
	}

	m_GUIElements.load<GUIType>(name, [&element]() { return element; });
}

template<typename GUIType>
inline GUIType& GUI::get(const std::string& name)
{
	if (GUIType* element = dynamic_cast<GUIType*>(&m_GUIElements[name]))
	{
		return *element;
	}

	std::cerr << "Invalid GUI element type" << std::endl;
}
