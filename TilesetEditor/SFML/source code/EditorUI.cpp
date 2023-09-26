#include "stdafx.h"
#include "EditorUI.h"
#include "Tilemap.h"
#include "WindowManager.h"
#include "FileDialog.h"
#include "TextInput.h"
#include "Utils.h"
#include <fstream>
#include "CustomPointEditor.h"

EditorUI::EditorUI(Tilemap& map, StateMachine& stateMachineInstance)
	: m_stateMachine(&stateMachineInstance), m_tilemap(&map), m_currentTilesetEditor(nullptr) 
{
	m_customPointEditor = std::make_unique<CustomPointEditor>(map);

	initGUI();

	loadTilesetEditors("resources/saves/TilesetData.txt");
}

EditorUI::~EditorUI()
{
	saveTilesetDataToFile("resources/saves/TilesetData.txt");
}

void EditorUI::updateEvents(const sf::Event& evnt)
{
	m_gui.updateEvents(evnt);
}

void EditorUI::update(float dt)
{
	if (m_tilemap->m_isInFullscreen) return; // if the the map is in fullscreen dont update the whole UI-
	m_customPointEditor->update(dt);

	m_gui.update();

	m_tilemap->setDepthDrawValue(m_gui.get<Combo<int>>("layersToShow").getActiveValue());
	m_tilemap->m_currentMouseLayer = m_gui.get<Combo<int>>("layerToDraw").getActiveValue();
	
	
	updateButtons();
	updateTilesetEditor(dt);
}

void EditorUI::render()
{
	if (m_tilemap->m_isInFullscreen) return; // is the map is in fullscreen we dont want to render the UI

	m_customPointEditor->render();

	renderBackground();	
	renderTilesetEditor();
	m_gui.render();
	renderTexts();
}

void EditorUI::renderBackground()
{
	ResizableSprite background;

	background.setTexture(TextureManager::getTexture("EditorBg"));
	background.setSize(m_stateMachine->getWindow()->getOriginalResolutionF());

	m_stateMachine->getWindow()->draw(background);
}

void EditorUI::renderTexts()
{
	Utils::setupText(m_text, "Show Layers", 25, { 920.f, 110.f }, sf::Color(200, 200, 200), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "Draw At Layer", 25, m_text.getPosition() + sf::Vector2f(185.f, 0.f), sf::Color(200, 200, 200), {0.5f, 0.5f});
	m_stateMachine->getWindow()->draw(m_text);

	Utils::setupText(m_text, "Brush Size", 30, { 1350.f, 360.f }, sf::Color(200, 200, 200), { 0.5f, 0.5f });
	m_stateMachine->getWindow()->draw(m_text);
}

