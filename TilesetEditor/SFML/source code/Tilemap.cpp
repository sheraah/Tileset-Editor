#include "stdafx.h"
#include "Tilemap.h"
#include "ResizableSprite.h"
#include "WindowManager.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "Settings.h"
#include "Line.h"
#include "LayeredData.h"
#include "ViewMovementComponent.h"

Tilemap::Tilemap(WindowManager& window)
	:m_currentMouseLayer(0), m_firstViewSize(795.f, 600.f),
	m_backgroundTexture(&TextureManager::getTexture("mapBg")),
	m_depthDrawValue(-1), m_window(&window)
{
	m_viewMovement = std::make_unique<ViewMovementComponent>(*m_window, m_view);
	m_viewMovement->setMinZoom(0.1f);
	m_viewMovement->setMaxZoom(4.f);
	m_viewMovement->setZoomSpeed(0.1f);

	setMapSize(getMapSize());
	loadMapDataFromSave("resources/saves/MapData.txt");

	m_view.setSize(m_window->getOriginalResolutionF());
	m_viewMovement->resetViewPosition();

	m_currentMouseLayer = 0;

	setViewSize(m_firstViewSize);
	setTilemapSize(getTilemapSize());

	resetBrushOutlines();
}

Tilemap::~Tilemap()
{
	saveMapDataToSave("resources/saves/MapData.txt");
}

void Tilemap::updateEvents(const sf::Event& evnt)
{
	sf::Vector2f mousePosition = InputHandler::getMousePositionF(*m_window);
	//update the viewMovement events such as scrolling only if the mouse is inside the view bounds
	if (!m_viewMovement->getViewportBounds().contains(mousePosition)) return;

	m_viewMovement->updateEvents(evnt);
}

void Tilemap::update(float dt)
{
	updateMouseInput(dt);
	updateKeyboardInput();
}

void Tilemap::render()
{
	m_window->getWindow().setView(m_view);

	renderTilemap(*m_viewMovement);

	renderMouseTile();

	m_window->resetView();
}

void Tilemap::setViewSize(sf::Vector2f size)
{
	m_viewSize = size;

	resetViewport();
}

void Tilemap::setTilemapSize(sf::Vector2f size)
{
	Settings::set("TilemapSize", size);
	calculateGridSize();
}

void Tilemap::setBackgroundTexture(sf::Texture& texture)
{
	m_backgroundTexture = &texture;
}

void Tilemap::resize(sf::Vector2i newSize)
{
	m_map.resize(newSize.x);

	for (size_t i = 0; i < getWidth(); i++)
	{
		m_map[i].resize(newSize.y);

		for (size_t y = 0; y < getHeight(); y++)
		{
			m_map[i][y].resize(getMapDepth());
		}
	}

	//if there is no layer added yet add one layer
	if (getMapDepth() == 0) addLayer();

	calculateGridSize();
}

void Tilemap::addLayer()
{
	for (size_t x = 0; x < getWidth(); x++)
	{
		for (size_t y = 0; y < getHeight(); y++)
		{
			//adds a new default constructed tile to each tile
			m_map[x][y].emplace_back();
		}
	}
}

void Tilemap::removeLayer()
{
	//if the map deapth is 1 we dont
	//want to remove the layer since we want to have at least 1 main layer
	if (getMapDepth() == 1) return;

	for (size_t x = 0; x < getWidth(); x++)
	{
		for (size_t y = 0; y < getHeight(); y++)
		{
			//pops the top tile of each tile 
			m_map[x][y].pop_back();
		}
	}
}

void Tilemap::clear()
{
	for (size_t x = 0; x < getWidth(); x++)
	{
		for (size_t y = 0; y < getHeight(); y++)
		{
			for (size_t z = 0; z < getMapDepth(); z++)
			{
				m_map[x][y][z].reset();
			}
		}
	}
}

void Tilemap::setTile(sf::Vector3i index, int tileType, const std::string& textureName, sf::IntRect textureRect, Tile::CollisionType collision)
{
	m_map[index.x][index.y][index.z] = Tile(tileType, textureName, textureRect, collision);
}

