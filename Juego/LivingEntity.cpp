#include "LivingEntity.hpp"

LivingEntity::LivingEntity(const sf::Texture& texture):
	Entity(texture)
{

}

LivingEntity::LivingEntity(const sf::Texture& texture, unsigned int hp, unsigned int speed, float attack):
	Entity(texture),
	hp(hp),
	speed(speed),
	attack(attack)
{

}