void EditorUI::updateButtons()
{
	if (m_gui.get<Button>("loadBg").isPressedOnce()) // if the loadBg button is pressed create a filedialog for loading the bg
	{
		loadBackground();
	}

	else if (m_gui.get<Button>("saveMap").isPressedOnce()) // if the saveMapPng button is pressed create filedialog for saving the map PNG using the tilemap reference
	{
		saveMap();
	}

	else if (m_gui.get<Button>("loadMap").isPressedOnce())
	{
		loadMap();
	}

	else if (m_gui.get<Button>("loadTileset").isPressedOnce()) // load a tileset from file using filedialog
	{
		loadTilesetFromFile();
	}

	else if (m_gui.get<Button>("settings").isPressedOnce()) //push settings state
	{
		m_currentTilesetEditor->m_tilesetEditor->closeWindow();
		m_customPointEditor->close();
		m_stateMachine->pushState(new SettingsState(*m_stateMachine, *m_tilemap));
	}
	
	

	else if (m_gui.get<Button>("addLayer").isPressedOnce() && m_tilemap->getMapDepth() < 10) //add new layer 
	{
		size_t layerIndex = m_tilemap->getMapDepth();
		m_tilemap->addLayer();
		m_gui.get<Combo<int>>("layerToDraw").addElement("Layer" + std::to_string(layerIndex), layerIndex);
		m_gui.get<Combo<int>>("layersToShow").addElement("Layer" + std::to_string(layerIndex), layerIndex);
	}

	else if (m_gui.get<Button>("removeLayer").isPressedOnce() && m_tilemap->getMapDepth() > 1) //remove the last layer 
	{
		size_t layerIndex = m_tilemap->getMapDepth() - 1;
		m_tilemap->setMapDepth(layerIndex);
		m_gui.get<Combo<int>>("layerToDraw").removeElement("Layer" + std::to_string(layerIndex));
		m_gui.get<Combo<int>>("layersToShow").removeElement("Layer" + std::to_string(layerIndex));
	}

	else if (m_gui.get<Button>("clearMap").isPressedOnce()) //clear all tiles
	{
		m_tilemap->clear();
	}

	else if (m_gui.get<Button>("customPoints").isPressedOnce()) //open the custom point editor
	{
		m_customPointEditor->open();
	}

	else if (m_gui.get<TextInput>("brushSize").textUpdated())
	{
		int newBrushSize = Utils::StrToValue<int>(m_gui.get<TextInput>("brushSize").getText());

		if (newBrushSize > 0 && newBrushSize < 30)
		{
			m_tilemap->setBrushSize(newBrushSize);
		}
	}
}

void EditorUI::loadBackground()
{
	FileDialog dialog;

	dialog.addFilter("Images", "*.png;*.jpg");
	std::string filePath = dialog.openDialog();

	if (!filePath.empty()) TextureManager::loadTexture("mapBg", filePath);
}

void EditorUI::initGUI()
{
	m_text.setFont(FontManager::getFont("woodUI"));

	addButton("saveMap", "Save Map", { 20.f, 650.f }, { 200.f, 40.f });
	addButton("loadMap", "Load Map", { 20.f, 710 }, { 200.f, 40.f });
	addButton("loadBg", "Load Background", { 20.f, 770.f }, { 200.f, 40.f });
	addButton("loadTileset", "Load Tileset", { 20.f, 830.f }, { 200.f, 40.f });
	addButton("addLayer", "Add New Layer", { 840.f, 50.f }, { 150.f, 40.f });
	addButton("removeLayer", "Remove Layer", { 1040.f, 50.f }, { 150.f, 40.f });
	addButton("customPoints", "Add Custom Points", { 1250.f, 240.f }, { 260.f, 40.f });
	addButton("clearMap", "Clear Map", { 1250.f, 130.f }, { 150.f, 40.f });
	addButton("settings", "", { 1420.f, 0.f }, { 80.f, 80.f });

	m_gui.get<Button>("settings").setTexture(TextureManager::getTexture("SettingsIcon"));

	addTextInputBox("brushSize", { 1300.f, 400.f }, { 120.f, 35.f }, "...", 2, std::to_string(m_tilemap->getBrushSize()));

	addCombo<int>("layersToShow", { 860.f, 130.f }, { 120.f, 35.f });
	addCombo<int>("layerToDraw", { 1040.f, 130.f }, { 120.f, 35.f });

	m_gui.get<Combo<int>>("layersToShow").addElement("All Layers", -1);

	for (int i = 0; i < m_tilemap->getMapDepth(); i++)
	{
		m_gui.get<Combo<int>>("layersToShow").addElement("Layer" + std::to_string(i), i);
		m_gui.get<Combo<int>>("layerToDraw").addElement("Layer" + std::to_string(i), i);
	}
}

