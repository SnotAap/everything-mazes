#include "Header.h"


Robot0::Robot0(Grid grid)
{
	tileSize = grid.tileMap[grid.startAndEndCords.first]->size.x;
	size = sf::Vector2f((tileSize / 1.5f), (tileSize / 1.5f));
	shape.setSize(size);
	shape.setOrigin(sf::Vector2f((size.x / 2), (size.y / 2)));
	gridOffset = sf::Vector2f((grid.tileMap[grid.startAndEndCords.first]->position.x + tileSize / 2), (grid.tileMap[grid.startAndEndCords.first]->position.y + tileSize / 2));
	position = gridOffset;
	shape.setPosition(position);
	gridPos = grid.startAndEndCords.first;
	color = sf::Color::Yellow;
	shape.setFillColor(color);

	setupNodeMap(grid);
	exploredTiles.emplace_back(gridPos);
	currentTile = exploredTiles.begin();
	nodeMap[gridPos]->visited = true;
	grid.tileMap[gridPos]->color = sf::Color::Red;
	grid.tileMap[gridPos]->shape.setFillColor(grid.tileMap[gridPos]->color);
}

void Robot0::setupNodeMap(Grid grid)
{
	for (int x = 0; x < grid.sizeX; x++)
	{
		for (int y = 0; y < grid.sizeY; y++)
		{
			cordinates Cords = std::pair<int, int>(std::make_pair(x, y));
			std::shared_ptr<Node> mazeNodePtr = std::make_shared<Node>(x, y);
			nodeMap[Cords] = mazeNodePtr;
		}
	}
	//setting up neighbors.
	for (int x = 0; x < grid.sizeX; x++)
	{
		for (int y = 0; y < grid.sizeY; y++)
		{
			cordinates myCords = std::pair<int, int>(std::make_pair(x, y));
			std::shared_ptr<Node> northNeighbor;
			std::shared_ptr<Node> eastNeighbor;
			std::shared_ptr<Node> southNeighbor;
			std::shared_ptr<Node> westNeighbor;
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
			if (x < grid.sizeX - 1)
			{
				cordinates nextXCords = std::pair<int, int>(std::make_pair(x + 1, y));
				eastNeighbor = nodeMap[nextXCords];
			}
			if (y < grid.sizeY - 1)
			{
				cordinates nextYCords = std::pair<int, int>(std::make_pair(x, y + 1));
				southNeighbor = nodeMap[nextYCords];
			}
			//
			std::shared_ptr<Node> myneighbors[4] = { northNeighbor, eastNeighbor, southNeighbor, westNeighbor };
			for (int i = 0; i < 4; i++)
			{
				nodeMap[myCords]->neighbors[i] = myneighbors[i];
			}
		}
	}
}

int Robot0::floodFillGetDirection(Grid grid)
{
	movingDirection = -1;
	while (movingDirection < 0)
	{
		std::vector<int> possibleDirections;
		std::vector<cordinates> possibleMoves;
		//
		for (int i = 0; i < accesibleTiles.size(); i++)
		{
			if (nodeMap[*currentTile]->neighbors[accesibleTiles[i]] != nullptr)
			{
				if (nodeMap[*currentTile]->neighbors[accesibleTiles[i]]->visited == false)
				{
		
						possibleMoves.emplace_back(nodeMap[*currentTile]->neighbors[accesibleTiles[i]]->gridPos);
						possibleDirections.emplace_back(accesibleTiles[i]);
				}
			}
		}

		std::cout << "GridPos: " << gridPos.first << ", " << gridPos.second << "possible Moves: " << possibleDirections.size() << "these are: ";
		for (int i = 0; i < possibleDirections.size(); i++)
		{
			switch (possibleDirections[i])
			{
			case 0:
				std::cout << "North, ";
				break;
			case 1:
				std::cout << "East, ";
				break;
			case 2:
				std::cout << "South, ";
				break;
			case 3:
				std::cout << "West, ";
				break;
			}
			
		}			
		std::cout << std::endl;

		if (possibleMoves.size() > 0)
		{
			int move = rand() % possibleDirections.size();
			cordinates nextPath = possibleMoves[move];
			//
			movingDirection = possibleDirections[move];
			nodeMap[nextPath]->visited = true;
			grid.tileMap[nextPath]->color = sf::Color::Red;
			grid.tileMap[nextPath]->shape.setFillColor(grid.tileMap[nextPath]->color);
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
		else
		{
			if (currentTile != exploredTiles.begin())
			{
				std::list<cordinates>::iterator lastTile = currentTile;
				currentTile--;
				movingDirection = nodeMap[*currentTile]->getSide(nodeMap[*lastTile]);


			}
			else break;
		}

	}
	return movingDirection;
}

void Robot0::movement(Grid grid, int deltaTime)
{
	if (!moving)
	{
		updateAccesiblity(grid);
		movingDirection = floodFillGetDirection(grid);
		setFuturePosition();
		move(deltaTime);
		moving = true;
	}
	else
	{
		if (move(deltaTime))
		{
			moving = false;
		}
	}
}

void Robot0::draw(sf::RenderWindow* window)
{
	window->draw(shape);
}