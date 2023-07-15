#pragma once

class Grid
{
public:
	int sizeX;
	int sizeY;
	std::map<cordinates, std::shared_ptr<Tile>> tileMap;
	std::map<cordinates, walls> wallMap;

	Grid(int sizeX_, int sizeY_);

	void setup(std::vector<std::shared_ptr<Object>>& renderList, std::vector<cordinates>& expressedwallMap);
	std::pair<cordinates, cordinates> recursiveBacktrackingMaze(std::vector<std::shared_ptr<Object>>& renderList, unsigned int deltaTime);
	std::pair<cordinates, cordinates> primsMaze(std::vector<std::shared_ptr<Object>>& renderList, unsigned int deltaTime);
};