void EditorUI::loadTilesetEditors(const std::string& filePath)
{
	std::ifstream in(filePath);
	//iterrate over all tileset editors and if the tileset is loaded then initialize it
	for (size_t i = 0; i < m_tilesetEditors.size(); i++) 
	{
		sf::Vector2f buttonPosition = sf::Vector2f(260.f, 660.f) + sf::Vector2f(180.f * (i / 2), 160.f * (i % 2));

		m_tilesetEditors[i].m_button = std::make_unique<Button>(*m_stateMachine->getWindow());
		m_tilesetEditors[i].m_button->setSize({ 150.f, 130.f });
		m_tilesetEditors[i].m_button->setPosition(buttonPosition);

		if (TextureManager::textureExists("Tileset" + std::to_string(i)))
		{
			resetTilesetEditorObject(i);
			in >> *m_tilesetEditors[i].m_tilesetEditor;
		}
	}

	in.close();
}

void EditorUI::addButton(const std::string& name, const std::string& buttonText, sf::Vector2f position, sf::Vector2f size)
{
	m_gui.add(name, Button(*m_stateMachine->getWindow()));
	m_gui.get<Button>(name).setPosition(position);
	m_gui.get<Button>(name).setSize(size);
	m_gui.get<Button>(name).setTexture(TextureManager::getTexture("BlackButton"));
	m_gui.get<Button>(name).setFont(FontManager::getFont("woodUI"));
	m_gui.get<Button>(name).setTextColor(sf::Color(200, 200, 200));
	m_gui.get<Button>(name).setText(buttonText);
}

void EditorUI::addTextInputBox(const std::string& name, sf::Vector2f position,
	sf::Vector2f size, const std::string& blankText, size_t textLimit, const std::string& text)
{
	m_gui.add(name, TextInput(*m_stateMachine->getWindow()));

	m_gui.get<TextInput>(name).getButton().setPosition(position);
	m_gui.get<TextInput>(name).getButton().setSize(size);
	m_gui.get<TextInput>(name).getButton().setFont(FontManager::getFont("woodUI"));
	m_gui.get<TextInput>(name).getButton().setTexture(TextureManager::getTexture("InputBox"));
	m_gui.get<TextInput>(name).setBlankText(blankText);
	m_gui.get<TextInput>(name).setTextLimit(textLimit);
	m_gui.get<TextInput>(name).setText(text);
}

void EditorUI::saveMap()
{
	FileDialog dialog;

	std::string filePath = dialog.openFolderDialog();

	if (filePath.empty()) return;
	
	m_tilemap->saveTilemapDataToFolder(filePath);

	std::ofstream out(filePath + "TilesetTextures.txt");

	size_t index = 0;
	while (TextureManager::textureExists("Tileset" + std::to_string(index)))
	{
		std::string textureName = "Tileset" + std::to_string(index);
		out << textureName << " " << TextureManager::getTexturePath(textureName) << "\n";
		index++;
	}

	out.close();

	saveTilesetDataToFile(filePath + "TilesetData.txt");

	m_customPointEditor->loadToSave(filePath + "CustomPoints.txt");
}

void EditorUI::loadTilesetFromFile()
{
	FileDialog fileDialog;

	fileDialog.addFilter("Images", "*.png;*.jpg");

	std::string filePath = fileDialog.openDialog();

	if (filePath.empty()) return;
	
	size_t emptyIndex = getNextAviableTilesetEditorIndex();

	if (emptyIndex >= m_tilesetEditors.size()) return;

	std::string textureName = "Tileset" + std::to_string(emptyIndex);
	TextureManager::loadTexture(textureName, filePath);

	resetTilesetEditorObject(emptyIndex);

	m_tilesetEditors[emptyIndex].m_tilesetEditor->setTileDimension({ 10, 10 }); // default value of 10x10 tileset
	
}

