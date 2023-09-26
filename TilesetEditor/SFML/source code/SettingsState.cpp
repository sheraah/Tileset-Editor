#include "stdafx.h"
#include "SettingsState.h"
#include "TextInput.h"
#include "WindowManager.h"
#include "Utils.h"
#include "Tilemap.h"
#include "Checkbox.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "ResizableSprite.h"
#include "StateMachine.h"
#include "Settings.h"

SettingsState::SettingsState(StateMachine& stateMachine, Tilemap& map)
	: State(stateMachine), m_tilemap(&map)
{
	initButtons();
	initCombos();
	initTextInputBoxes();
	initCheckboxes();
}

void SettingsState::update(float dt)
{
	m_gui.update();

	updateButtons();
	updateCombos();
	updateTextInputBoxes();
	updateCheckboxes();
}

void SettingsState::updateEvents(const sf::Event& evnt)
{
	m_gui.updateEvents(evnt);
}

void SettingsState::render()
{
	renderBackground();

	m_gui.render();

	renderTexts();
}

void SettingsState::initButtons()
{
	m_text.setFont(FontManager::getFont("woodUI"));

	Button button(*m_stateMachine->getWindow());
	button.setSize({ 170.f, 70.f });
	button.setTexture(TextureManager::getTexture("WoodButton"));
	button.setFont(FontManager::getFont("woodUI"));

	button.setPosition({ 665.f, 400.f });
	button.setText("Back");
	button.setTextColor(sf::Color(49, 44, 42));
	button.setHoverColor(sf::Color(150, 117, 87, 100));

	m_gui.add("backButton", button);

	button.setPosition({ 665.f, 520.f });
	button.setText("Quit");
	button.setTextColor(sf::Color(255, 20, 20));
	button.setHoverColor(sf::Color(150, 117, 87, 100));

	m_gui.add("quitButton", button);
}

void SettingsState::initCombos()
{
	Button resolutionComboButton(*m_stateMachine->getWindow());
	resolutionComboButton.setTexture(TextureManager::getTexture("WoodButton"));
	resolutionComboButton.setFont(FontManager::getFont("woodUI"));
	resolutionComboButton.setSize({ 170.f, 40.f });
	resolutionComboButton.setTextColor(sf::Color(30, 30, 30));
	resolutionComboButton.setHoverColor(sf::Color(150, 117, 87, 100));

	m_gui.add("resolutionCombo", Combo<sf::Vector2u>(*m_stateMachine->getWindow()));
	getResolutionCombo().setPosition({ 75.f, 35.f });
	getResolutionCombo().setButton(resolutionComboButton);

	sf::Vector2u currentRes = m_stateMachine->getWindow()->getSize();

	getResolutionCombo().addElement(std::to_string(currentRes.x) + "x" + std::to_string(currentRes.y), currentRes);

	for (const sf::VideoMode& mode : sf::VideoMode::getFullscreenModes())
	{
		getResolutionCombo().addElement(std::to_string(mode.width) + "x" + std::to_string(mode.height), { mode.width, mode.height });
	}
}

void SettingsState::initCheckboxes()
{
	Button checkboxButton(*m_stateMachine->getWindow());

	checkboxButton.setTexture(TextureManager::getTexture("InputBox"));
	checkboxButton.setSize({ 50.f, 60.f });
	checkboxButton.setPosition({ 600.f , 650.f });

	m_gui.add("SingleDrawBox", Checkbox(*m_stateMachine->getWindow(), TextureManager::getTexture("CheckboxChecked")));
	m_gui.get<Checkbox>("SingleDrawBox").setButton(checkboxButton);
	m_gui.get<Checkbox>("SingleDrawBox").setChecked(m_tilemap->getSingleTileDrawMode());

	m_gui.add("OverrideTileBox", Checkbox(*m_stateMachine->getWindow(), TextureManager::getTexture("CheckboxChecked")));
	m_gui.get<Checkbox>("OverrideTileBox").setButton(checkboxButton);
	m_gui.get<Checkbox>("OverrideTileBox").getButton().setPosition(checkboxButton.getPosition() + sf::Vector2f(0.f, 180.f));
	m_gui.get<Checkbox>("OverrideTileBox").setChecked(m_tilemap->getOverrideTile());
}

