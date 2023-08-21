#include "Header.h"

Grid::Grid(int sizeX_, int sizeY_)
{
	sizeX = sizeX_;
	sizeY = sizeY_;

	timerTime = 2.0f;
	timer = timerTime;
}

void Grid::setup(std::vector<std::shared_ptr<Object>>& renderList)
{
	int tileSizeX = int(floor(1920 / (sizeX + 4)));
	int tilesSizeY = int(floor(1080 / (sizeY + 4)));
	int tileSize;
	if (tileSizeX < tilesSizeY) tileSize = tileSizeX;
	else tileSize = tilesSizeY;
	//
	cordinates cords = std::pair<int, int>(std::make_pair(NULL, NULL));
	std::shared_ptr<Tile> plane = std::make_shared<Tile>(float(1920 / 2), float(1080 / 2), cords, float(tileSize * (sizeX)), float(tileSize * (sizeY)), sf::Color(0, 0, 0, 69), 0.0f);
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
	//
	std::vector<cordinates> expressedwallMap;
	//
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
			if (x < sizeX - 1)
			{
				cordinates nextXCords = std::pair<int, int>(std::make_pair(x + 1, y));
				eastNeighbor = tileMap[nextXCords];
			}
			if (y < sizeY - 1)
			{
				cordinates nextYCords = std::pair<int, int>(std::make_pair(x, y + 1));
				southNeighbor = tileMap[nextYCords];
			}

			//
			std::shared_ptr<Tile> myneighbors[4] = { northNeighbor, eastNeighbor, southNeighbor, westNeighbor };
			for (int i = 0; i < 4; i++)
			{
				tileMap[myCords]->neighbors[i] = myneighbors[i];
			}
		}
	}

	for (int i = 0; i < expressedwallMap.size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{
			renderList.emplace_back(wallMap[expressedwallMap[i]][j]);
		}
	}
}

void Grid::recursiveBacktrackingMaze()
{
	std::list<cordinates> exploredTiles;
	//
	int yOpening = rand() % sizeY;
	cordinates opening = std::pair<int, int>(std::make_pair(0, yOpening));
	tileMap[opening]->visited = true;
	std::pair<cordinates, int> removedStartWall = std::make_pair(opening, West);
	removedWallsOrderd.emplace_back(removedStartWall);
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
					std::pair<cordinates, int> removedWall = std::make_pair(*currentTile, possibleDirections[move]);
					removedWallsOrderd.emplace_back(removedWall);
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
	std::pair<cordinates, int> removedEndWall = std::make_pair(openingEnd, East);
	removedWallsOrderd.emplace_back(removedEndWall);
	startAndEndCords = std::pair<cordinates, cordinates>(std::make_pair(opening, openingEnd));
}

class MazeNode
{
public:
	cordinates position;

	bool partOfMaze = false;
	bool partOfFrontier = false;
	std::shared_ptr<MazeNode> neighbors[4];

	MazeNode(int xPos, int yPos)
	{
		position = std::pair<int, int>(std::make_pair(xPos, yPos));
	}

	int getSide(std::shared_ptr<MazeNode> neighbor)
	{
		int side = -1;
		if (position.first > neighbor->position.first)
		{
			side = East;
		}
		if (position.first < neighbor->position.first)
		{
			side = West;
		}
		if (position.second < neighbor->position.second)
		{
			side = North;
		}
		if (position.second > neighbor->position.second)
		{
			side = South;
		}
		return side;
	}
};

