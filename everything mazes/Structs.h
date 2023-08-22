#pragma once

struct Node
{
public:
	bool visited = false;
	double globalGoal = 0.0;
	double localGoal = 0.0;
	cordinates position;
	walls walls;
	std::shared_ptr<Node> neighbors[4];
	std::shared_ptr<Node> parent = nullptr;

	bool isAccesible(std::shared_ptr<Node> neighbor)
	{
		int side = 0;
		if (position.first < neighbor->position.first)
		{
			side = East;
		}		
		if (position.first > neighbor->position.first)
		{
			side = West;
		}				
		if (position.second > neighbor->position.second)
		{
			side = North;
		}				
		if (position.second < neighbor->position.second)
		{
			side = South;
		}
		if (walls[side]->active)
			return false;
		else
			return true;		
	}
};