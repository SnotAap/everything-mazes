#include "Header.h"

void gridSetup(int sizeX, int sizeY, std::vector<std::shared_ptr<Object>>& renderList, std::map<cordinates, std::shared_ptr<Tile>>& tileMap, std::map<cordinates, walls>& wallMap, std::vector<cordinates>& expressedwallMap)
{	
	int tileSizeX = int(floor(1920 / (sizeX + 4)));
	int tilesSizeY= int(floor(1080 / (sizeY + 4)));
	int tileSize;
	if (tileSizeX < tilesSizeY) tileSize = tileSizeX;
	else tileSize = tilesSizeY;
	//
	cordinates cords = std::pair<int, int>(std::make_pair(NULL, NULL));
	std::shared_ptr<Tile> plane = std::make_shared<Tile>(float(1920 / 2), float(1080 / 2), cords, float(tileSize * (sizeX)), float(tileSize * (sizeY)), sf::Color(0, 0, 0 , 69), 0.0f);
	plane->shape.setOrigin(sf::Vector2f(plane->size.x / 2, plane->size.y / 2));
	renderList.emplace_back(plane);
	//
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			float positionX = float((plane->position.x - (plane->size.x / 2) + (x * (tileSize))));
			float positionY = float((plane->position.y - (plane->size.y / 2) + (y * (tileSize))));
			//			
			cordinates myCords = std::pair<int, int>(std::make_pair(x, y));
			std::shared_ptr<Wall> wallN = std::make_shared<Wall>(float(positionX), float(positionY), float(tileSize), float(3), 0.0f);
			wallN->active = true;
			std::shared_ptr<Wall> wallE = std::make_shared<Wall>(float(positionX + tileSize), float(positionY + tileSize), float(tileSize), float(3), 270.0f);
			wallE->active = true;
			std::shared_ptr<Wall> wallS = std::make_shared<Wall>(float(positionX + tileSize), float(positionY + tileSize), float(tileSize), float(3), 180.0f);
			wallS->active = true;
			std::shared_ptr<Wall> wallW = std::make_shared<Wall>(float(positionX), float(positionY), float(tileSize), float(3), 90.0f);
			wallW->active = true;
			//
			std::shared_ptr<Tile> tile = std::make_shared<Tile>(positionX, positionY, myCords, float(tileSize), float(tileSize), sf::Color(0, 191, 255, 255), 0.0f);
			//
			std::shared_ptr<Wall> myWalls[4] = { wallN, wallE, wallS, wallW };
			for (int i = 0; i < 4; i++)
			{
				wallMap[myCords][i] = myWalls[i];
				tile->walls[i] = wallMap[myCords][i];
			}
			//	
			tileMap[myCords] = tile;
			renderList.emplace_back(tile);
	
			
		}
	}

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)																																								
		{																																															
			cordinates myCords = std::pair<int, int>(std::make_pair(x, y));	
			std::shared_ptr<Tile> northNeighbor;
			std::shared_ptr<Tile> eastNeighbor;
			std::shared_ptr<Tile> southNeighbor;
			std::shared_ptr<Tile> westNeighbor;
			//
			if (x > 0)
			{
				cordinates prevXCords = std::pair<int, int>(std::make_pair(x - 1, y));																							
				wallMap[myCords][West] = wallMap[prevXCords][East];
				westNeighbor = tileMap[prevXCords];
			}
			if (y > 0)
			{
				cordinates prevYCords = std::pair<int, int>(std::make_pair(x, y - 1));
				wallMap[myCords][North] = wallMap[prevYCords][South];
				northNeighbor = tileMap[prevYCords];
			}
			//
			expressedwallMap.emplace_back(myCords);
			cordinates nextXCords = std::pair<int, int>(std::make_pair(x + 1, y));
			cordinates nextYCords = std::pair<int, int>(std::make_pair(x, y + 1));
			eastNeighbor = tileMap[nextXCords];
			southNeighbor = tileMap[nextYCords];
			//
			std::shared_ptr<Tile> myneighbors[4] = {northNeighbor, eastNeighbor, southNeighbor, westNeighbor };
			for (int i = 0; i < 4; i++)
			{
				tileMap[myCords]->neighbors[i] = myneighbors[i];
			}
		}
	}
}

std::pair<cordinates, cordinates> generateMaze(int sizeX, int sizeY, std::map<cordinates, walls>& wallMap, std::map<cordinates, std::shared_ptr<Tile>>& tileMap)
{
	std::list<cordinates> exploredTiles;
	//
	int yOpening = rand() % sizeY;
	cordinates opening = std::pair<int, int>(std::make_pair(0, yOpening));
	tileMap[opening]->visited = true;
	wallMap[opening][West]->active = false;
	exploredTiles.emplace_back(opening);
	std::list<cordinates>::iterator currentTile;
	currentTile = exploredTiles.begin();
	//
	while (true)
	{
		std::vector<int> possibleDirections;
		std::vector<cordinates> possibleMoves;
		for (int i = 0; i < 4; i++)
		{
			if (tileMap[*currentTile]->neighbors[i] != nullptr)
			{
				if (tileMap[*currentTile]->neighbors[i]->visited == false)
				{
					possibleMoves.emplace_back(tileMap[*currentTile]->neighbors[i]->gridPos);
					possibleDirections.emplace_back(i);
				}
			}
		}
		if (possibleMoves.size() > 0)
		{
			int move = rand() % possibleDirections.size();
			cordinates nextPath = possibleMoves[move];
			//
			if (nextPath.first >= 0 && nextPath.first < sizeX)
			{
				if (nextPath.second >= 0 && nextPath.second < sizeY)
				{						
					wallMap[*currentTile][possibleDirections[move]]->active = false;
					tileMap[nextPath]->visited = true;
					if (*currentTile == exploredTiles.back())
					{
						exploredTiles.emplace_back(nextPath);
						currentTile++;
					}
					else
					{
						currentTile++;
						exploredTiles.insert(currentTile, nextPath);
						currentTile--;
					}
				}
			}			
		}
		else
		{
			if (currentTile != exploredTiles.begin())
			{
				currentTile--;
			}
			else break;
		}
		
	}
	int yOpeningEnd = rand() % sizeY;
	cordinates openingEnd = std::pair<int, int>(std::make_pair(sizeX - 1, yOpeningEnd));
	wallMap[openingEnd][East]->active = false;
	return std::pair<cordinates, cordinates>(std::make_pair(opening, openingEnd));
}