void Tilemap::setTile(sf::Vector3i index, const Tile& tile)
{
	m_map[index.x][index.y][index.z] = tile;
}

void Tilemap::setCurrentDrawTile(int tileType, const std::string& textureName, sf::IntRect textureRect, Tile::CollisionType collision)
{
	m_currentMouseTile = Tile(tileType, textureName, textureRect, collision);
}

void Tilemap::setCurrentDrawTile(const Tile& tile)
{
	m_currentMouseTile = tile;
}

sf::Vector2f Tilemap::getTilemapSize()
{
	return Settings::get<sf::Vector2f>("TilemapSize");
}

void Tilemap::setCameraSpeed(float newSpeed)
{
	Settings::set("CameraSpeed", newSpeed);

	m_viewMovement->setViewMoveSpeed(newSpeed);
}

float Tilemap::getCameraSpeed()
{
	return Settings::get<float>("CameraSpeed");
}

void Tilemap::setSingleTileDrawMode(bool drawMode)
{
	Settings::set("SingleTileDrawMode", drawMode);
}

bool Tilemap::getSingleTileDrawMode()
{
	return Settings::get<bool>("SingleTileDrawMode");
}

void Tilemap::setDrawGrid(bool newDrawGrid)
{
	Settings::set("DrawGrid", newDrawGrid);
}

bool Tilemap::getDrawGrid()
{
	return Settings::get<bool>("DrawGrid");
}

void Tilemap::setBrushSize(int brushSize)
{
	Settings::set("BrushSize", brushSize);

	resetBrushOutlines();
}

int Tilemap::getBrushSize()
{
	return Settings::get<int>("BrushSize");
}

void Tilemap::setOverrideTile(bool newOverrideMode)
{
	Settings::set("OverrideTile", newOverrideMode);
}

bool Tilemap::getOverrideTile()
{
	return Settings::get<bool>("OverrideTile");
}

size_t Tilemap::getMapDepth() const
{
	try
	{
		return m_map.at(0).at(0).size();
	}
	catch (const std::out_of_range& e)
	{
		std::cout << e.what();
		return 0;
	}
}

void Tilemap::setDepthDrawValue(short depthDrawValue)
{
	if (depthDrawValue < -1 || (depthDrawValue != -1 && depthDrawValue >= getMapDepth())) return;

	m_depthDrawValue = depthDrawValue;
}

sf::FloatRect Tilemap::getTilemapBounds()
{
	return sf::FloatRect({ 0.f, 0.f }, getTilemapSize());
}

sf::Vector2f Tilemap::getTilemapSizeScale()
{
	return getTilemapSize() / m_window->getOriginalResolutionF();
}

void Tilemap::saveTilemapPNG(const std::string& filePath)
{
	//create a render texture with the size of the tilemap resolution
	sf::RenderTexture tilemap;
	tilemap.create(static_cast<unsigned int>(getTilemapSize().x), static_cast<unsigned int>(getTilemapSize().y));
	//draw the background
	ResizableSprite background;
	background.setTexture(*m_backgroundTexture);
	background.setSize(getTilemapSize());

	tilemap.draw(background);
	//draw all tiles
	for (size_t x = 0; x < getWidth(); x++)
	{
		for (size_t y = 0; y < getHeight(); y++)
		{
			for (size_t z = 0; z < getMapDepth(); z++)
			{
				sf::Vector2f tilePosition = { x * m_gridSize.x, y * m_gridSize.y };
				m_map[x][y][z].render(tilePosition, m_gridSize, tilemap);
			}
		}
	}

	tilemap.display();

	sf::Image tilemapImage = tilemap.getTexture().copyToImage();

	tilemapImage.saveToFile(filePath);
}

void Tilemap::saveTilemapDataToFolder(const std::string& filePath)
{
	saveTilemapPNG(filePath + "Map.png");

	std::ofstream in(filePath + "Settings.txt");

	in << "MapSize " << getMapSize() << "\n";
	in << "MapResolution " << getTilemapSize() << "\n";

	in.close();

	saveMapDataToSave(filePath + "MapData.txt");
}

