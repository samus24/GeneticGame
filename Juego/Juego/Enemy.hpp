#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <functional>
#include <memory>

#include "LivingEntity.hpp"

class Enemy : public LivingEntity{
public:
	void AI1(LivingEntity player){
		auto posP = player.getCenter();
		auto posE = getCenter();
		auto v = posP - posE;
		if (v.x > 0) v.x = NORMALSPEED;
		else if (v.x < 0) v.x = -NORMALSPEED;
		if (v.y > 0) v.y = NORMALSPEED;
		else if (v.y < 0) v.y = -NORMALSPEED;
		setSpeed(v);
	}

	void AI2(LivingEntity player){

	}

	void AI3(LivingEntity player){

	}

public:
	Enemy(const sf::Texture& texture);

	Enemy(const sf::Texture& texture, unsigned int maxHP, unsigned int hp, sf::Vector2f speed, float attack);

	virtual void update(sf::Time dt, LivingEntity player);

	void setAI(std::function<void(LivingEntity)> ai);
	
private:
	bool _playerDetected;
	bool _isAtacking;
	bool _isBlocking;

	sf::Time _impervious;
	sf::Time _decision;

	std::function<void(LivingEntity)> _ai;

};

#endif