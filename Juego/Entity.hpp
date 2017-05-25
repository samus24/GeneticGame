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

		sf::Sprite getSprite() const
		{
			return sprite;
		}

		bool getBlock() const
		{
			return block;
		}

		void subHealth()
		{
			health--;
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
		int health;

		Direction facing;
		bool block;

		sf::Texture texture;
		sf::Sprite sprite;

		bool attackPossible(const Entity &a, const Entity &b) const
		{
			bool aa = a.posX == b.posX && a.posY == b.posY + 1;
			bool ab = a.posX == b.posX && a.posY == b.posY - 1;

			bool ba = a.posY == b.posY && a.posX == b.posX - 1;
			bool bb = a.posY == b.posY && a.posX == b.posX + 1;

			bool posA = aa && (a.facing == Direction::WEST);	//Enemigo justo a la izquierda del jugador y jugador mirándole
			bool posB = ab && (a.facing == Direction::EAST);	//Enemigo justo a la derecha del jugador y jugador mirándole
			bool posC = ba && (a.facing == Direction::NORTH);	//Enemigo justo encima del jugador y jugador mirándole
			bool posD = bb && (a.facing == Direction::SOUTH);	//Enemigo justo debajo del jugador y jugador mirándole

			return (posA || posB || posC || posD);
		}
};

#endif
