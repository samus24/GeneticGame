#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include "Utils.hpp"

class Entity :public sf::Drawable, public sf::Transformable{
public:
	Entity(const sf::Texture& texture);

	Entity(const sf::Texture& texture, sf::Vector2i pos, sf::Vector2i size);

	Entity(const sf::Texture& texture, unsigned int x, unsigned int y, unsigned int sizeX, unsigned int sizeY);

	void setPosition(sf::Vector2f pos);

	sf::Vector2f getPosition() const;

	void move(sf::Vector2f vec);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

protected:
	//sf::Vector2i pos;
	sf::Vector2i size;
	sf::Sprite sprite;
};

#endif