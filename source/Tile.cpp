#include "Header.h"
Tile::Tile(float xPos_, float yPos_, cordinates gridPos_, float xSize_, float ySize_, sf::Color color_, float rotation_)
{
	position = sf::Vector2f(xPos_, yPos_);
	gridPos = gridPos_;
	size = sf::Vector2f(xSize_, ySize_);
	rotation = rotation_;
	color = color_;
	shape.setPosition(position);
	shape.setSize(size);
	shape.setRotation(rotation);
	shape.setFillColor(color);
}



void Tile::draw(sf::RenderWindow* window)
{
	window->draw(shape);
}


