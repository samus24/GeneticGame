#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <string>

enum Direction
{
	NORTH, SOUTH, EAST, WEST
};

class Entity
{
	public:
		Entity()
		{}

		Entity(const std::string &image)
		{
			texture.loadFromFile(image);
			sprite.setTexture(texture);

			posX = 0;
			posY = 0;
			block = false;

			facing = Direction::SOUTH;
		}

		Direction getDirection()
		{
			return facing;
		}

		sf::Sprite getSprite()
		{
			return sprite;
		}

		bool getBlock()
		{
			return block;
		}

		bool attackPossible(Entity rival)
		{

		}

		void updatePosition(int x, int y)
		{
			posX = x;
			posY = y;
			sprite.setPosition(x, y);
		}

		virtual bool attack(Entity rival);

	protected:
		int posX;
		int posY;

		Direction facing;
		bool block;

		sf::Texture texture;
		sf::Sprite sprite;
};

#endif