void Tilemap::loadTilemapDataFromFolder(const std::string& filePath)
{
	std::ifstream in(filePath + "Settings.txt");

	LayeredData data;

	in >> data;
	in >> data;

	setMapSize(data.get<sf::Vector2i>("MapSize"));
	setTilemapSize(data.get<sf::Vector2f>("MapResolution"));

	in.close();

	loadMapDataFromSave(filePath + "MapData.txt");
}

void Tilemap::setMapSize(sf::Vector2i newMapSize)
{
	if (newMapSize.x <= 0 || newMapSize.y <= 0) return;

	Settings::set("MapSize", newMapSize);

	resize(newMapSize);
}

sf::Vector2i Tilemap::getMapSize()
{
	return Settings::get<sf::Vector2i>("MapSize");
}

void Tilemap::setMapDepth(size_t newDepth)
{
	if (getMapDepth() == newDepth || newDepth == 0) return;

	for (size_t x = 0; x < getWidth(); x++)
	{
		for (size_t y = 0; y < getHeight(); y++)
		{
			m_map[x][y].resize(newDepth, Tile());
		}
	}
}

void Tilemap::deleteMouseTiles()
{
	sf::Vector2i mousePos = getCurrentMouseGridPosition();

	if (mousePos == -1) return;

	if (!m_isDeletingTile)
	{
		m_undoManager.addNewUndoFrame();
		m_isDeletingTile = true;
		m_previousMosueGridIndex = mousePos;
	}
	//get every tile between the previous mouseGridindex and the current mousegrindex
	forEachPointOnLine(m_previousMosueGridIndex, mousePos, [this](sf::Vector2i point)
	{
		//treat the point as a center point for the delete brush and delete all tiles that are inside the brush
		forEachPointInBrush(sf::Vector3i(point.x, point.y, m_currentMouseLayer), [this](const sf::Vector3i& mapIndex)
		{
			Tile& tile = m_map[mapIndex.x][mapIndex.y][mapIndex.z];

			if (!tile.isEmpty())
			{	
				m_undoManager.addUndoData(mapIndex, tile);
				tile.reset();
			}
		});
	});

	m_previousMosueGridIndex = mousePos;
}

void Tilemap::renderGrids()
{
	sf::Vector2i mouseGridPosition = getCurrentMouseGridPosition();

	if (mouseGridPosition == -1) return;

	sf::Vector2f gridPosition = sf::Vector2f(mouseGridPosition) * m_gridSize;

	sf::Vector2f BottomRight = getTilemapSize();
	float LineThickness = m_viewMovement->getZoomFactor() * 2.f;

	m_window->draw(Line({ 0.f, gridPosition.y }, { BottomRight.x, gridPosition.y }, LineThickness, sf::Color::Black));
	m_window->draw(Line({ 0.f, gridPosition.y + m_gridSize.y }, { BottomRight.x, gridPosition.y + m_gridSize.y }, LineThickness, sf::Color::Black));

	m_window->draw(Line({ gridPosition.x, 0.f }, { gridPosition.x, BottomRight.y }, LineThickness, sf::Color::Black));
	m_window->draw(Line({ gridPosition.x + m_gridSize.x, 0.f }, { gridPosition.x + m_gridSize.x, BottomRight.y }, LineThickness, sf::Color::Black));
}

void Tilemap::undoTileDrawing()
{
	m_undoManager.undo([this](sf::Vector3i mapIndex, const Tile& tile)
	{
		setTile(mapIndex, tile);
	});
}

void Tilemap::loadMapDataFromSave(const std::string& filePath)
{
	std::ifstream in(filePath);

	LayeredData data;

	in >> data;
	//read in the map depth 
	setMapDepth(data.get<int>("mapDepth"));
	//read in all tiles
	for (size_t x = 0; x < getWidth(); x++)
	{
		for (size_t y = 0; y < getHeight(); y++)
		{
			for (size_t z = 0; z < getMapDepth(); z++)
			{
				in >> m_map[x][y][z];
			}
		}
	}

	in.close();
}


