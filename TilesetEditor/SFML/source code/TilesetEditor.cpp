#include "stdafx.h"
#include "ResizableSprite.h"
#include "TilesetEditor.h"
#include "TextureManager.h"
#include "WindowManager.h"
#include "InputHandler.h"
#include "Button.h"
#include "Checkbox.h"
#include "FontManager.h"
#include "FileDialog.h"
#include "WindowManager.h"
#include "Utils.h"
#include "TextInput.h"
#include "LayeredData.h"
#include "ViewMovementComponent.h"

TilesetEditor::TilesetEditor(WindowManager& window)
	: m_selecetedTileChanged(false), m_editorWindow(this), m_selectedTileIndex(-1, -1), m_currentMouseTileIndex(-1, -1), m_window(&window)

{
	initView();
	initButtons();
}

void TilesetEditor::setTilesetTexture(const std::string& tilesetTextureName)
{
	n_tilesetTextureName = tilesetTextureName;
}

void TilesetEditor::setTileSize(sf::Vector2i tileSizeInPixels)
{
	if (n_tilesetTextureName.empty()) return;

	if (tileSizeInPixels.x <= 0) tileSizeInPixels.x = 1;
	if (tileSizeInPixels.y <= 0) tileSizeInPixels.y = 1;

	sf::Vector2i textureSize = static_cast<sf::Vector2i>(TextureManager::getTexture(n_tilesetTextureName).getSize());

	if (textureSize.x < tileSizeInPixels.x) tileSizeInPixels.x = textureSize.x;
	if (textureSize.y < tileSizeInPixels.y) tileSizeInPixels.y = textureSize.y;

	m_tileSize = tileSizeInPixels;

	m_tileDimensions = static_cast<sf::Vector2i>(TextureManager::getTexture(n_tilesetTextureName).getSize()) / m_tileSize;

	m_tileSizeOnView = m_window->getOriginalResolutionF() / static_cast<sf::Vector2f>(m_tileDimensions);

	resizeTileDataVector();
}

void TilesetEditor::setTileDimension(sf::Vector2i tileDimension)
{
	if (n_tilesetTextureName.empty() || tileDimension.x <= 0 || tileDimension.y <= 0) return;

	sf::Vector2i textureSize = static_cast<sf::Vector2i>(TextureManager::getTexture(n_tilesetTextureName).getSize());

	m_tileDimensions = tileDimension;

	m_tileSize = textureSize / m_tileDimensions;

	m_tileSizeOnView = m_window->getOriginalResolutionF() / static_cast<sf::Vector2f>(m_tileDimensions);

	resizeTileDataVector();
}

void TilesetEditor::setPosition(sf::Vector2f position)
{
	const sf::FloatRect& viewport = m_view.getViewport();

	m_view.setViewport({ position / m_window->getOriginalResolutionF(), { viewport.width, viewport.height } });

	resetUIPosition();
}

void TilesetEditor::setTilesetSize(sf::Vector2f tilesetSize)
{
	const sf::FloatRect& viewport = m_view.getViewport();

	m_view.setViewport({ { viewport.left, viewport.top }, tilesetSize / m_window->getOriginalResolutionF() });

	resetUIPosition();
}

Tile TilesetEditor::getActiveTile() const
{
	if (m_selectedTileIndex == -1) return Tile();

	const TileData& mouseTileData = m_tileData[m_selectedTileIndex.x][m_selectedTileIndex.y];
	sf::IntRect rect(m_tileSize * m_selectedTileIndex, m_tileSize);

	Tile tile(mouseTileData.m_tileType, n_tilesetTextureName, rect, mouseTileData.getCollision());

	return tile;
}

bool TilesetEditor::selectedTileChanged()
{
	bool temp = m_selecetedTileChanged;
	m_selecetedTileChanged = false;

	return temp;
}

bool TilesetEditor::needsToBeDeleted()
{
	return m_gui.get<Button>("deleteButton").isPressed();
}

bool TilesetEditor::loadButtonPressed()
{
	return m_gui.get<Button>("loadButton").isPressedOnce();
}

void TilesetEditor::update()
{
	updateMouseTileIndex();
	updateUI();
}

