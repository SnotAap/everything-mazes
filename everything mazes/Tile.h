#pragma once

class Tile : public Object
{
public:
	sf::Vector2f size;
	cordinates gridPos;
	float rotation;
	sf::Color color;
	sf::RectangleShape shape;
	walls walls;
	bool visited = false;
	bool inList = false;
	std::shared_ptr<Tile> neighbors[4];
	//
	Tile(float xPos_, float yPos_, cordinates gridPos_, float xSize_, float ySize_, sf::Color color_, float rotation_);
	
	virtual void draw(sf::RenderWindow* window) override;
};