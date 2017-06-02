#ifndef LIVINGENTITY_HPP
#define LIVINGENTITY_HPP

#include "Entity.hpp"

class LivingEntity : public Entity{
public:
	LivingEntity(const sf::Texture& texture);

	LivingEntity(const sf::Texture& texture, unsigned int maxHP, unsigned int hp, sf::Vector2f speed, float attack);

	void setHealth(unsigned int hp);

	unsigned int getHealth() const;

	unsigned int increaseHealth(int incr);

	void setSpeed(sf::Vector2f speed);

	sf::Vector2f getSpeed() const;

	sf::Vector2f increaseSpeed(float incr);

	void setAttack(float attack);

	float getAttack() const;

	unsigned int increaseAttack(int incr);

	void update(sf::Time dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	unsigned int maxHP;
	unsigned int hp;
	sf::Vector2f speed;
	float attack;

	sf::IntRect _upRect;
	sf::IntRect _downRect;
	sf::IntRect _leftRect;
	sf::IntRect _rightRect;

};

#endif