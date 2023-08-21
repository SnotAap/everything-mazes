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
			std::shared_ptr<Tile> myneighbors[4] = {northNeighbor, eastNeighbor, southNeighbor, westNeighbor };
			for (int i = 0; i < 4; i++)
			{
				tileMap[myCords]->neighbors[i] = myneighbors[i];
			}
		}
	}
}

std::pair<cordinates, cordinates> recursiveBacktrackingMaze(int sizeX, int sizeY, std::map<cordinates, walls>& wallMap, std::map<cordinates, std::shared_ptr<Tile>>& tileMap)
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

std::pair<cordinates, cordinates> primsMaze(int sizeX, int sizeY, std::map<cordinates, walls>& wallMap, std::map<cordinates, std::shared_ptr<Tile>>& tileMap)
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
		
		wallMap[frontier[randFront]][nodeMap[adjecentMazeTiles[randAdjecentMazeTile]]->getSide(nodeMap[frontier[randFront]])]->active = false;
		frontier.erase(frontier.begin() + randFront);
		//
		count++;

	}	
	int yOpening = rand() % sizeY;
	cordinates opening = std::pair<int, int>(std::make_pair(0, yOpening));
	wallMap[opening][West]->active = false;
	int yOpeningEnd = rand() % sizeY;
	cordinates openingEnd = std::pair<int, int>(std::make_pair(sizeX - 1, yOpeningEnd));
	wallMap[openingEnd][East]->active = false;
	return std::pair<cordinates, cordinates>(std::make_pair(opening, openingEnd));
}

std::pair<cordinates, cordinates> kruskalsMaze(int sizeX, int sizeY, std::map<cordinates, walls>& wallMap, std::map<cordinates, std::shared_ptr<Tile>>& tileMap)
{
	std::list<cordinates> startingtiles;
	std::map<cordinates, std::shared_ptr<MazeNode>> nMap;

	int rX = rand() % sizeX;
	int rY = rand() % sizeY;	
	cordinates rStartCord = std::pair<int, int>(std::make_pair(rX, rY));
	tileMap[rStartCord]->inList = true;
	tileMap[rStartCord]->shape.setFillColor(sf::Color::Red);	

	std::list<std::list<std::shared_ptr<Tile>>> mainlist;
	std::list <std::list<std::shared_ptr<Tile>>> ::iterator basedTile;
	basedTile = mainlist.begin();

	std::list<std::shared_ptr<Tile>> pathList;
	pathList.push_front(tileMap[rStartCord]);
	mainlist.push_front(pathList);

	int yOpening = rand() % sizeY;
	cordinates openingEnd = std::pair<int, int>(std::make_pair(sizeX - 1, rY));
	wallMap[openingEnd][East]->active = false;
	cordinates opening = std::pair<int, int>(std::make_pair(0, yOpening));
	wallMap[opening][West]->active = false;

	for (int i = 0; i < 50; i++)
	{		
		cordinates pickedTile = std::pair<int, int>(std::make_pair(rand() % sizeX, rand() % sizeY));
		/*if (tileMap[pickedTile]->inList == true)
		{			
			while (tileMap[pickedTile]->inList == true)
			{
				pickedTile = std::pair<int, int>(std::make_pair(rand() % sizeX, rand() % sizeY));
			}			
		}*/

		int randW = 2;
		//tiles die aangrensend zijn groeperen en een beter stop punt voor de for-loop maken
		
		if (tileMap[pickedTile]->gridPos.first == 0)
		{			
			if (tileMap[pickedTile]->gridPos.second == 0)
			{
				randW = ((rand() % 2) + 1);				
			}
			else if (tileMap[pickedTile]->gridPos.second == 19)
			{
				randW = rand() % 2;				
			}			
			else if (tileMap[pickedTile]->gridPos.second > 0 && tileMap[pickedTile]->gridPos.second < 19)
			{
				randW = rand() % 3;				
			}
		}
		else if (tileMap[pickedTile]->gridPos.second == 0)
		{
			if (tileMap[pickedTile]->gridPos.first > 0 && tileMap[pickedTile]->gridPos.first < 19)
			{
				randW = ((rand() % 3) + 1);				
			}
			else if (tileMap[pickedTile]->gridPos.first == 19)
			{
				randW = ((rand() % 2) + 2);				;
			}
		}
		else if (tileMap[pickedTile]->gridPos.second == 19)
		{
			if (tileMap[pickedTile]->gridPos.first > 0 && tileMap[pickedTile]->gridPos.first < 19)
			{
				while (randW == 2) {
					randW = rand() % 4;
				}							
			}
			else if (tileMap[pickedTile]->gridPos.first == 19)
			{
				while (randW == 2 || randW == 1)
				{
					randW = rand() % 4;
				}
			}
		}
		else if (tileMap[pickedTile]->gridPos.first == 19)
		{
			if (tileMap[pickedTile]->gridPos.second > 0 && tileMap[pickedTile]->gridPos.second < 19)
			{
				randW = rand() % 4;
				while (randW == 1)
				{
					randW = rand() % 4;					
				}
			}			
		}
		else
		{
			randW = rand() % 4;
		}		
		
		/*pickedTile-> inList == true && neighbors-> inList == true
		*merge lists
		* remove wall*/

		/*pickedTile-> inList == true && neighbors-> inList == fasle	
		* add neighbor to list	
		* remove wall*/

		/*pickedTile-> inList == false && neighbors-> inList == true
		* add pickedTile to list
		* remove wall*/		

		/*pickedTile-> inList == false && neighbors-> inList == false
		* creat list and add both
		* remove wall*/
		

		/*if (tileMap[pickedTile]->neighbors[randW]->listtileMap[pickedTile]->neighbors[randW]->inList == true)
		{
			wallMap[pickedTile][randW]->active = false;
			//tileMap[pickedTile]->mainpath = true;
			//mainlist.push_back(tileMap[pickedTile]);
		}
		else
		{
			wallMap[pickedTile][randW]->active = false;
		}*/

		if (tileMap[pickedTile]->inList == true)
		{
			
			if (tileMap[pickedTile]->neighbors[randW]->inList == true)
			{
				//merge lists
				wallMap[pickedTile][randW]->active = false;
			}
			else
			{
				//add to list
				tileMap[pickedTile]->inList = true;
				wallMap[pickedTile][randW]->active = false;
			}

		}
		else
		{
			
			if (tileMap[pickedTile]->neighbors[randW]->inList == true)
			{
				//join list

				tileMap[pickedTile]->inList = true;
				wallMap[pickedTile][randW]->active = false;

			}
			else
			{
				//make list
				std::list<std::shared_ptr<Tile>> myList;
				myList.push_back(tileMap[pickedTile]);
				myList.push_back(tileMap[pickedTile]->neighbors[randW]);
				tileMap[pickedTile]->inList = true;
				tileMap[pickedTile]->neighbors[randW]->inList = true;
				wallMap[pickedTile][randW]->active = false;
			}
		}		
	}	
	return std::pair<cordinates, cordinates>(std::make_pair(opening, openingEnd));
}