void SettingsState::initTextInputBoxes()
{
	Button textInputBoxButton(*m_stateMachine->getWindow());
	textInputBoxButton.setSize({ 105.f, 40.f });
	textInputBoxButton.setTextColor(sf::Color(13, 71, 161));
	textInputBoxButton.setTexture(TextureManager::getTexture("InputBox"));
	textInputBoxButton.setPosition({ 330.f, 70.f });
	textInputBoxButton.setFont(FontManager::getFont("woodUI"));

	m_gui.add("tilemapResolutionWidth", TextInput(*m_stateMachine->getWindow()));
	m_gui.get<TextInput>("tilemapResolutionWidth").setButton(textInputBoxButton);
	m_gui.get<TextInput>("tilemapResolutionWidth").setBlankText("w...");
	m_gui.get<TextInput>("tilemapResolutionWidth").setText(std::to_string((int)m_tilemap->getTilemapSize().x));
	m_gui.get<TextInput>("tilemapResolutionWidth").setTextLimit(4);

	m_gui.add("tilemapResolutionHeight", TextInput(*m_stateMachine->getWindow()));
	m_gui.get<TextInput>("tilemapResolutionHeight").setButton(textInputBoxButton);
	m_gui.get<TextInput>("tilemapResolutionHeight").setBlankText("h...");
	m_gui.get<TextInput>("tilemapResolutionHeight").getButton().setPosition({ 330.f, 150.f });
	m_gui.get<TextInput>("tilemapResolutionHeight").setText(std::to_string((int)m_tilemap->getTilemapSize().y));
	m_gui.get<TextInput>("tilemapResolutionHeight").setTextLimit(4);

	m_gui.add("tilemapSizeWidth", TextInput(*m_stateMachine->getWindow()));
	m_gui.get<TextInput>("tilemapSizeWidth").setButton(textInputBoxButton);
	m_gui.get<TextInput>("tilemapSizeWidth").setBlankText("w...");
	m_gui.get<TextInput>("tilemapSizeWidth").getButton().setPosition({ 330.f, 330.f });
	m_gui.get<TextInput>("tilemapSizeWidth").setText(std::to_string((int)m_tilemap->getMapSize().x));
	m_gui.get<TextInput>("tilemapSizeWidth").setTextLimit(4);

	m_gui.add("tilemapSizeHeight", TextInput(*m_stateMachine->getWindow()));
	m_gui.get<TextInput>("tilemapSizeHeight").setButton(textInputBoxButton);
	m_gui.get<TextInput>("tilemapSizeHeight").setBlankText("h...");
	m_gui.get<TextInput>("tilemapSizeHeight").getButton().setPosition({ 330.f, 400.f });
	m_gui.get<TextInput>("tilemapSizeHeight").setText(std::to_string((int)m_tilemap->getMapSize().y));
	m_gui.get<TextInput>("tilemapSizeHeight").setTextLimit(4);
	m_gui.get<TextInput>("tilemapSizeHeight").getButton().setTextColor(sf::Color(5, 137, 173));

	m_gui.add("cameraSpeed", TextInput(*m_stateMachine->getWindow()));
	m_gui.get<TextInput>("cameraSpeed").setButton(textInputBoxButton);
	m_gui.get<TextInput>("cameraSpeed").setBlankText("...");
	m_gui.get<TextInput>("cameraSpeed").getButton().setPosition({ 1060.f, 100.f });
	m_gui.get<TextInput>("cameraSpeed").setText(std::to_string((int)m_tilemap->getCameraSpeed()));
	m_gui.get<TextInput>("cameraSpeed").setTextLimit(4);
	m_gui.get<TextInput>("cameraSpeed").getButton().setTextColor(sf::Color(50, 168, 82));
}

void SettingsState::renderBackground()
{
	ResizableSprite background;

	background.setSize(m_stateMachine->getWindow()->getOriginalResolutionF());
	background.setTexture(TextureManager::getTexture("SettingsStateBg"));

	m_stateMachine->getWindow()->draw(background);
}