void EditorUI::updateTilesetEditor(float dt)
{
	for (size_t i = 0; i < m_tilesetEditors.size(); i++)
	{
		if (m_tilesetEditors[i].m_tilesetEditor.get() == nullptr) continue;

		m_tilesetEditors[i].m_tilesetEditor->updateWindow(dt);

		m_tilesetEditors[i].m_button->update();

		if (m_tilesetEditors[i].m_button->isPressedOnce())
		{
			m_currentTilesetEditor = &m_tilesetEditors[i];
		}
	}

	if (m_currentTilesetEditor == nullptr) return;

	TilesetEditor& currentEditor = *m_currentTilesetEditor->m_tilesetEditor;

	currentEditor.update();

	if (currentEditor.loadButtonPressed())
	{
		//resetting the buttons texture it gets werid when the texture it self changes 
		m_currentTilesetEditor->m_button->setTexture(m_currentTilesetEditor->m_button->getTexture()); 
	}

	else if (currentEditor.needsToBeDeleted())
	{
		//subtract from it the begin so that we get the editor index
		size_t currentEditorIndex = m_currentTilesetEditor - &m_tilesetEditors[0];

		deleteTilesetEditorInstance(currentEditorIndex);
	}

	else if (currentEditor.selectedTileChanged()) // if the user has chnaged the tile update the tilemap draw tile
	{
		m_tilemap->setCurrentDrawTile(currentEditor.getActiveTile());
	}
}

void EditorUI::renderTilesetEditor()
{
	for (size_t i = 0; i < m_tilesetEditors.size(); i++)
	{
		if (m_tilesetEditors[i].m_tilesetEditor.get() == nullptr) continue;
		m_tilesetEditors[i].m_button->render();

		m_tilesetEditors[i].m_tilesetEditor->renderWindow();
	}

	if (m_currentTilesetEditor == nullptr) return; // render the tileset from where the user can selecet tiles

	m_currentTilesetEditor->m_tilesetEditor->render();
}

void EditorUI::resetTilesetEditorObject(size_t index)
{
	std::string textureName = "Tileset" + std::to_string(index);

	m_tilesetEditors[index].m_tilesetEditor.reset(new TilesetEditor(*m_stateMachine->getWindow()));
	m_tilesetEditors[index].m_tilesetEditor->setTilesetTexture(textureName);
	m_tilesetEditors[index].m_tilesetEditor->setPosition({ 860.f, 530.f });
	m_tilesetEditors[index].m_tilesetEditor->setTilesetSize({ 500.f, 450.f });

	m_tilesetEditors[index].m_button->setTexture(TextureManager::getTexture(textureName));
	m_currentTilesetEditor = &m_tilesetEditors[index];
}

size_t EditorUI::getNextAviableTilesetEditorIndex()
{
	for (size_t i = 0; i < m_tilesetEditors.size(); i++)
	{
		if (m_tilesetEditors[i].m_tilesetEditor.get() == nullptr) return i;
	}

	return m_tilesetEditors.size();
}

void EditorUI::deleteTilesetEditorInstance(size_t index)
{
	m_currentTilesetEditor = nullptr;
	m_tilesetEditors[index].m_tilesetEditor.reset();

	TextureManager::erase("Tileset" + std::to_string(index));
}

void EditorUI::saveTilesetDataToFile(const std::string& filePath)
{
	std::ofstream out(filePath);

	for (size_t i = 0; i < m_tilesetEditors.size(); i++)
	{
		if (m_tilesetEditors[i].m_tilesetEditor.get() == nullptr) continue;

		out << *m_tilesetEditors[i].m_tilesetEditor;
	}

	out.close();
}

void EditorUI::loadMap()
{
	FileDialog dialog;

	std::string filePath = dialog.openFolderDialog();

	if (filePath.empty()) return;

	m_tilemap->loadTilemapDataFromFolder(filePath); //updates the tilemap

	std::ifstream in(filePath + "TilesetTextures.txt");

	for (size_t i = 0; i < m_tilesetEditors.size(); i++) //updates the tileseteditors
	{
		std::string textureName;
		std::string texturePath;

		if (in >> textureName >> texturePath)
		{
			TextureManager::loadTexture(textureName, texturePath);
			continue;
		}
		
		deleteTilesetEditorInstance(i);
	}

	in.close();

	loadTilesetEditors(filePath + "TilesetData.txt");

	m_customPointEditor->loadFromSave(filePath + "CustomPoints.txt"); //updates the custom points
}
