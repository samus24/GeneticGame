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

sf::IntRect Entity::getBounds() const{
	sf::Vector2f pos = this->getPosition();
	return sf::IntRect(pos.x, pos.y, size.x, size.y);
}

std::vector<sf::Vector2f> Entity::getCorners() const{
	std::vector<sf::Vector2f> corners;
	sf::Vector2f pos = getPosition();
	corners.push_back(pos);
	pos.x += TILESIZE.x - 2;
	corners.push_back(pos);
	pos.x -= TILESIZE.x - 2;
	pos.y += TILESIZE.y - 2;
	corners.push_back(pos);
	pos.x += TILESIZE.x - 2;
	corners.push_back(pos);
	return corners;
}

sf::Vector2f Entity::getCenter() const{
	return sf::Transformable::getPosition() + sf::Vector2f(size.x * 0.5, size.y * 0.5);
}

void Entity::move(sf::Vector2f vec){
	sf::Transformable::move(vec);
	sprite.setPosition(this->getPosition());
}