void TilesetEditor::render()
{
	renderTileset();
	renderUI();
}

void TilesetEditor::renderWindow()
{
	m_editorWindow.render();
}

void TilesetEditor::closeWindow()
{
	m_editorWindow.close();
}

void TilesetEditor::updateWindow(float dt)
{
	m_editorWindow.update(dt);
}

void TilesetEditor::renderTileset()
{
	m_window->getWindow().setView(m_view);

	ResizableSprite tilesetSprite;
	tilesetSprite.setTexture(TextureManager::getTexture(n_tilesetTextureName));
	tilesetSprite.setSize(m_window->getOriginalResolutionF());

	m_window->draw(tilesetSprite); // draw the tileset image
	// draw a white rectangle over the hovered tile if the index is valid and its not the seleceted tile index
	if (m_currentMouseTileIndex != -1 && m_currentMouseTileIndex != m_selectedTileIndex)
	{
		sf::RectangleShape shape;

		shape.setFillColor(sf::Color(0, 0, 0, 100));
		shape.setPosition(sf::Vector2f(m_currentMouseTileIndex) * m_tileSizeOnView);
		shape.setSize(m_tileSizeOnView);

		m_window->draw(shape);
	}

	if (m_selectedTileIndex != -1) // draw a white rectange a bit denser if the selced tile index is valid
	{
		sf::RectangleShape shape;

		shape.setFillColor(sf::Color(0, 0, 0, 150));
		shape.setPosition(sf::Vector2f(m_selectedTileIndex) * m_tileSizeOnView);
		shape.setSize(m_tileSizeOnView);

		m_window->draw(shape);
	}

	m_window->resetView();
}

void TilesetEditor::updateUI()
{
	m_gui.update();

	if (m_gui.get<Button>("loadButton").isPressedOnce())
	{
		loadNewTileset();

		resetTileData();
	}

	if (m_gui.get<Button>("editButton").isPressedOnce())
	{
		m_editorWindow.open();
	}
}

void TilesetEditor::renderUI()
{
	m_gui.render();

}

void TilesetEditor::initButtons()
{
	Button button(*m_window);
	//setsize
	button.setTexture(TextureManager::getTexture("WoodButton"));
	button.setFont(FontManager::getFont("woodUI"));

	button.setText("load");
	button.setTextColor(sf::Color(122, 122, 122));
	m_gui.add("loadButton", button);

	button.setText("Delete");
	button.setTextColor(sf::Color(166, 90, 90));

	m_gui.add("deleteButton", button);

	button.setText("Edit");
	button.setTextColor(sf::Color(122, 122, 122));

	m_gui.add("editButton", button);

	resetUIPosition();
}

void TilesetEditor::initView()
{
	m_view.setSize(m_window->getOriginalResolutionF());
	m_view.setCenter(m_window->getOriginalResolutionF() * 0.5f);
}

void TilesetEditor::loadNewTileset()
{
	FileDialog fileDialog;

	fileDialog.addFilter("Images", "*.png;*.jpg");

	std::string filePath = fileDialog.openDialog();

	if (!filePath.empty())
	{
		TextureManager::loadTexture(n_tilesetTextureName, filePath);
	}

	setTileDimension(m_tileDimensions);
}

void TilesetEditor::resetTileData()
{
	for (size_t i = 0; i < m_tileData.size(); i++)
	{
		for (size_t j = 0; j < m_tileData[i].size(); j++)
		{
			m_tileData[i][j] = TileData();
		}
	}
}

void TilesetEditor::resizeTileDataVector()
{
	m_tileData.resize(m_tileDimensions.x);

	for (size_t i = 0; i < m_tileDimensions.x; i++)
	{
		m_tileData[i].resize(m_tileDimensions.y); 
	}
}

