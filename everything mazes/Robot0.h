#pragma once
class Robot0 : public BaseRobot
{
private:
	struct Node
	{
		cordinates gridPos;
		bool visited = false;
		std::shared_ptr<Node> neighbors[4];

		Node(int xPos, int yPos)
		{
			gridPos = std::pair<int, int>(std::make_pair(xPos, yPos));
		}

		int getSide(std::shared_ptr<Node> neighbor)
		{
			int side = -1;
			if (gridPos.first > neighbor->gridPos.first)
			{
				side = East;
			}
			if (gridPos.first < neighbor->gridPos.first)
			{
				side = West;
			}
			if (gridPos.second < neighbor->gridPos.second)
			{
				side = North;
			}
			if (gridPos.second > neighbor->gridPos.second)
			{
				side = South;
			}
			return side;
		}
	};

public:
	std::list<cordinates> exploredTiles;
	std::list<cordinates>::iterator currentTile;

	std::map<cordinates, std::shared_ptr<Node>> nodeMap;

	void setupNodeMap(Grid grid);

	Robot0(Grid grid);


	int floodFillGetDirection(Grid grid);

	virtual void movement(Grid grid, int deltaTime) override;

	virtual void draw(sf::RenderWindow* window) override;


};