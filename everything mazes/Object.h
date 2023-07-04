#pragma once

class Object
{
public:
	sf::Vector2f position;	

	virtual void draw(sf::RenderWindow* window);
};