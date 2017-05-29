#ifndef LIVINGENTITY_HPP
#define LIVINGENTITY_HPP

#include "Entity.hpp"

class LivingEntity : protected Entity{
public:
	LivingEntity(const sf::Texture& texture);

	LivingEntity(const sf::Texture& texture, unsigned int hp, unsigned int speed, float attack);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	unsigned int hp;
	unsigned int speed;
	float attack;


};

#endif