void Tilemap::saveMapDataToSave(const std::string& filePath)
{
	std::ofstream out(filePath);
	//wrtie mapDepth
	out << "mapDepth " << getMapDepth() << '\n';
	//write all tiles 
	for (size_t x = 0; x < getWidth(); x++)
	{
		for (size_t y = 0; y < getHeight(); y++)
		{
			for (size_t z = 0; z < getMapDepth(); z++)
			{
				out << m_map[x][y][z];
			}
		}
	}

	out.close();
}

bool Tilemap::tilesUnderAreEmpty(const sf::Vector3i& mapIndex)
{
	if (mapIndex.z == 0 || mapIndex.z >= getMapDepth()) return false;

	for (size_t i = 0; i < mapIndex.z; i++)
	{
		if (m_map[mapIndex.x][mapIndex.y][i].isEmpty()) return true;
	}

	return false;
}

bool Tilemap::tilesAboveAreEmpty(const sf::Vector3i& mapIndex)
{
	if (mapIndex.z <= 0 || mapIndex.z >= getMapDepth()) return false;

	for (size_t i = 0; i < mapIndex.z; i++)
	{
		if (m_map[mapIndex.x][mapIndex.y][mapIndex.z].isEmpty()) return true;
	}

	return false;
}

bool Tilemap::isValidTileDrawIndex(const sf::Vector3i& mapIndex)
{
	bool isNotValid = !getOverrideTile() && !m_map[mapIndex.x][mapIndex.y][mapIndex.z].isEmpty() || tilesUnderAreEmpty(mapIndex);

	return !isNotValid;
}

void Tilemap::forEachPointInBrush(const sf::Vector3i& centerMapIndex, std::function<void(const sf::Vector3i& mapIndex)> function)
{
    int brushSize = getBrushSize();
    float brushRadius = brushSize / 2.f;
	//checks if the index given in this function is inside a circle with the radius of centermapindex
    auto isPointInCircle = [&centerMapIndex, this, brushRadius, brushSize](const sf::Vector3i& index)
    {
        sf::Vector2f centerPoint = sf::Vector2f(centerMapIndex.x , centerMapIndex.y );
		sf::Vector2f indexPoint = sf::Vector2f(index.x, index.y);

		if (brushSize % 2 == 0)
		{
			centerPoint.x -= 0.5f;
			centerPoint.y -= 0.5f;
		}

		return sf::Vector2f(indexPoint - centerPoint).length() <= brushRadius;
    };

	//iterating over every point on a square of length brushSize and checking if the point is inside the circle
    for (int y = 0; y < brushSize; y++)
    {
        for (int x = 0; x < brushSize; x++)
        {
            sf::Vector3i mapIndex = {
                centerMapIndex.x - static_cast<int>(brushRadius) + x,
                centerMapIndex.y - static_cast<int>(brushRadius) + y,
                centerMapIndex.z
            };

            if (!isValidMapIndex({ mapIndex.x, mapIndex.y }) || !isPointInCircle(mapIndex)) continue;
			//if the point is inside the circle and a valid map index execute the lambda passed into the function
            function(mapIndex);
        }
    }
}

void Tilemap::renderPreviewMouseTiles()
{
	sf::Vector2i currentMouseGrid = getCurrentMouseGridPosition();

	if (currentMouseGrid == -1) return;

	forEachPointInBrush({ currentMouseGrid.x, currentMouseGrid.y, m_currentMouseLayer }, [this](const sf::Vector3i& mapIndex)
	{
		if (!isValidTileDrawIndex(mapIndex)) return;

		sf::Vector2f mouseTilePosition = sf::Vector2f(mapIndex.x, mapIndex.y) * m_gridSize;

		m_currentMouseTile.render(mouseTilePosition, m_gridSize, m_window->getWindow());
	});
}

