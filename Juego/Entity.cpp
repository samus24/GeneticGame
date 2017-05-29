#include "Entity.hpp"

Entity::Entity(const sf::Texture& texture):
	sprite(texture)
{
	pos.x = 0;
	pos.y = 0;
	size.x = 1;
	size.y = 1;
}

Entity::Entity(const sf::Texture& texture, sf::Vector2i pos, sf::Vector2i size):
	sprite(texture)
{
	this->pos = pos;
	this->size = size;
}

Entity::Entity(const sf::Texture& texture, unsigned int x, unsigned int y, unsigned int sizeX, unsigned int sizeY):
	sprite(texture)
{
	pos.x = x;
	pos.y = y;
	size.x = sizeX;
	size.y = sizeY;
}

void Entity::setPos(sf::Vector2i pos){
	this->pos = pos;
}

sf::Vector2i Entity::getPos() const{
	return pos;
}

void Entity::move(sf::Vector2i vec){
	pos += vec;		// Comprobar rango negativo
}