void SettingsState::renderTexts()
{
	sf::Vector2f ComboPos = getResolutionCombo().getPosition();
	sf::Vector2f ComboTextPosition = ComboPos + sf::Vector2f(getResolutionCombo().getBounds().width * 0.5f, -20.f);

	Utils::setupText(m_text, "Resolution", 30, ComboTextPosition, sf::Color(200, 200, 200), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "Tilemap Resolution", 30, ComboTextPosition + sf::Vector2f(220.f, 20.f), sf::Color(79, 79, 79), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "width", 28, m_text.getPosition() + sf::Vector2f(-90.f, 55.f), sf::Color(150, 150, 150), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "height", 28, m_text.getPosition() + sf::Vector2f(0.f, 80.f), sf::Color(150, 150, 150), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "width", 28, m_text.getPosition() + sf::Vector2f(0.f, 170.f), sf::Color(150, 150, 150), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "height", 28, m_text.getPosition() + sf::Vector2f(0.f, 80.f), sf::Color(150, 150, 150), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "Camera Speed", 30, ComboTextPosition + sf::Vector2f(950.f, 40.f), sf::Color(79, 79, 79), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "Tilemap Size", 30, ComboTextPosition + sf::Vector2f(220.f, 270.f), sf::Color(79, 79, 79), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "Singe Tile Draw Mode", 30, { 440.f, 670.f } , sf::Color(79, 79, 79), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "Override Tile", 30, m_text.getPosition() + sf::Vector2f(0.f, 180.f), sf::Color(79, 79, 79), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "Left Mouse - Place Tile", 32, ComboTextPosition + sf::Vector2f(990.f, 350.f), sf::Color(217, 191, 24), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "Right Mouse - Move Camera", 32, m_text.getPosition() + sf::Vector2f(0.f, 50.f), sf::Color(217, 191, 24), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "Mouse Wheel - Zoom Camera", 32, m_text.getPosition() + sf::Vector2f(0.f, 50.f), sf::Color(217, 191, 24), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "Space - Delete Tile", 32, m_text.getPosition() + sf::Vector2f(0.f, 50.f), sf::Color(217, 191, 24), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "Q - Reset Camera Position", 32, m_text.getPosition() + sf::Vector2f(0.f, 50.f), sf::Color(217, 191, 24), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "W - Reset Camera Zoom", 32, m_text.getPosition() + sf::Vector2f(0.f, 50.f), sf::Color(217, 191, 24), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "E - Show Tile Grids", 32, m_text.getPosition() + sf::Vector2f(0.f, 50.f), sf::Color(217, 191, 24), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "A - Place Custom Point", 32, m_text.getPosition() + sf::Vector2f(0.f, 50.f), sf::Color(217, 191, 24), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "S - Show Cutstom Points", 32, m_text.getPosition() + sf::Vector2f(0.f, 50.f), sf::Color(217, 191, 24), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "F - Toggle Fullscreen", 32, m_text.getPosition() + sf::Vector2f(0.f, 50.f), sf::Color(217, 191, 24), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "Crtl + Z - undo drawing", 32, m_text.getPosition() + sf::Vector2f(0.f, 50.f), sf::Color(217, 191, 24), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);


}

void SettingsState::updateButtons()
{
	if (m_gui.get<Button>("backButton").isPressed())
	{
		m_stateMachine->popState();
	}

	if (m_gui.get<Button>("quitButton").isPressed())
	{
		m_stateMachine->popAllStates();
	}
}

void SettingsState::updateCombos()
{
	if (getResolutionCombo().wasChanged()) // if the combo value was swapped change the resolution to that value
	{
		m_stateMachine->getWindow()->resizeWindow(getResolutionCombo().getActiveValue());
		Settings::set("WindowSize", m_stateMachine->getWindow()->getSize());
	}
}

void SettingsState::updateTextInputBoxes()
{
	if (m_gui.get<TextInput>("tilemapResolutionWidth").textUpdated()) // if the width input box was changed update the tilemapresolution width
	{
		m_tilemap->setTilemapSize({ 
			Utils::StrToValue<float>(m_gui.get<TextInput>("tilemapResolutionWidth").getText()), 
			m_tilemap->getTilemapSize().y 
		});
	}

	else if (m_gui.get<TextInput>("tilemapResolutionHeight").textUpdated()) // if the height input box was changed update the tilemapresolution height
	{
		m_tilemap->setTilemapSize({
			m_tilemap->getTilemapSize().x, 
			Utils::StrToValue<float>(m_gui.get<TextInput>("tilemapResolutionHeight").getText())
		});
	}

	else if (m_gui.get<TextInput>("tilemapSizeWidth").textUpdated()) // if the width input box was changed update the tilemapsize width
	{
		m_tilemap->setMapSize({
			Utils::StrToValue<int>(m_gui.get<TextInput>("tilemapSizeWidth").getText()),
			m_tilemap->getMapSize().y 
		});
	}

	else if (m_gui.get<TextInput>("tilemapSizeHeight").textUpdated()) // if the height input box was changed update the tilemapsize height
	{
		m_tilemap->setMapSize({
			m_tilemap->getMapSize().x,
			Utils::StrToValue<int>(m_gui.get<TextInput>("tilemapSizeHeight").getText()) 
		});
	}

	else if (m_gui.get<TextInput>("cameraSpeed").textUpdated()) // if the height input box was changed update the tilemap height
	{
		m_tilemap->setCameraSpeed(Utils::StrToValue<float>(m_gui.get<TextInput>("cameraSpeed").getText()));
	}
}

void SettingsState::updateCheckboxes()
{
	m_tilemap->setSingleTileDrawMode(m_gui.get<Checkbox>("SingleDrawBox").isChecked());

	m_tilemap->setOverrideTile(m_gui.get<Checkbox>("OverrideTileBox").isChecked());
}

Combo<sf::Vector2u>& SettingsState::getResolutionCombo()
{
	return m_gui.get<Combo<sf::Vector2u>>("resolutionCombo");
}
