#pragma once

class Line : public Object
{
public:
	sf::Vector2f size;
	float rotation;
	sf::Color color;
	sf::RectangleShape shape;

	Line(float xPos_, float yPos_, float xSize_, float ySize_, float rotation_);
	virtual void draw(sf::RenderWindow* window) override; 
};