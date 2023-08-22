#include "Header.h"

void Line::draw(sf::RenderWindow* window)
{
	window->draw(shape);
}

Line::Line(float xPos_, float yPos_, float xSize_, float ySize_, float rotation_)
{
	position = sf::Vector2f(xPos_, yPos_);
	size = sf::Vector2f(xSize_, ySize_);
	rotation = rotation_;
	color = sf::Color::Black;
	shape.setPosition(position);
	shape.setSize(size);
	shape.setRotation(rotation);
	shape.setFillColor(color);

}