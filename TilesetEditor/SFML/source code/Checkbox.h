#pragma once
#include "Button.h"
#include <functional>

class Checkbox : public UIBase
{
public:
	//default contruct a checkbox with the given window instance and a texture to apply when the checkbox is checked
	Checkbox(WindowManager& window, sf::Texture& checkedTexture);
	//returns true if the checkbox was clicked by the user false otherwise
	bool isChecked() const;
	//sets the button for the checkbox button
	void setButton(const Button& button);
	//sets the texture that will be displayed on top of the button when its checked
	void setCheckedTexture(sf::Texture& checkedTexture);
	//settings this to true will make the checkbox 'active' 
	//it also executes the checked or unchecked function depending on the new checked 
	//but only if execute function is true 
	void setChecked(bool newChecked, bool executeFunction = true);
	//updates the checkbox
	void update();
	//returns the button to modify it
	Button& getButton();
	//draws the checkbox to the target
	void render() override;
	//this function is going to execute once when the checkbox is checked
	void setCheckedFunction(std::function<void()> function);
	//this function is going to execute once when the checkbox is unchecked
	void setUncheckedFunction(std::function<void()> function);
private:
	//determines whether the checkbos is check or not
	bool m_checked;
	//the main button used to display the checkbox
	Button m_button;
	//the texture used to display whether the checkbox is checked or not
	sf::Texture* m_checkedTexture;
	//the function thats gonna be called when the checkbox is checked
	std::function<void()> m_checkedFunction;
	//the function thats gonna be called when the checkbox is unchecked
	std::function<void()> m_uncheckedFunction;
private:
	//renders the checkedTexture to the screen 
	void renderCheckedTexture() const;
};
