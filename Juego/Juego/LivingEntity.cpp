#include "LivingEntity.hpp"

LivingEntity::LivingEntity(const sf::Texture& texture):
	Entity(texture)
{

}

LivingEntity::LivingEntity(const sf::Texture& texture, unsigned int maxHP, unsigned int hp, sf::Vector2f speed, float attack) :
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

void LivingEntity::setSpeed(sf::Vector2f speed){
	this->speed = speed;
}

sf::Vector2f LivingEntity::getSpeed() const{
	return speed;
}

sf::Vector2f LivingEntity::increaseSpeed(float incr){
	speed *= incr;
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

void LivingEntity::update(sf::Time dt){
	move(speed * dt.asSeconds());
}

void LivingEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(sprite);
}
