#ifndef LIVINGENTITY_HPP
#define LIVINGENTITY_HPP

#include "Entity.hpp"

class LivingEntity : protected Entity{
public:
	LivingEntity(const sf::Texture& texture);

	LivingEntity(const sf::Texture& texture, unsigned int maxHP, unsigned int hp, unsigned int speed, float attack);

	void setHealth(unsigned int hp);

	unsigned int getHealth() const;

	unsigned int increaseHealth(int incr);

	void setSpeed(unsigned int speed);

	unsigned int getSpeed() const;

	unsigned int increaseSpeed(int incr);

	void setAttack(float attack);

	float getAttack() const;

	unsigned int increaseAttack(int incr);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	unsigned int maxHP;
	unsigned int hp;
	unsigned int speed;
	float attack;


};

#endif