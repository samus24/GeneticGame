#include "Entity.hpp"

Entity::Entity(const sf::Texture& texture):
	sprite(texture, TILE0)
{
	sf::Transformable::setPosition(sf::Vector2f(0,0));
	size.x = 1;
	size.y = 1;
}

Entity::Entity(const sf::Texture& texture, sf::Vector2i pos, sf::Vector2i size):
	sprite(texture, TILE0)
{
	sf::Transformable::setPosition(sf::Vector2f(pos));
	this->size = size;
}

Entity::Entity(const sf::Texture& texture, unsigned int x, unsigned int y, unsigned int sizeX, unsigned int sizeY):
	sprite(texture, TILE0)
{
	sf::Transformable::setPosition(sf::Vector2f(x,y));
	size.x = sizeX;
	size.y = sizeY;
}

void Entity::setPosition(sf::Vector2f pos){
	sf::Transformable::setPosition(pos);
	sprite.setPosition(pos);
}

sf::Vector2f Entity::getPosition() const{
	return sf::Transformable::getPosition();
}

void Entity::move(sf::Vector2f vec){
	sf::Transformable::move(vec);
	sprite.setPosition(this->getPosition());
}