#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "Entity.hpp"

enum Direction
{
	NORTH, SOUTH, EAST, WEST
};

class Player : public Entity
{
	public:
		Player()
		{}

		Player(const std::string &image)
		{
			Entity::Entity(image);
			health = 3;
		}

		void subHealth()
		{
			health--;
		}

		bool attack(Entity rival)
		{
			if (rival.getBlock())
				return false;
			else if (posX != r)
			else
			{

			}
		}


	private:
		int health;
};

#endif
