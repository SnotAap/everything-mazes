#include "Header.h"

double distance(cordinates start, cordinates end)
{
	return double(sqrt((pow(end.first, 2) - pow(start.first, 2)) + (pow(end.second, 2) - pow(start.second, 2))));
}


std::vector<cordinates> Astar(int sizeX, int sizeY, std::map<cordinates, walls>& wallMap, cordinates start, cordinates end)
{
	std::map<cordinates, std::shared_ptr<Node>> nodeMap;
	std::vector<cordinates> path;
	//
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			cordinates cord = std::pair<int, int>(std::make_pair(x, y));
			std::shared_ptr<Node> node;
			cordinates myCords = std::pair<int, int>(std::make_pair(x, y));
			for (int i = 0; i < 4; i++)
			{
				node->walls[i] = wallMap[cord][i];
			}			
			node->position = myCords;
			nodeMap[cord] = node;			
		}
	}
	//
	std::shared_ptr<Node> startNode = nodeMap[start];
	startNode->localGoal = 0.0;
	startNode->globalGoal = distance(start, end);
	std::shared_ptr<Node> finishNode = nodeMap[end];
	std::shared_ptr<Node> currentNode = startNode;
	//
	std::list<std::shared_ptr<Node>> listNotTestedNodes;
	listNotTestedNodes.push_back(startNode);
	//
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
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
			//
			if (y > 0)
			{
				cordinates prevYCords = std::pair<int, int>(std::make_pair(x, y - 1));
				northNeighbor = nodeMap[prevYCords];
			}
			//			
			cordinates nextXCords = std::pair<int, int>(std::make_pair(x + 1, y));
			cordinates nextYCords = std::pair<int, int>(std::make_pair(x, y + 1));
			eastNeighbor = nodeMap[nextXCords];
			southNeighbor = nodeMap[nextYCords];
			//
			std::shared_ptr<Node> myneighbors[4] = { northNeighbor, eastNeighbor, southNeighbor, westNeighbor };
			for (int i = 0; i < 4; i++)
			{
				nodeMap[myCords]->neighbors[i] = myneighbors[i];
			}
		}
	}

	while (!listNotTestedNodes.empty() && currentNode != finishNode)
	{
		listNotTestedNodes.sort([](const Node* lhs, const Node* rhs) {return lhs->globalGoal < rhs->globalGoal; });
		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->visited)
		{
			listNotTestedNodes.pop_front();
		}
		//
		if (listNotTestedNodes.empty()) break;
		//
		currentNode = listNotTestedNodes.front();
		currentNode->visited = true;
		//
		for (auto nodeNeighbor : currentNode->neighbors)
		{
			if (!nodeNeighbor->visited && currentNode->isAccesible(nodeNeighbor))
			{
				listNotTestedNodes.push_back(nodeNeighbor);
			}
			//
			double possiblyLowerGoal = currentNode->localGoal + distance(currentNode->position, nodeNeighbor->position);
			//
			if (possiblyLowerGoal < nodeNeighbor->localGoal)
			{
				nodeNeighbor->parent = currentNode;
				nodeNeighbor->localGoal = possiblyLowerGoal;
				nodeNeighbor->globalGoal = nodeNeighbor->localGoal + distance(nodeNeighbor->position, finishNode->position);
			}
		}
	}
	//loop through the correct tiles.
	std::shared_ptr<Node> p = finishNode;
	while (p->parent != nullptr)
	{
		path.emplace_back(p->position);
		p = p->parent;
	}
	//return the solved path
	return path;
}

