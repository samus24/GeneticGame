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
	attack(attack),
	timePerFrame(TIMEPERFRAME),
	_facing(Facing::SOUTH),
	speedPuTime(sf::Time::Zero),
	attackPuTime(sf::Time::Zero),
	speedIncr(1),
	attackIncr(0)
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
	if(std::abs(speed.x) <= NORMALSPEED && std::abs(speed.y) <= NORMALSPEED) this->speed *= speedIncr;
	if (speed.x > 0){
		_facing = Facing::EAST;
		sprite.setTextureRect(RIGHTRECT);
	}
	else if (speed.x < 0){
		_facing = Facing::WEST;
		sprite.setTextureRect(LEFTRECT);
	}
	else if (speed.y > 0){
		_facing = Facing::SOUTH;
		sprite.setTextureRect(DOWNRECT);
	}
	else if (speed.y < 0){
		_facing = Facing::NORTH;
		sprite.setTextureRect(UPRECT);
	}
}

LivingEntity::Facing LivingEntity::getFacing() const{
	return _facing;
}

sf::Vector2f LivingEntity::getSpeed() const{
	return speed;
}

sf::Vector2f LivingEntity::increaseSpeed(float incr, sf::Time t){
	speed *= incr;
	speedIncr = incr;
	if (speed.x > 0){
		_facing = Facing::EAST;
		sprite.setTextureRect(RIGHTRECT);
	}
	else if (speed.x < 0){
		_facing = Facing::WEST;
		sprite.setTextureRect(LEFTRECT);
	}
	else if (speed.y > 0){
		_facing = Facing::SOUTH;
		sprite.setTextureRect(DOWNRECT);
	}
	else if (speed.y < 0){
		_facing = Facing::NORTH;
		sprite.setTextureRect(UPRECT);
	}
	speedPuTime = t;
	return speed;
}

void LivingEntity::setAttack(float attack){
	this->attack = attack;
}

float LivingEntity::getAttack() const{
	return attack;
}

unsigned int LivingEntity::increaseAttack(int incr, sf::Time t){
	int sum = attack + incr;
	attack = std::max(sum, 0);
	attackPuTime = t;
	attackIncr = incr;
	return attack;
}

void LivingEntity::update(sf::Time dt){
	timePerFrame -= dt;
	if (speedPuTime > sf::Time::Zero) speedPuTime -= dt;
	attackPuTime -= dt;
	if (speed != sf::Vector2f(0,0) && timePerFrame <= sf::Time::Zero){
		auto r = sprite.getTextureRect();
		r.left += TILESIZE.x;
		if (r.left >= TILESIZE.x * NSPRITES) r.left = 0;
		sprite.setTextureRect(r);
		timePerFrame = TIMEPERFRAME;
	}
	move(speed * dt.asSeconds());
	if (speedPuTime < sf::Time::Zero){
		setSpeed(getSpeed() / speedIncr);
		speedIncr = 1;
		speedPuTime = sf::Time::Zero;
	}
	if (attackPuTime <= sf::Time::Zero){
		setAttack(getAttack() - attackIncr);
		attackIncr = 0;
	}
}

void LivingEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();
	target.draw(sprite);
}

void LivingEntity::setTextureRect(sf::IntRect r){
	sprite.setTextureRect(r);
}

sf::IntRect LivingEntity::getAttackZone() const{
	sf::IntRect bounds = getBounds();
	switch (_facing){
	case Facing::NORTH:
		bounds.top -= TILESIZE.y;
		bounds.left -= TILESIZE.x / 2.f;
		bounds.width += TILESIZE.x;
		return bounds;
	case Facing::WEST:
		bounds.top -= TILESIZE.y /2.f;
		bounds.height += TILESIZE.y;
		bounds.left -= TILESIZE.x;
		return bounds;
	case Facing::SOUTH:
		bounds.top += TILESIZE.y;
		bounds.left -= TILESIZE.x / 2.f;
		bounds.width += TILESIZE.x;
		return bounds;
	case Facing::EAST:
		bounds.top -= TILESIZE.y / 2.f;
		bounds.height += TILESIZE.y;
		bounds.left += TILESIZE.x;
		return bounds;
	}
}

void LivingEntity::setSpriteColor(sf::Color c){
	sprite.setColor(c);
}

sf::Time LivingEntity::getSpeedPuTime() const{
	return speedPuTime;
}

sf::Time LivingEntity::getAttackPuTime() const{
	return attackPuTime;
}