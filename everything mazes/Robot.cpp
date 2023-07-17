#include "Header.h"

BaseRobot::BaseRobot(Grid grid)
{
	tileSize = grid.tileMap[grid.startAndEndCords.first]->size.x;
	size = sf::Vector2f((tileSize / 1.5f), (tileSize / 1.5f));
	shape.setSize(size);
	shape.setOrigin(sf::Vector2f((size.x / 2), (size.y / 2)));
	gridOffset = sf::Vector2f((grid.tileMap[grid.startAndEndCords.first]->position.x + tileSize / 2), (grid.tileMap[grid.startAndEndCords.first]->position.y + tileSize / 2));
	position = gridOffset;
	shape.setPosition(position);	
	gridPos = grid.startAndEndCords.first;
	color = sf::Color::Red;
	shape.setFillColor(color);
}

void BaseRobot::updateAccesiblity(Grid grid)
{
	accesibleTiles.clear();
	if (gridPos.second > 0)					//north
	{
		//if (grid.wallMap[gridPos][North] != nullptr)
		{
			if (!grid.wallMap[gridPos][North]->active)
			{
				accesibleTiles.emplace_back(North);
			}
		}

	}
	if (gridPos.first < grid.sizeX - 1)		//east
	{
		
		//if (grid.wallMap[gridPos][North] != nullptr)
		{
			if (!grid.wallMap[gridPos][East]->active)
			{
				accesibleTiles.emplace_back(East);
			}
		}

	}
	if (gridPos.second < grid.sizeY - 1)	//south
	{
		//if (grid.wallMap[gridPos][North] != nullptr)
		{
			if (!grid.wallMap[gridPos][South]->active)
			{
				accesibleTiles.emplace_back(South);
			}
		}

	}
	if (gridPos.first > 0)					//west
	{
		//if (grid.wallMap[gridPos][North] != nullptr)
		{
			if (!grid.wallMap[gridPos][West]->active)
			{
				accesibleTiles.emplace_back(West);
			}
		}

	}
}

bool BaseRobot::moveNorth(int deltaTime)
{
	if (position.y > futurePosition.y)
	{
		position.y -= speed;// *deltaTime;
		shape.setPosition(position);
		return false;
	}
	else
	{
		position = futurePosition;
		shape.setPosition(position);
		gridPos.second--;
		return true;
	}
}

bool BaseRobot::moveEast(int deltaTime)
{
	if (position.x < futurePosition.x)
	{
		position.x += speed;// * deltaTime;
		shape.setPosition(position);
		return false;
	}
	else
	{
		position = futurePosition;
		shape.setPosition(position);
		gridPos.first++;
		return true;
	}
}

bool BaseRobot::moveSouth(int deltaTime)
{
	if (position.y < futurePosition.y)
	{
		position.y += speed;// * deltaTime;
		shape.setPosition(position);
		return false;
	}
	else
	{
		position = futurePosition;
		shape.setPosition(position);
		gridPos.second++;
		return true;
	}
}

bool BaseRobot::moveWest(int deltaTime)
{
	if (position.x > futurePosition.x)
	{
		position.x -= speed;// * deltaTime;
		shape.setPosition(position);
		return false;
	}
	else
	{
		position = futurePosition;
		shape.setPosition(position);
		gridPos.first--;
		return true;
	}
}

void BaseRobot::setFuturePosition()
{
	switch (movingDirection)
	{
	case North:
		futurePosition = sf::Vector2f(position.x, position.y - tileSize);
		break;
	case East:
		futurePosition = sf::Vector2f(position.x + tileSize, position.y);
		break;
	case South:
		futurePosition = sf::Vector2f(position.x, position.y + tileSize);
		break;
	case West:
		futurePosition = sf::Vector2f(position.x - tileSize, position.y);
		break;
	}
	shape.setPosition(position);
}

bool BaseRobot::move(int deltaTime)
{
	switch (movingDirection)
	{
	case North:
		return moveNorth(deltaTime);
		break;
	case East:
		return moveEast(deltaTime);
		break;
	case South:
		return moveSouth(deltaTime);
		break;
	case West:
		return moveWest(deltaTime);
		break;
	}
}

void BaseRobot::randomMovement(Grid grid, int deltaTime)
{	
	if (!moving)
	{
		updateAccesiblity(grid);

		movingDirection = accesibleTiles[rand() % accesibleTiles.size()];
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

void BaseRobot::draw(sf::RenderWindow* window)
{
	window->draw(shape);
}