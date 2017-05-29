#include "LivingEntity.hpp"

LivingEntity::LivingEntity(const sf::Texture& texture):
	Entity(texture)
{

}

LivingEntity::LivingEntity(const sf::Texture& texture, unsigned int maxHP, unsigned int hp, unsigned int speed, float attack) :
	Entity(texture),
	maxHP(maxHP),
	hp(hp),
	speed(speed),
	attack(attack)
{

}

void LivingEntity::setHealth(unsigned int hp){
	this->hp = hp;
}

unsigned int LivingEntity::getHealth() const{
	return hp;
}

unsigned int LivingEntity::increaseHealth(int incr){
	int sum = hp + incr;
	sum = std::max(sum, 0);
	hp = std::min(maxHP, unsigned int(sum));
	return hp;
}

void LivingEntity::setSpeed(unsigned int speed){
	this->speed = speed;
}

unsigned int LivingEntity::getSpeed() const{
	return speed;
}

unsigned int LivingEntity::increaseSpeed(int incr){
	int sum = speed + incr;
	speed = std::max(sum, 0);
	return speed;
}

void LivingEntity::setAttack(float attack){
	this->attack = attack;
}

float LivingEntity::getAttack() const{
	return attack;
}

unsigned int LivingEntity::increaseAttack(int incr){
	int sum = attack + incr;
	attack = std::max(sum, 0);
	return attack;
}

void LivingEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(sprite);
}