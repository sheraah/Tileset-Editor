#include "stdafx.h"
#include "CustomPointEditor.h"
#include "Tilemap.h"
#include "WindowManager.h"
#include "ViewMovementComponent.h"
#include "TextInput.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "Utils.h"
#include "InputHandler.h"
#include <fstream>

CustomPointEditor::CustomPointEditor(Tilemap& tilemap)
	:m_tilemap(&tilemap), m_window(new WindowManager())
{
	initView();
	initUI();
}

CustomPointEditor::~CustomPointEditor()
{
	loadToSave("resources/saves/CustomPoints.txt");

	delete m_window;
}

void CustomPointEditor::open()
{
	m_window->open({ 1050, 600 }, "CustomPointEditor", sf::Style::Close);
}

void CustomPointEditor::update(float dt)
{
	if (!m_window->getWindow().isOpen()) return; //if the editor windows isnt opened dont update its atributes

	updateEvents();

	updateKeyboardInput();

	m_viewMovement->update(dt);

	updateUI();
}

void CustomPointEditor::render()
{
	if (!m_window->getWindow().isOpen()) return; //if the window isnt opened dont render!

	m_window->clear();

	renderBackground();
	renderTilemap();
	renderUI();

	m_window->display();
}

void CustomPointEditor::close()
{
	m_window->getWindow().close();
}

void CustomPointEditor::updateEvents()
{
	sf::Event evnt;

	while (m_window->getWindow().pollEvent(evnt))
	{
		m_window->updateEvents(evnt);

		m_gui.updateEvents(evnt);

		//only if mouse is inside the tileset view update events such as scroll etc.
		if (m_viewMovement->getViewportBounds().contains(InputHandler::getMousePositionF(*m_window))) 
		{
			m_viewMovement->updateEvents(evnt);
		}
	}
}

void CustomPointEditor::initView()
{
	m_viewMovement = std::make_unique<ViewMovementComponent>(*m_window, m_view);
	m_viewMovement->setViewMoveSpeed(400.f);
	m_viewMovement->resetViewPosition();
	m_viewMovement->setMinZoom(0.25f);

	m_view.setSize(m_window->getOriginalResolutionF());
	m_view.setViewport({ 0.f, 0.f, 0.7f, 1.f }); // define a section for the tileset to display
}

void CustomPointEditor::updateKeyboardInput()
{
	//if mouse isnt inside the tileset view dont update keyboard events
	if (!m_viewMovement->getViewportBounds().contains(InputHandler::getMousePositionF(*m_window))) return;

	if (InputHandler::isPressed(sf::Keyboard::W, *m_window, InputHandler::InputMode::Once))
	{
		m_viewMovement->resetZoom();
	}

	else if (InputHandler::isPressed(sf::Keyboard::Q, *m_window, InputHandler::InputMode::Once))
	{
		m_viewMovement->resetViewPosition();
	}
}

void CustomPointEditor::initUI()
{
	m_text.setFont(FontManager::getFont("woodUI"));

	Button button(*m_window);

	button.setFont(FontManager::getFont("woodUI"));
	button.setTexture(TextureManager::getTexture("WoodButton"));
	button.setSize({ 170.f, 50.f });
	button.setText("Add Point Name");
	button.setPosition({ 1150.f, 200.f });

	m_gui.add<Button>("addPointName", button);
	m_gui.add<Button>("removePointName", button);
	m_gui.get<Button>("removePointName").setPosition({ 1150.f, 300.f });
	m_gui.get<Button>("removePointName").setText("Remove Point");

	m_gui.add<TextInput>("pointName", TextInput(*m_window));
	m_gui.get<TextInput>("pointName").setButton(button);
	m_gui.get<TextInput>("pointName").getButton().setTexture(TextureManager::getTexture("InputBox"));
	m_gui.get<TextInput>("pointName").getButton().setPosition({ 1150.f, 100.f });
	m_gui.get<TextInput>("pointName").setTextLimit(15);

	m_gui.add("customPoint", Combo<std::string>(*m_window));
	getCustomPointCombo().setPosition({ 1150.f, 450.f });
	getCustomPointCombo().setButton(button);
	getCustomPointCombo().addElement("None", std::string());

	loadFromSave("resources/saves/CustomPoints.txt");
}

void CustomPointEditor::updateUI()
{
	m_gui.update();

	if (m_gui.get<Button>("addPointName").isPressedOnce())
	{
		const std::string& pointName = m_gui.get<TextInput>("pointName").getText();
		if (!pointName.empty())
		{
			getCustomPointCombo().addElement(pointName, pointName);
			getCustomPointCombo().setActiveValue(pointName);

			m_customPointsData.insert({ pointName, std::vector<sf::Vector2f>() });
		}
	}

	else if (m_gui.get<Button>("removePointName").isPressedOnce())
	{
		if (!getCustomPointCombo().getActiveValue().empty())
		{
			m_customPointsData.erase(getCustomPointCombo().getActiveValue());
			getCustomPointCombo().removeElement(getCustomPointCombo().getActiveValue());
		}
	}
}