//void Player::findPath(std::vector<Line*>& lineList, TileMap& map, sf::Vector2i goalPos)
//{
//	//Node *nodes = nullptr;
//
//	std::list<sf::Vector2i> path;
//	int playerPosX = (int)floor(((Player::position.x - 25) / 50));
//	int playerPosY = (int)floor(((Player::position.y - 25) / 50));
//	int taskPosX = (int)goalPos.x;
//	int taskPosY = (int)goalPos.y;
//	//
//
//
//	//initialising all nodes;
//	static Node nodes[mapWidth * mapHeight];
//
//	for (int x = 0; x < mapWidth; x++)
//	{
//		for (int y = 0; y < mapHeight; y++)
//		{
//			nodes[y * mapWidth + x].position = sf::Vector2f((float)x, (float)y);
//			nodes[y * mapWidth + x].visited = false;
//			nodes[y * mapWidth + x].parent = nullptr;
//			nodes[y * mapWidth + x].obstacle = map[y * mapWidth + x]->obstacle;
//			nodes[y * mapWidth + x].globalGoal = INFINITY;
//			nodes[y * mapWidth + x].localGoal = INFINITY;
//		}
//	}
//	//
//	Node* startNode = &nodes[playerPosY * mapWidth + playerPosX];
//	Node* finishNode = &nodes[taskPosY * mapWidth + taskPosX];
//	if (!finishNode->obstacle)
//	{
//		Node* currentNode;
//		startNode->localGoal = 0.0f;
//		startNode->globalGoal = distance(startNode, finishNode);
//		currentNode = startNode;
//
//		std::list<Node*> listNotTestedNodes;
//		listNotTestedNodes.push_back(startNode);
//		//create connections between the nodes, (the neighbors)
//		for (int x = 0; x < mapWidth - 1; x++)
//		{
//			for (int y = 0; y < mapHeight - 1; y++)
//			{
//				if (y > 0)             nodes[y * mapWidth + x].vecNeighbours.emplace_back(&nodes[(y - 1) * mapWidth + (x + 0)]);
//				if (y < mapHeight) nodes[y * mapWidth + x].vecNeighbours.emplace_back(&nodes[(y + 1) * mapWidth + (x + 0)]);
//				if (x > 0)             nodes[y * mapWidth + x].vecNeighbours.emplace_back(&nodes[(y + 0) * mapWidth + (x - 1)]);
//				if (x < mapWidth)  nodes[y * mapWidth + x].vecNeighbours.emplace_back(&nodes[(y + 0) * mapWidth + (x + 1)]);
//
//			}
//		}
//		//
//		while (!listNotTestedNodes.empty() && currentNode != finishNode)
//		{
//			listNotTestedNodes.sort([](const Node* lhs, const Node* rhs) {return lhs->globalGoal < rhs->globalGoal; });
//			while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->visited)
//			{
//				listNotTestedNodes.pop_front();
//			}
//
//			if (listNotTestedNodes.empty()) break;
//
//			currentNode = listNotTestedNodes.front();
//			currentNode->visited = true;
//
//			for (auto nodeNeightbour : currentNode->vecNeighbours)
//			{
//				if (!nodeNeightbour->visited && !nodeNeightbour->obstacle)
//				{
//					listNotTestedNodes.push_back(nodeNeightbour);
//				}
//
//				float possiblyLowerGoal = currentNode->localGoal + distance(currentNode, nodeNeightbour);
//
//				if (possiblyLowerGoal < nodeNeightbour->localGoal)
//				{
//					nodeNeightbour->parent = currentNode;
//					nodeNeightbour->localGoal = possiblyLowerGoal;
//					nodeNeightbour->globalGoal = nodeNeightbour->localGoal + distance(nodeNeightbour, finishNode);
//				}
//			}
//		}
//
//		//loop through the correct tiles.
//		Node* p = finishNode;
//		while (p->parent != nullptr)
//		{
//
//			Line* myLine = new Line(sf::Vector2f(p->position.x * mapWidth / 2 + 25.0f, p->position.y * mapHeight / 2 + 25.0f), sf::Vector2f(p->parent->position.x * mapWidth / 2 + 25.0f, p->parent->position.y * mapHeight / 2 + 25.0f));
//			lineList.emplace_back(myLine);
//			path.emplace_back(sf::Vector2i((int)p->position.x, (int)p->position.y));
//			p = p->parent;
//
//		}
//	}
//	//return the solved path
//
//	currentPath = path;
//}