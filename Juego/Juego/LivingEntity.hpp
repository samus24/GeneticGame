#ifndef LIVINGENTITY_HPP
#define LIVINGENTITY_HPP

#include "Entity.hpp"

class LivingEntity : public Entity{
public:
	enum Facing{
		NORTH,
		WEST,
		SOUTH,
		EAST
	};
public:
	LivingEntity(const sf::Texture& texture);

	LivingEntity(const sf::Texture& texture, unsigned int maxHP, unsigned int hp, sf::Vector2f speed, float attack);

	void setHealth(unsigned int hp);

	unsigned int getHealth() const;

	unsigned int increaseHealth(int incr);

	void setSpeed(sf::Vector2f speed);

	sf::Vector2f getSpeed() const;

	sf::Vector2f increaseSpeed(float incr, sf::Time t);

	void setAttack(float attack);

	float getAttack() const;

	Facing getFacing() const;

	unsigned int increaseAttack(int incr, sf::Time t);

	void update(sf::Time dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void setTextureRect(sf::IntRect r);

	sf::IntRect getAttackZone() const;

private:
	unsigned int maxHP;
	unsigned int hp;
	sf::Vector2f speed;
	float attack;
	Facing _facing;

	sf::Time timePerFrame;
	sf::Time speedPuTime;
	float speedIncr;
	sf::Time attackPuTime;
	float attackIncr;

};

#endif