void TilesetEditor::resetUIPosition()
{
	const sf::FloatRect viewportOnScreen = sf::FloatRect(
		sf::Vector2f(m_view.getViewport().left, m_view.getViewport().top) * m_view.getSize(),
		sf::Vector2f(m_view.getViewport().width, m_view.getViewport().height) * m_view.getSize());


	sf::Vector2f topleftUIPosition = { viewportOnScreen.left + viewportOnScreen.width * 1.05f, viewportOnScreen.top};
	sf::Vector2f buttonSize = { 0.2f * viewportOnScreen.width,  0.12f * viewportOnScreen.height };
	float curentButtonHeight = topleftUIPosition.y + viewportOnScreen.height * 0.2f;

	m_gui.get<Button>("loadButton").setSize(buttonSize);
	m_gui.get<Button>("loadButton").setPosition({ topleftUIPosition.x, curentButtonHeight });
	m_gui.get<Button>("editButton").setSize(buttonSize);
	m_gui.get<Button>("editButton").setPosition({ topleftUIPosition.x, curentButtonHeight + viewportOnScreen.height * 0.2f });
	m_gui.get<Button>("deleteButton").setSize(buttonSize);
	m_gui.get<Button>("deleteButton").setPosition({ topleftUIPosition.x, curentButtonHeight + viewportOnScreen.height * 0.4f });
}

void TilesetEditor::updateMouseTileIndex()
{
	if (m_tileDimensions == 0 || m_tileSize == 0) return;

	sf::Vector2f viewMousePosition = m_window->screenToWorld(InputHandler::getMousePositionF(*m_window), m_view);

	if (!sf::FloatRect({ 0.f, 0.f }, m_view.getSize()).contains(viewMousePosition))
	{
		m_currentMouseTileIndex = { -1, -1 };
		return;
	}

	m_currentMouseTileIndex = sf::Vector2i(viewMousePosition / m_tileSizeOnView);

	if (InputHandler::isPressed(sf::Mouse::Left, *m_window, InputHandler::InputMode::Once))
	{
		m_selecetedTileChanged = true;
		m_selectedTileIndex = m_currentMouseTileIndex;
	}
}

TilesetEditor::TileData::TileData()
	: m_tileType(0)
{
	resetCollisionFlags();
}

void TilesetEditor::TileData::addCollisionFlag(Tile::CollisionType collision)
{
	m_collision |= static_cast<int>(collision);
}

void TilesetEditor::TileData::removeCollisionFlag(Tile::CollisionType collision)
{
	m_collision &= ~static_cast<int>(collision);
}

bool TilesetEditor::TileData::containsCollisionFlag(Tile::CollisionType collision) const
{
	return m_collision & static_cast<int>(collision);
}

void TilesetEditor::TileData::resetCollisionFlags(Tile::CollisionType collision)
{
	m_collision = static_cast<int>(collision);
}

Tile::CollisionType TilesetEditor::TileData::getCollision() const
{
	return static_cast<Tile::CollisionType>(m_collision);
}

TilesetEditor::TilesetEditorWindow::TilesetEditorWindow(TilesetEditor* editorInstance)
	: m_editorInstance(editorInstance), m_window(new WindowManager()),
	m_selectedMouseTile(0, 0), m_currentMouseGrid(-1, -1)
{
	initView();
	initUI();
}

TilesetEditor::TilesetEditorWindow::~TilesetEditorWindow()
{
	delete m_window;
}

void TilesetEditor::TilesetEditorWindow::open()
{
	m_window->open({ 1050, 650 }, "Tileset Editor", sf::Style::Close);

	m_gui.get<TextInput>("TileType").setText(std::to_string(m_editorInstance->m_tileData[m_selectedMouseTile.x][m_selectedMouseTile.y].m_tileType));

	resetTextBoxValues();
}

void TilesetEditor::TilesetEditorWindow::update(float dt)
{
	if (!m_window->getWindow().isOpen()) return;

	updateEvents();
	updateTileset();
	updateViewMovement(dt);
	updateMouseInput();
	updateKeyboardInput();
	updateUI();
}

void TilesetEditor::TilesetEditorWindow::render()
{
	if (!m_window->getWindow().isOpen()) return;

	m_window->clear();

	renderBackground();
	renderTileset();
	renderUI();

	m_window->display();
}

void TilesetEditor::TilesetEditorWindow::close()
{
	m_window->getWindow().close();
}