void CustomPointEditor::renderUI()
{
	m_gui.render();

	renderTexts();
}

void CustomPointEditor::renderBackground()
{
	ResizableSprite background;

	background.setSize(m_window->getOriginalResolutionF());
	background.setTexture(TextureManager::getTexture("TilesetWindowBg"));
	m_window->draw(background);
}

void CustomPointEditor::renderTexts()
{
	Utils::setupText(m_text, "Add Point", 50, { 1270.f, 40.f }, sf::Color(120, 120, 120), { 0.5f, 0.5f });
	m_window->draw(m_text);

	Utils::setupText(m_text, "Draw Point", 50, { 1270.f, 400.f }, sf::Color(120, 120, 120), { 0.5f, 0.5f });
	m_window->draw(m_text);
}

void CustomPointEditor::renderTilemap()
{
	m_window->getWindow().setView(m_view);

	m_tilemap->renderTilemap(*m_viewMovement);

	renderCustomPoints();

	m_window->resetView();
}

void CustomPointEditor::renderCustomPoints()
{	
	m_text.setScale(m_tilemap->getTilemapSizeScale());

	renderMouseCustomPoint();

	const std::string& activeString = getCustomPointCombo().getActiveValue();

	if (activeString.empty()) // if the selected custom point is NONE then render all points
	{
		for (const auto& [name, vec] : m_customPointsData)
		{
			for (const auto& position : vec)
			{
				renderCustomPoint(name, position, sf::Color::White);
			}
		}
	}
	else // else render all points with the active name
	{
		for (const auto& position : m_customPointsData[activeString])
		{
			renderCustomPoint(activeString, position, sf::Color::White);
		}
	}


	m_text.setScale({ 1.f, 1.f });
}

void CustomPointEditor::renderMouseCustomPoint()
{
	sf::Vector2f mousePositionView = m_window->screenToWorld(InputHandler::getMousePositionF(*m_window)) / m_tilemap->getTilemapSizeScale();

	if (!m_window->getBounds().contains(mousePositionView)) return;

	renderCustomPoint(getCustomPointCombo().getActiveValue(), mousePositionView, sf::Color(150, 150, 150, 200));

	if (InputHandler::isPressed(sf::Mouse::Left, *m_window, InputHandler::InputMode::Once))
	{
		m_customPointsData[getCustomPointCombo().getActiveValue()].push_back(mousePositionView);
	}
}

void CustomPointEditor::renderCustomPoint(const std::string& text, sf::Vector2f position, sf::Color color)
{
	if (text.empty()) return;

	const sf::Vector2f ScaledPosition = position * m_tilemap->getTilemapSizeScale();
	const sf::Vector2f rectangleSize = sf::Vector2f(25.f, 20.f) * m_tilemap->getTilemapSizeScale();
	const sf::Vector2f rectanglePosition = ScaledPosition - rectangleSize * 0.5f;

	if (InputHandler::isPressed(sf::Keyboard::Space, *m_window))
	{
		sf::Vector2f mousePositionWindow = InputHandler::getMousePositionF(*m_window);
		sf::Vector2f mousePositionView = m_window->screenToWorld(mousePositionWindow);

		if (sf::FloatRect(rectanglePosition, rectangleSize).contains(mousePositionView))
		{
			auto removeIt = std::remove(m_customPointsData[text].begin(), m_customPointsData[text].end(), position);

			if (removeIt == m_customPointsData[text].end()) return;

			m_customPointsData[text].erase(removeIt);
			return;
		}
	}

	sf::RectangleShape shape;

	shape.setSize(rectangleSize);
	shape.setFillColor(color);
	shape.setPosition(rectanglePosition);
	

	m_window->draw(shape);

	Utils::setupText(m_text, text, 16, ScaledPosition - sf::Vector2f(0.f, rectangleSize.y), sf::Color::Black, { 0.5f, 0.5f });	
	m_window->draw(m_text);
}

Combo<std::string>& CustomPointEditor::getCustomPointCombo()
{
	return m_gui.get<Combo<std::string>>("customPoint");
}


void CustomPointEditor::loadFromSave(const std::string& filePath)
{
	std::ifstream in(filePath);
	
	m_customPointsData.clear();

	std::string name;
	sf::Vector2f position;

	while (in >> name >> position)
	{	
		if (!m_customPointsData.contains(name)) getCustomPointCombo().addElement(name, name);

		m_customPointsData[name].push_back(position);
	}

	in.close();
}

void CustomPointEditor::loadToSave(const std::string& filePath)
{
	std::ofstream out(filePath);

	for (const auto& [name, vec] : m_customPointsData)
	{
		for (const auto& position : vec)
		{
 			out << name << " " << position << '\n';
		}
	}

	out.close();
}
