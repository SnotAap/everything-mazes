#pragma once

class BaseRobot : public Object
{
public:
	sf::RectangleShape shape;
	sf::Color color;
	sf::Vector2f size;
	sf::Vector2f gridOffset;
	sf::Vector2f futurePosition;
	cordinates gridPos;
	float tileSize;
	float speed = 1.0f;
	std::vector<int> accesibleTiles;
	bool moving = false;
	int movingDirection;

	BaseRobot(Grid grid);
	void updateAccesiblity(Grid grid);
	
	void setFuturePosition();
	bool moveNorth(int deltaTime);
	bool moveEast(int deltaTime);
	bool moveSouth(int deltaTime);
	bool moveWest(int deltaTime);
	bool move(int deltaTime);

	void randomMovement(Grid grid, int deltaTime);

	virtual void draw(sf::RenderWindow* window) override;
};