void TilesetEditor::TilesetEditorWindow::initView()
{
	m_viewMovement = std::make_unique<ViewMovementComponent>(*m_window, m_tilesetView);
	m_viewMovement->setViewMoveSpeed(600.f);
	m_viewMovement->setMinZoom(0.5f);
	m_viewMovement->setMaxZoom(2.f);
	m_viewMovement->setZoomSpeed(0.05f);

	m_tilesetView.setSize(m_window->getOriginalResolutionF());
	m_tilesetView.setCenter(m_tilesetView.getSize() * 0.5f);
	m_tilesetView.setViewport({ 0.f, 0.f, 0.7f, 1.f });
}

void TilesetEditor::TilesetEditorWindow::initUI()
{
	m_text.setFont(FontManager::getFont("woodUI"));

	TextInput textInput(*m_window);
	textInput.setTextLimit(2);

	Button button(*m_window);
	button.setSize({ 120.f, 50.f});
	button.setFont(FontManager::getFont("woodUI"));
	button.setTextColor(sf::Color(200, 200, 200));
	button.setTexture(TextureManager::getTexture("InputBox"));

	button.setPosition({ 1300.f, 90.f });
	textInput.setButton(button);
	m_gui.add("TilesetWidth", textInput);
	textInput.getButton().setPosition({ 1300.f, 170.f});
	m_gui.add("TilesetHeight", textInput);
	textInput.getButton().setPosition({ 1300.f, 270.f });
	m_gui.add("TileWidth", textInput);
	textInput.getButton().setPosition({ 1300.f, 350.f });
	m_gui.add("TileHeight", textInput);
	textInput.setTextLimit(3);
	textInput.getButton().setPosition({ 1300.f, 580.f });
	m_gui.add("TileType", textInput);



	Checkbox checkBox(*m_window, TextureManager::getTexture("CheckboxChecked"));
	button.setSize({ 50.f, 50.f });
	button.setPosition({ 1300.f, 430.f });
	checkBox.setButton(button);

	m_gui.add("ShowCollision", checkBox);
	m_gui.add("AllCollision", checkBox);
	checkBox.getButton().setSize({ 45.f, 45.f });
	m_gui.add("LeftCollision", checkBox);
	m_gui.add("RightCollision", checkBox);
	m_gui.add("TopCollision", checkBox);
	m_gui.add("BottomCollision", checkBox);

	m_gui.get<Checkbox>("AllCollision").getButton().setPosition({ 1300.f, 640.f });
	m_gui.get<Checkbox>("LeftCollision").getButton().setPosition({ 1310.f, 710.f });
	m_gui.get<Checkbox>("RightCollision").getButton().setPosition({ 1310.f, 770.f });
	m_gui.get<Checkbox>("TopCollision").getButton().setPosition({ 1310.f, 830.f });
	m_gui.get<Checkbox>("BottomCollision").getButton().setPosition({ 1310.f, 890.f });
	
	m_gui.get<Checkbox>("ShowCollision").setChecked(true);

	m_gui.get<Checkbox>("AllCollision").setCheckedFunction([this]()
	{
		m_gui.get<Checkbox>("LeftCollision").setChecked(true);
		m_gui.get<Checkbox>("RightCollision").setChecked(true);
		m_gui.get<Checkbox>("TopCollision").setChecked(true);
		m_gui.get<Checkbox>("BottomCollision").setChecked(true);
	});
	
	m_gui.get<Checkbox>("AllCollision").setUncheckedFunction([this]()
	{
		m_gui.get<Checkbox>("LeftCollision").setChecked(false);
		m_gui.get<Checkbox>("RightCollision").setChecked(false);
		m_gui.get<Checkbox>("TopCollision").setChecked(false);
		m_gui.get<Checkbox>("BottomCollision").setChecked(false);
	});

	auto addCollisionFlag = [this](Tile::CollisionType collision)
	{
		m_editorInstance->m_tileData[m_selectedMouseTile.x][m_selectedMouseTile.y].addCollisionFlag(collision);
	};

	auto removeCollisionFlag = [this](Tile::CollisionType collision)
	{
		m_editorInstance->m_tileData[m_selectedMouseTile.x][m_selectedMouseTile.y].removeCollisionFlag(collision);
	};

	m_gui.get<Checkbox>("LeftCollision").setCheckedFunction([addCollisionFlag]()
	{
		addCollisionFlag(Tile::CollisionType::Left);
	});

	m_gui.get<Checkbox>("LeftCollision").setUncheckedFunction([removeCollisionFlag]()
	{
		removeCollisionFlag(Tile::CollisionType::Left);
	});

	m_gui.get<Checkbox>("RightCollision").setCheckedFunction([addCollisionFlag]()
	{
		addCollisionFlag(Tile::CollisionType::Right);
	});

	m_gui.get<Checkbox>("RightCollision").setUncheckedFunction([removeCollisionFlag]()
	{
		removeCollisionFlag(Tile::CollisionType::Right);
	});

	m_gui.get<Checkbox>("TopCollision").setCheckedFunction([addCollisionFlag]()
	{
		addCollisionFlag(Tile::CollisionType::Top);
	});

	m_gui.get<Checkbox>("TopCollision").setUncheckedFunction([removeCollisionFlag]()
	{
		removeCollisionFlag(Tile::CollisionType::Top);
	});

	m_gui.get<Checkbox>("BottomCollision").setCheckedFunction([addCollisionFlag]()
	{
		addCollisionFlag(Tile::CollisionType::Bottom);
	});

	m_gui.get<Checkbox>("BottomCollision").setUncheckedFunction([removeCollisionFlag]()
	{
		removeCollisionFlag(Tile::CollisionType::Bottom);
	});
}

