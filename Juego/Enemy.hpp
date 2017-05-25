#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>
#include "Entity.hpp"

class Enemy: public Entity
{
	private:


	public:

		Enemy()
		{}

		Enemy(const std::string &image)
		{
			Entity::Entity(image);
			health = 3;
		}

		bool attack(Entity rival)
		{
			
		}

		void cure()
		{
			health++;
		}
};

#endif
