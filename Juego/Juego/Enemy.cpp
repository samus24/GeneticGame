#include "Enemy.hpp"

Enemy::Enemy(const sf::Texture& texture):
	LivingEntity(texture),
	_playerDetected(false),
	_isAtacking(false),
	_isBlocking(false),
	_decision(sf::seconds(5)),
	_impervious(IMPERVIOUSTIME)
{
	_ai = std::bind(&Enemy::AI1, this, std::placeholders::_1);
}

Enemy::Enemy(const sf::Texture& texture, unsigned int maxHP, unsigned int hp, sf::Vector2f speed, float attack) :
	LivingEntity(texture, maxHP, hp, speed, attack),
	_playerDetected(false),
	_isAtacking(false),
	_isBlocking(false),
	_decision(sf::seconds(3)),
	_impervious(IMPERVIOUSTIME)
{
	_ai = std::bind(&Enemy::AI1, this, std::placeholders::_1);
}

void Enemy::update(sf::Time dt, LivingEntity player){
	_decision -= dt;
	if (_decision <= sf::Time::Zero){
		_ai(player);
		_decision = DECISIONTIME;
	}
	LivingEntity::update(dt);
}

void Enemy::setAI(std::function<void(LivingEntity)> ai){
	_ai = ai;
}