void TilesetEditor::TilesetEditorWindow::updateEvents()
{
	sf::Event evnt;

	while (m_window->getWindow().pollEvent(evnt))
	{
		m_window->updateEvents(evnt);
		
		m_gui.updateEvents(evnt);

		if (m_viewMovement->getViewportBounds().contains(InputHandler::getMousePositionF(*m_window)))
		{
			m_viewMovement->updateEvents(evnt);
		}
	}

	if(!m_window->getHasFocus()) m_editorInstance->m_selecetedTileChanged = true;
}

void TilesetEditor::TilesetEditorWindow::updateUI()
{
	m_gui.update();

	if (m_gui.get<TextInput>("TilesetWidth").textUpdated())
	{
		m_editorInstance->setTileDimension({ 
			Utils::StrToValue<int>(m_gui.get<TextInput>("TilesetWidth").getText()), 
			m_editorInstance->m_tileDimensions.y
		});

		if (m_gui.get<TextInput>("TilesetWidth").getText().length() > 0) resetTextBoxValues();
	}
	else if (m_gui.get<TextInput>("TilesetHeight").textUpdated())
	{
		m_editorInstance->setTileDimension({ 
			m_editorInstance->m_tileDimensions.x, 
			Utils::StrToValue<int>(m_gui.get<TextInput>("TilesetHeight").getText()) 
		});

		if (m_gui.get<TextInput>("TilesetHeight").getText().length() > 0) resetTextBoxValues();
	}
	else if (m_gui.get<TextInput>("TileWidth").textUpdated())
	{
		m_editorInstance->setTileSize({ 
			Utils::StrToValue<int>(m_gui.get<TextInput>("TileWidth").getText()),
			m_editorInstance->m_tileSize.y 
		});

		if (m_gui.get<TextInput>("TileWidth").getText().length() > 0) resetTextBoxValues();
	}
	else if (m_gui.get<TextInput>("TileHeight").textUpdated())
	{
		m_editorInstance->setTileSize({ 
			m_editorInstance->m_tileSize.x, 
			Utils::StrToValue<int>(m_gui.get<TextInput>("TileHeight").getText()) 
		});

		if(m_gui.get<TextInput>("TileHeight").getText().length() > 0) resetTextBoxValues();
	}

	else if (m_gui.get<TextInput>("TileType").textUpdated())
	{
		int newTileType = Utils::StrToValue<int>(m_gui.get<TextInput>("TileType").getText());

		if (newTileType < 0) return;

		m_editorInstance->m_tileData[m_selectedMouseTile.x][m_selectedMouseTile.y].m_tileType = newTileType;
	}
}

void TilesetEditor::TilesetEditorWindow::updateTileset()
{
}