void Tilemap::renderBrushOutlines()
{
	sf::Vector2i mousePos = getCurrentMouseGridPosition();

	if (mousePos == -1) return;

	sf::Vector2i topLeft =mousePos - getBrushSize() / 2;

	m_brushOutlineShape.setPosition(static_cast<sf::Vector2f>(topLeft) * m_gridSize);
	
	m_window->draw(m_brushOutlineShape);
}


size_t Tilemap::getWidth() const
{
	return m_map.size();
}

size_t Tilemap::getHeight() const
{
	try
	{
		return m_map[0].size();
	}
	catch (const std::out_of_range& e)
	{
		std::cout << e.what();
		return 0;
	}
}

void Tilemap::resetBrushOutlines()
{
	m_brushOutlineShape.setOutlineThickness(5.F);
	m_brushOutlineShape.setOutlineColor(sf::Color::Black);
	m_brushOutlineShape.setFillColor(sf::Color::Transparent);

	std::vector<sf::Vector2i> leftVerts;
	std::vector<sf::Vector2i> rightVerts;

	
	sf::Vector2i prevPoint = { -1, -1 };
	//this algorithms sotres all vertices of all points of the brush outline and then draws lines from each of the points
	forEachPointInBrush(sf::Vector3i(getBrushSize() / 2, getBrushSize() / 2, m_currentMouseLayer),
	[this, &prevPoint, &leftVerts, &rightVerts](const sf::Vector3i& point)
	{
		if (prevPoint.y < point.y)
		{
			if (!leftVerts.empty() && leftVerts.back().x != point.x)
			{
				leftVerts.push_back({ leftVerts.back().x, point.y });
			}

			leftVerts.push_back({ point.x, point.y });
			leftVerts.push_back({ point.x, point.y + 1 });

			if (prevPoint != -1)
			{
				if (!rightVerts.empty() && rightVerts.back().x != prevPoint.x + 1)
				{
					rightVerts.push_back({ rightVerts.back().x, prevPoint.y });
				}

				rightVerts.push_back({ prevPoint.x + 1, prevPoint.y });
				rightVerts.push_back({ prevPoint.x + 1, prevPoint.y + 1});
			}
		}


		prevPoint = sf::Vector2i(point.x, point.y);
	});

	rightVerts.push_back({ prevPoint.x + 1, prevPoint.y });
	rightVerts.push_back({ prevPoint.x + 1, prevPoint.y + 1 });

	m_brushOutlineShape.setPointCount(leftVerts.size() + rightVerts.size());

	for (size_t i = 0; i < leftVerts.size(); i++)
	{
		m_brushOutlineShape.setPoint(i, static_cast<sf::Vector2f>(leftVerts[i]) * m_gridSize);
	}

	for (size_t i = 0; i < rightVerts.size(); i++)
	{
		m_brushOutlineShape.setPoint(leftVerts.size() + i, static_cast<sf::Vector2f>(rightVerts[rightVerts.size() - i - 1]) * m_gridSize);
	}
}

bool Tilemap::isValidMapIndex(sf::Vector2i index) const
{
	return index.x >= 0 && index.x < getWidth() && index.y >= 0 && index.y < getHeight();
}

void Tilemap::renderTilemap(ViewMovementComponent& viewMovement)
{
	renderTilemapBackground(viewMovement);
	renderTiles(viewMovement);

	if (getDrawGrid()) renderGrids();
}

void Tilemap::renderTiles(ViewMovementComponent& viewMovement)
{
	for (size_t x = 0; x < getWidth(); x++)
	{
		for (size_t y = 0; y < getHeight(); y++)
		{
			sf::Vector2f tilePosition = { x * m_gridSize.x, y * m_gridSize.y };

			if (m_depthDrawValue == -1)
			{
				for (size_t z = 0; z < getMapDepth(); z++)
				{
					m_map[x][y][z].render(tilePosition, m_gridSize, viewMovement.getWindow()->getWindow());
				}
			}
			else
			{
				m_map[x][y][m_depthDrawValue].render(tilePosition, m_gridSize, viewMovement.getWindow()->getWindow());
			}
		}
	}
}

