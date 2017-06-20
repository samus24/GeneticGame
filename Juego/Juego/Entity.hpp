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

	sf::Vector2f getCenter() const;

	sf::IntRect getBounds() const;

	sf::FloatRect getFloatBounds() const;

	std::vector<sf::Vector2f> getCorners() const;

	void move(sf::Vector2f vec);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

protected:
	//sf::Vector2i pos;
	sf::Vector2i size;
	sf::Sprite sprite;
};

#endif