void TilesetEditor::TilesetEditorWindow::renderBackground()
{
	sf::Vector2f backgroundSize = { m_window->getOriginalResolutionF().x * (1.f - m_tilesetView.getViewport().width), m_window->getOriginalResolutionF().y };
	sf::Vector2f backgroundPosition = { m_window->getOriginalResolutionF().x * m_tilesetView.getViewport().width, 0.f };

	ResizableSprite background;
	background.setTexture(TextureManager::getTexture("SettingsStateBg"));
	background.setSize(m_window->getOriginalResolutionF() * sf::Vector2f(m_tilesetView.getViewport().width, m_tilesetView.getViewport().height));
	m_window->draw(background);

	background.setSize(backgroundSize);
	background.setTexture(TextureManager::getTexture("TilesetWindowBg"), true); 
	background.setPosition(backgroundPosition);
	m_window->draw(background);
}

void TilesetEditor::TilesetEditorWindow::renderUI()
{
	Utils::setupText(m_text, "Tileset", 55, { 1270.f, 30.f }, sf::Color(156, 156, 156), { 0.5f, 0.5f });
	m_window->draw(m_text);

	Utils::setupText(m_text, "Tileset Width", 30, m_text.getPosition() + sf::Vector2f(-80.f, 80.f), sf::Color(200, 200, 200), {0.5f, 0.5f});
	m_window->draw(m_text);

	Utils::setupText(m_text, "Tileset Height", 30, m_text.getPosition() + sf::Vector2f(0.f, 85.f), sf::Color(200, 200, 200), { 0.5f, 0.5f });
	m_window->draw(m_text);

	Utils::setupText(m_text, "Tile Width", 30, m_text.getPosition() + sf::Vector2f(0.f, 90.f), sf::Color(200, 200, 200), { 0.5f, 0.5f });
	m_window->draw(m_text);

	Utils::setupText(m_text, "Tile Height", 30, m_text.getPosition() + sf::Vector2f(0.f, 85.f), sf::Color(200, 200, 200), { 0.5f, 0.5f });
	m_window->draw(m_text);

	Utils::setupText(m_text, "Show Collision", 30, m_text.getPosition() + sf::Vector2f(0.f, 80.f), sf::Color(200, 200, 200), { 0.5f, 0.5f });
	m_window->draw(m_text);

	Utils::setupText(m_text, "Tile", 55, m_text.getPosition() + sf::Vector2f(80.f, 70.f), sf::Color(156, 156, 156), { 0.5f, 0.5f });
	m_window->draw(m_text);

	Utils::setupText(m_text, "TileType", 30, m_text.getPosition() + sf::Vector2f(-80.f, 80.f), sf::Color(200, 200, 200), { 0.5f, 0.5f });
	m_window->draw(m_text);

	Utils::setupText(m_text, "Collision All", 30, m_text.getPosition() + sf::Vector2f(0.f, 60.f), sf::Color(200, 200, 200), { 0.5f, 0.5f });
	m_window->draw(m_text);

	Utils::setupText(m_text, "Collision Left", 30, m_text.getPosition() + sf::Vector2f(0.f, 70.f), sf::Color(200, 200, 200), { 0.5f, 0.5f });
	m_window->draw(m_text);

	Utils::setupText(m_text, "Collision Right", 30, m_text.getPosition() + sf::Vector2f(0.f, 60.f), sf::Color(200, 200, 200), { 0.5f, 0.5f });
	m_window->draw(m_text);

	Utils::setupText(m_text, "Collision Top", 30, m_text.getPosition() + sf::Vector2f(0.f, 60.f), sf::Color(200, 200, 200), { 0.5f, 0.5f });
	m_window->draw(m_text);

	Utils::setupText(m_text, "Collision Bottom", 30, m_text.getPosition() + sf::Vector2f(0.f, 60.f), sf::Color(200, 200, 200), { 0.5f, 0.5f });
	m_window->draw(m_text);

	m_gui.render();
}

