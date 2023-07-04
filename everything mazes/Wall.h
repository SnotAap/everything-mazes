#pragma once

class Wall : public Object
{
public:
	sf::Vector2f size;
	float rotation;
	sf::Color color;
	bool active = false;
	sf::RectangleShape shape;

	Wall(float xPos_, float yPos_, float xSize_, float ySize_, float rotation_);
	virtual void draw(sf::RenderWindow* window) override;
};