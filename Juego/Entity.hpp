#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

class Entity :public sf::Drawable, public sf::Transformable{
public:
	Entity(const sf::Texture& texture);

	Entity(const sf::Texture& texture, sf::Vector2i pos, sf::Vector2i size);

	Entity(const sf::Texture& texture, unsigned int x, unsigned int y, unsigned int sizeX, unsigned int sizeY);

	void setPos(sf::Vector2i pos);

	sf::Vector2i getPos() const;

	void move(sf::Vector2i vec);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

protected:
	sf::Vector2i pos;
	sf::Vector2i size;
	sf::Sprite sprite;
};

#endif