void TilesetEditor::TilesetEditorWindow::renderTileset()
{
	m_window->getWindow().setView(m_tilesetView);

	ResizableSprite tileset;
	tileset.setSize(m_window->getOriginalResolutionF());

	tileset.setTexture(TextureManager::getTexture(m_editorInstance->n_tilesetTextureName), true);
	m_window->draw(tileset);

	// draw a white rectangle over the hovered tile if the index is valid and its not the seleceted tile index
	if (m_currentMouseGrid != -1 && m_currentMouseGrid != m_selectedMouseTile)
	{
		sf::RectangleShape shape;
	
		shape.setFillColor(sf::Color(0, 0, 0, 100));
		shape.setPosition(sf::Vector2f(m_currentMouseGrid) * m_editorInstance->m_tileSizeOnView);
		shape.setSize(m_editorInstance->m_tileSizeOnView);
	
		m_window->draw(shape);
	}
	
	if (m_selectedMouseTile != -1) // draw a white rectange a bit denser if the selced tile index is valid
	{
		sf::RectangleShape shape;
	
		shape.setFillColor(sf::Color(0, 0, 0, 150));
		shape.setPosition(sf::Vector2f(m_selectedMouseTile) * m_editorInstance->m_tileSizeOnView);
		shape.setSize(m_editorInstance->m_tileSizeOnView);
	
		m_window->draw(shape);
	}

	if (m_gui.get<Checkbox>("ShowCollision").isChecked())
	{
		renderCollisionBoxes();
	}

	m_window->resetView();
}

void TilesetEditor::TilesetEditorWindow::updateViewMovement(float dt)
{
	m_viewMovement->update(dt);
}

void TilesetEditor::TilesetEditorWindow::updateMouseInput()
{

	sf::Vector2f mousePositionOnTileset = m_window->screenToWorld(InputHandler::getMousePositionF(*m_window), m_tilesetView);

	bool allChecked = m_gui.get<Checkbox>("LeftCollision").isChecked() && m_gui.get<Checkbox>("RightCollision").isChecked() &&
		m_gui.get<Checkbox>("TopCollision").isChecked() && m_gui.get<Checkbox>("BottomCollision").isChecked();

	m_gui.get<Checkbox>("AllCollision").setChecked(allChecked, false);

	if (!m_window->getBounds().contains(mousePositionOnTileset))
	{
		m_currentMouseGrid = { -1, -1 };
		return;
	}

	m_currentMouseGrid = sf::Vector2i(mousePositionOnTileset / m_editorInstance->m_tileSizeOnView);

	if (!InputHandler::isPressed(sf::Mouse::Left, *m_window)) return;

	m_selectedMouseTile = m_currentMouseGrid;

	TileData& currentTile = m_editorInstance->m_tileData[m_selectedMouseTile.x][m_selectedMouseTile.y];

	m_gui.get<TextInput>("TileType").setText(std::to_string(currentTile.m_tileType));
	m_gui.get<Checkbox>("LeftCollision").setChecked(currentTile.containsCollisionFlag(Tile::CollisionType::Left));
	m_gui.get<Checkbox>("RightCollision").setChecked(currentTile.containsCollisionFlag(Tile::CollisionType::Right));
	m_gui.get<Checkbox>("TopCollision").setChecked(currentTile.containsCollisionFlag(Tile::CollisionType::Top));
	m_gui.get<Checkbox>("BottomCollision").setChecked(currentTile.containsCollisionFlag(Tile::CollisionType::Bottom));
}

void TilesetEditor::TilesetEditorWindow::updateKeyboardInput()
{
	if(!m_viewMovement->getViewportBounds().contains(InputHandler::getMousePositionF(*m_window))) return;

	if (InputHandler::isPressed(sf::Keyboard::Q, *m_window))
	{
		m_viewMovement->resetViewPosition();
	}
	else if (InputHandler::isPressed(sf::Keyboard::W, *m_window))
	{
		m_viewMovement->resetZoom();
	}
}

void TilesetEditor::TilesetEditorWindow::resetTextBoxValues()
{
	m_gui.get<TextInput>("TilesetWidth").setText(std::to_string(m_editorInstance->m_tileDimensions.x));
	m_gui.get<TextInput>("TilesetHeight").setText(std::to_string(m_editorInstance->m_tileDimensions.y));
	m_gui.get<TextInput>("TileWidth").setText(std::to_string(m_editorInstance->m_tileSize.x));
	m_gui.get<TextInput>("TileHeight").setText(std::to_string(m_editorInstance->m_tileSize.y));
}

