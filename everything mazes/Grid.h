#pragma once

class Grid
{
public:
	int sizeX;
	int sizeY;
	std::map<cordinates, std::shared_ptr<Tile>> tileMap;
	std::map<cordinates, walls> wallMap;
	std::vector <std::pair<cordinates, int>> removedWallsOrderd;
	std::pair<cordinates, cordinates> startAndEndCords;

    #define timerTime 10
	int timer = timerTime;
	int wallRemoveItorater = 0;

	Grid(int sizeX_, int sizeY_);

	void setup(std::vector<std::shared_ptr<Object>>& renderList);
	void recursiveBacktrackingMaze();
	void primsMaze();

	bool removeWalls(int deltaTime);
};