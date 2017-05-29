#ifndef POWERUP_HPP
#define POWERUP_HPP

#include "myRandom.hpp"
#include "LivingEntity.hpp"

class PowerUp {
public:
	enum class Type{
		BoostHealth,
		BoostAttack,
		BoostSpeed,

		TypesCount
	};

	PowerUp(){
		_type = Type(myRandom::getRandom(0, int(Type::TypesCount) - 1));
	}

	void applyBoost(LivingEntity e){
		switch (_type){
		case Type::BoostHealth:
			e.increaseHealth(1);
			break;
		case Type::BoostAttack:
			e.increaseAttack(1);
			break;
		case Type::BoostSpeed:
			e.increaseSpeed(1);
			break;
		default:
			break;
		}
	}

private:
	Type _type;
};

#endif