void TilesetEditor::TilesetEditorWindow::renderCollisionBoxes()
{
	for (size_t i = 0; i < m_editorInstance->m_tileData.size(); i++)
	{
		for (size_t j = 0; j < m_editorInstance->m_tileData[i].size(); j++)
		{
			const TileData& currentTile = m_editorInstance->m_tileData[i][j];

			sf::Vector2f tilePosition = static_cast<sf::Vector2f>(sf::Vector2i(i, j)) * m_editorInstance->m_tileSizeOnView;

			bool left = currentTile.containsCollisionFlag(Tile::CollisionType::Left);
			bool right = currentTile.containsCollisionFlag(Tile::CollisionType::Right);
			bool top = currentTile.containsCollisionFlag(Tile::CollisionType::Top);
			bool bottom = currentTile.containsCollisionFlag(Tile::CollisionType::Bottom);

			sf::VertexArray collisionBox;
			collisionBox.setPrimitiveType(sf::Triangles);

			sf::Vector2f topLeft = tilePosition;
			sf::Vector2f topRight = { topLeft.x + m_editorInstance->m_tileSizeOnView.x, topLeft.y };
			sf::Vector2f bottomLeft = { topLeft.x, topLeft.y + m_editorInstance->m_tileSizeOnView.y };
			sf::Vector2f bottomRight = { topRight.x, bottomLeft.y };

			sf::Color vertColor = sf::Color(0, 255, 0, 40);
			sf::Vertex centerVert = sf::Vertex(topLeft + m_editorInstance->m_tileSizeOnView * 0.5f, vertColor);

				
			if (left)
			{
				collisionBox.append(centerVert);
				collisionBox.append(sf::Vertex(topLeft, vertColor));
				collisionBox.append(sf::Vertex(bottomLeft, vertColor));
			}

			if (bottom)
			{
				collisionBox.append(centerVert);
				collisionBox.append(sf::Vertex(bottomLeft, vertColor));
				collisionBox.append(sf::Vertex(bottomRight, vertColor));
			}

			if (right)
			{
				collisionBox.append(centerVert);
				collisionBox.append(sf::Vertex(bottomRight, vertColor));
				collisionBox.append(sf::Vertex(topRight, vertColor));
			}

			if (top)
			{
				collisionBox.append(centerVert);
				collisionBox.append(sf::Vertex(topRight, vertColor));
				collisionBox.append(sf::Vertex(topLeft, vertColor));
			}

			m_window->draw(collisionBox);
		}
	}
}

std::istream& operator>>(std::istream& in, TilesetEditor& editor)
{
	LayeredData data;

	in >> data;
	in >> data;

	editor.setTileDimension(data.get<sf::Vector2i>("TilesetDimensions")); 
	editor.setTileSize(data.get<sf::Vector2i>("TileSize")); 

	for (size_t i = 0; i < editor.m_tileData.size(); i++)
	{
		for (size_t j = 0; j < editor.m_tileData[i].size(); j++)
		{
			TilesetEditor::TileData& currentTile = editor.m_tileData[i][j];

			in >> data;
			in >> data;

			currentTile.m_tileType = data.get<short>("TileType");
			currentTile.resetCollisionFlags(static_cast<Tile::CollisionType>(data.get<int>("Collision")));
		}
	}

	return in;
}

std::ostream& operator<<(std::ostream& os, const TilesetEditor& editor)
{
	os << "TilesetDimensions " << editor.m_tileDimensions << '\n';
	os << "TileSize " << editor.m_tileSize << '\n';

	for (size_t i = 0; i < editor.m_tileData.size(); i++)
	{
		for (size_t j = 0; j < editor.m_tileData[i].size(); j++)
		{
			const TilesetEditor::TileData& currentTile = editor.m_tileData[i][j];

			os << "TileType " << currentTile.m_tileType << '\n';
			os << "Collision " << static_cast<int>(currentTile.getCollision()) << '\n';
		}
	}

	return os;
}