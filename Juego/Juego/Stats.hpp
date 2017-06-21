#ifndef STATS_HPP
#define STATS_HPP

#include "SFML\Graphics.hpp"

struct Stats{
	Stats(){
		rawPoints = 0;
		killedEnemies = 0;
		dungeonsCompleted = 0;
		timeAlive = sf::Time::Zero;
	}

	unsigned int rawPoints;
	unsigned int killedEnemies;
	unsigned int dungeonsCompleted;
	sf::Time timeAlive;
};

#endif