void Tilemap::renderMouseTile()
{
	if (!m_isDeletingTile)
	{
		renderPreviewMouseTiles();
	}
	else
	{
		renderBrushOutlines();
	}
}

void Tilemap::renderTilemapBackground(ViewMovementComponent& viewMovement)
{
	renderBlackBackground(viewMovement);

	ResizableSprite background;
	background.setTexture(*m_backgroundTexture);
	background.setSize(getTilemapSize());

	viewMovement.getWindow()->draw(background);
}

void Tilemap::renderBlackBackground(ViewMovementComponent& viewMovement)
{
	float oldZoomFactor = viewMovement.getZoomFactor();
	sf::Vector2f oldViewCenter = viewMovement.getView()->getCenter();

	sf::RectangleShape blackBlackground(viewMovement.getWindow()->getOriginalResolutionF());
	blackBlackground.setFillColor(sf::Color::Black);

	viewMovement.resetZoom();
	viewMovement.resetViewPosition();
	viewMovement.getWindow()->getWindow().setView(*viewMovement.getView());

	viewMovement.getWindow()->draw(blackBlackground);

	viewMovement.setZoomFactor(oldZoomFactor);
	viewMovement.getView()->setCenter(oldViewCenter);
	viewMovement.getWindow()->getWindow().setView(*viewMovement.getView());
}

sf::Vector2i Tilemap::getCurrentMouseGridPosition() const
{
	sf::Vector2f mousePositionWindow = InputHandler::getMousePositionF(*m_window);

	if (!m_viewMovement->getViewportBounds().contains(mousePositionWindow)) return sf::Vector2i(-1, -1); // if the mouse is outside the view bounds then return

	sf::Vector2f mousePositionView = m_window->screenToWorld(InputHandler::getMousePositionF(*m_window), m_view);

	mousePositionView /= m_gridSize;

	sf::Vector2i currentMouseGrid = static_cast<sf::Vector2i>(mousePositionView);

	if (!isValidMapIndex(currentMouseGrid)) return sf::Vector2i(-1, -1);

	return currentMouseGrid;
}

void Tilemap::paintAtIndex(sf::Vector2i mouseIndex)
{
	forEachPointOnLine(m_previousMosueGridIndex, mouseIndex, [this, &mouseIndex](sf::Vector2i point)
	{
		forEachPointInBrush(sf::Vector3i(point.x, point.y, m_currentMouseLayer), [this](const sf::Vector3i& mapIndex)
		{
			if(!isValidTileDrawIndex(mapIndex)) return;

			m_undoManager.addUndoData(mapIndex, m_map[mapIndex.x][mapIndex.y][mapIndex.z]);
			setTile(mapIndex, m_currentMouseTile);
		});
	});

	m_previousMosueGridIndex = mouseIndex;
}

int Tilemap::sign(int value)
{
	return (value > 0) - (value < 0);
}

void Tilemap::forEachPointOnLine(sf::Vector2i point1, sf::Vector2i point2, std::function<void(sf::Vector2i point)> function)
{
	sf::Vector2i distance{ 
		abs(point2.x - point1.x),
		abs(point2.y - point1.y) 
	};
	sf::Vector2i signOfPoints {
		sign(point2.x - point1.x),
		sign(point2.y - point1.y)
	};

	sf::Vector2i point = point1;
	int swapFlag = 0;

	if (distance.y > distance.x) 
	{
		std::swap(distance.x, distance.y);
		swapFlag = 1;
	}

	int p = 2 * distance.y - distance.x;

	for (int i = 0; i < distance.x; i++) 
	{
		function(point);

		if (p >= 0) 
		{
			if (swapFlag) 
			{
				point.x += signOfPoints.x;
			}
			else 
			{
				point.y += signOfPoints.y;
			}
			p -= 2 * distance.x;
		}

		if (swapFlag) 
		{
			point.y += signOfPoints.y;
		}
		else 
		{
			point.x += signOfPoints.x;
		}

		p += 2 * distance.y;
	}

	function(point2);
}

void Tilemap::updateMouseInput(float dt)
{
	updateLeftMousebuttonInput();

	m_viewMovement->update(dt);
}

