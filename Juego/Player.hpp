#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "Entity.hpp"
#include "Chest.hpp"

const int MAX_LIFES = 5;

class Player: public Entity
{
	public:
		Player()
		{}

		Player(const std::string &image)
		{
			Entity::Entity(image);
			health = 3;
			key = false;
		}

		bool attack(const Entity &rival)
		{
			if (rival.getBlock())
				return false;
			else if (!attackPossible(*this, rival))
				return false;
			else
				return true;
		}

		void treasureChest(Chest chest)
		{
			if (chestPossible(chest))
			{
				if (chest.getElem() == Elem::KEY)
					key = true;
				else if (chest.getElem() == Elem::MAX_LIFE)
					health = MAX_LIFE;
				else
					health++;
			}
		}

	private:

		bool key;

		bool chestPossible(Chest chest)
		{
			bool aa = posX == chest.getPosX() && posY == chest.getPosY() + 1;
			bool ab = posX == chest.getPosX() && posY == chest.getPosY() - 1;

			bool ba = posY == chest.getPosY() && posX == chest.getPosX() - 1;
			bool bb = posY == chest.getPosY() && posX == chest.getPosX() + 1;

			bool posA = aa && (a.facing == Direction::WEST);	//Cofre justo a la izquierda del jugador y jugador mirándole
			bool posB = ab && (a.facing == Direction::EAST);	//Cofre justo a la derecha del jugador y jugador mirándole
			bool posC = ba && (a.facing == Direction::NORTH);	//Cofre justo encima del jugador y jugador mirándole
			bool posD = bb && (a.facing == Direction::SOUTH);	//Cofre justo debajo del jugador y jugador mirándole

			return (posA || posB || posC || posD);
		}
};

#endif