void Grid::primsMaze()
{
	std::vector<cordinates> maze;
	std::vector<cordinates> frontier;
	std::map<cordinates, std::shared_ptr<MazeNode>> nodeMap;
	//initializing nodes.
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			cordinates Cords = std::pair<int, int>(std::make_pair(x, y));
			std::shared_ptr<MazeNode> mazeNodePtr = std::make_shared<MazeNode>(x, y);
			nodeMap[Cords] = mazeNodePtr;
		}
	}
	//setting up neighbors.
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			cordinates myCords = std::pair<int, int>(std::make_pair(x, y));
			std::shared_ptr<MazeNode> northNeighbor;
			std::shared_ptr<MazeNode> eastNeighbor;
			std::shared_ptr<MazeNode> southNeighbor;
			std::shared_ptr<MazeNode> westNeighbor;
			//
			if (x > 0)
			{
				cordinates prevXCords = std::pair<int, int>(std::make_pair(x - 1, y));
				westNeighbor = nodeMap[prevXCords];
			}
			if (y > 0)
			{
				cordinates prevYCords = std::pair<int, int>(std::make_pair(x, y - 1));
				northNeighbor = nodeMap[prevYCords];
			}
			if (x < sizeX - 1)
			{
				cordinates nextXCords = std::pair<int, int>(std::make_pair(x + 1, y));
				eastNeighbor = nodeMap[nextXCords];
			}
			if (y < sizeY - 1)
			{
				cordinates nextYCords = std::pair<int, int>(std::make_pair(x, y + 1));
				southNeighbor = nodeMap[nextYCords];
			}
			//
			std::shared_ptr<MazeNode> myneighbors[4] = { northNeighbor, eastNeighbor, southNeighbor, westNeighbor };
			for (int i = 0; i < 4; i++)
			{
				nodeMap[myCords]->neighbors[i] = myneighbors[i];
			}
		}
	}
	//setting up random starting point
	int randX = rand() % sizeX;
	int randY = rand() % sizeY;
	cordinates randStartCord = std::pair<int, int>(std::make_pair(randX, randY));
	maze.emplace_back(randStartCord);
	nodeMap[maze[0]]->partOfMaze = true;
	//
	int count = 0;
	//
	while (maze.size() < (sizeX * sizeY))
	{
		//updating frontier
		for (int i = 0; i < 4; i++)
		{
			if (nodeMap[maze[count]]->neighbors[i] != nullptr)
			{
				if (!nodeMap[maze[count]]->neighbors[i]->partOfFrontier && !nodeMap[maze[count]]->neighbors[i]->partOfMaze)
				{
					cordinates neighborPosition = nodeMap[maze[count]]->neighbors[i]->position;
					frontier.emplace_back(neighborPosition);
					nodeMap[neighborPosition]->partOfFrontier = true;
				}
			}
		}
		int randFront = rand() % frontier.size();
		std::vector<cordinates> adjecentMazeTiles;
		//getting maze neighbors of rand Frontier point
		for (int i = 0; i < 4; i++)
		{

			if (nodeMap[frontier[randFront]]->neighbors[i] != nullptr)
			{
				cordinates neighborCords = nodeMap[frontier[randFront]]->neighbors[i]->position;
				if (nodeMap[neighborCords]->partOfMaze)
				{
					adjecentMazeTiles.emplace_back(neighborCords);
				}
			}
		}
		//choosing random maze neighbor to carve path.
		int randAdjecentMazeTile = rand() % adjecentMazeTiles.size();
		nodeMap[frontier[randFront]]->partOfFrontier = false;
		nodeMap[frontier[randFront]]->partOfMaze = true;
		maze.emplace_back(frontier[randFront]);

		std::pair<cordinates, int> removedWall = std::make_pair(frontier[randFront], nodeMap[adjecentMazeTiles[randAdjecentMazeTile]]->getSide(nodeMap[frontier[randFront]]));
		removedWallsOrderd.emplace_back(removedWall);
		frontier.erase(frontier.begin() + randFront);
		//
		count++;
	}
	int yOpening = rand() % sizeY;
	cordinates opening = std::pair<int, int>(std::make_pair(0, yOpening));
	std::pair<cordinates, int> removedStartWall = std::make_pair(opening, West);
	removedWallsOrderd.emplace_back(removedStartWall);


	int yOpeningEnd = rand() % sizeY;
	cordinates openingEnd = std::pair<int, int>(std::make_pair(sizeX - 1, yOpeningEnd));
	std::pair<cordinates, int> removedEndWall = std::make_pair(openingEnd, East);
	removedWallsOrderd.emplace_back(removedEndWall);
	startAndEndCords = std::pair<cordinates, cordinates>(std::make_pair(opening, openingEnd));
}

bool Grid::removeWalls(int deltaTime)
{
	
	if (wallRemoveItorater < removedWallsOrderd.size())
	{
		timer -= deltaTime;
		if (timer <= 0)
		{

			wallMap[removedWallsOrderd[wallRemoveItorater].first][removedWallsOrderd[wallRemoveItorater].second]->active = false;
			wallRemoveItorater++;
			timer = timerTime;

		}
		return false;
	}
	else
	{
		return true;
	}
}