void Tilemap::updateKeyboardInput()
{
	if (InputHandler::isPressed(sf::Keyboard::F, *m_window, InputHandler::InputMode::Once))
	{
		toggleFullscreen();
	}

	else if (InputHandler::isPressed(sf::Keyboard::Q, *m_window, InputHandler::InputMode::Once)) // reset tilemap position to start 0.f, 0.f
	{
		m_viewMovement->resetViewPosition();
	}

	else if (InputHandler::isPressed(sf::Keyboard::W, *m_window, InputHandler::InputMode::Once))
	{
		m_viewMovement->resetZoom();
	}

	else if (InputHandler::isPressed(sf::Keyboard::E, *m_window, InputHandler::InputMode::Once))
	{
		setDrawGrid(!getDrawGrid());
	}

	else if (InputHandler::isPressed({ sf::Keyboard::LControl, sf::Keyboard::Z }, *m_window, InputHandler::InputMode::Once))
	{
		undoTileDrawing();
	}

	bool spacePressed = InputHandler::isPressed(sf::Keyboard::Space, *m_window);

	if (m_isPlacingTile || !spacePressed)
	{
		m_isDeletingTile = false;
		return;
	}

	deleteMouseTiles();

}

void Tilemap::updateLeftMousebuttonInput()
{
	sf::Vector2i currentMouseGrid = getCurrentMouseGridPosition();

	if (currentMouseGrid == -1) return;

	InputHandler::InputMode inputFlag = getSingleTileDrawMode() ? InputHandler::InputMode::Once : InputHandler::InputMode::Loop;

	if (!InputHandler::isPressed(sf::Mouse::Left, *m_window, inputFlag))
	{
		m_isPlacingTile = false;
		return;
	}

	if (!m_isPlacingTile)
	{
		m_undoManager.addNewUndoFrame();
		m_isPlacingTile = true;
		m_previousMosueGridIndex = currentMouseGrid;
	}

	paintAtIndex(currentMouseGrid);
}

void Tilemap::resetViewport()
{
	sf::Vector2f sizeViewport = m_viewSize / m_window->getOriginalResolutionF();

	m_view.setViewport({ { 0.f, 0.f }, sizeViewport });
}

void Tilemap::toggleFullscreen()
{
	m_isInFullscreen = !m_isInFullscreen;

	if (m_isInFullscreen)
	{
		setViewSize(m_window->getOriginalResolutionF());
	}
	else
	{
		setViewSize(m_firstViewSize);
	}
}

void Tilemap::calculateGridSize()
{
	m_gridSize.x = getTilemapSize().x / getWidth();
	m_gridSize.y = getTilemapSize().y / getHeight();
}

void Tilemap::UndoManager::addUndoData(sf::Vector3i mapIndex, Tile tile)
{
	if (m_undoTileData.empty()) return;

	undoDataType& undoMap = m_undoTileData.top();

	if (undoMap.find(mapIndex) != undoMap.end()) return; //there is already an element with this key return

	undoMap[mapIndex] = std::move(tile);
}

void Tilemap::UndoManager::undo(std::function<void(sf::Vector3i mapIndex, const Tile& tile)> func)
{
	if (m_undoTileData.empty()) return;

	for (const auto& [mapIndex, tile] : m_undoTileData.top()) //itration over the map and execution the lambda function before removing the undo frame
	{
		func(mapIndex, tile);
	}

	m_undoTileData.pop();
}

void Tilemap::UndoManager::addNewUndoFrame()
{
	if (!m_undoTileData.empty() && m_undoTileData.top().empty()) return;

	m_undoTileData.emplace();
}

size_t Tilemap::UndoManager::Vector3iHash::operator()(const sf::Vector3i& vector) const
{
	size_t hash = 0;
	hash_combine(hash, vector.x);
	hash_combine(hash, vector.y);
	hash_combine(hash, vector.z);
	return hash;
}

void Tilemap::UndoManager::Vector3iHash::hash_combine(size_t& seed, const size_t value) const
{
	seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}