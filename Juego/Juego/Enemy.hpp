#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <functional>
#include <memory>
#include <queue>

#include "LivingEntity.hpp"
#include "Dungeon.hpp"
#include "TileMap.hpp"

class Enemy : public LivingEntity{
private:
	enum Actions{
		ChangeState,
		Forward,
		Backward,
		Right,
		Left,
		Advance,
		Attack,
		Block,
		Heal
	};
public:
	bool _isAttacking;
	bool _isBlocking;

	Enemy(const sf::Texture& texture, const sf::Texture& attackTexture);

	Enemy(const sf::Texture& texture, const sf::Texture& attackTexture, unsigned int maxHP, unsigned int hp, sf::Vector2f speed, float attack);

	virtual void update(sf::Time dt, LivingEntity player, const Dungeon &dungeon, const TileMap &tiles);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual unsigned int increaseHealth(int incr);

	void setAI(unsigned int ai);

	void ai(LivingEntity player, const Dungeon &dungeon, const TileMap &tiles);

private:
	bool isPlayerInFront(LivingEntity player, const Dungeon &dungeon, const TileMap &tiles) const;

	bool isBlocked(const Dungeon &dungeon, const TileMap &tiles) const;

	bool isPlayerInRange(LivingEntity player, const Dungeon &dungeon, const TileMap &tiles) const;

	void changeState();

	void attack();

	void block();

	void heal();

	void forward();

	void backward();

	void turnLeft();

	void turnRight();

	void getCloserTo(LivingEntity player, const Dungeon &dungeon, const TileMap &tiles);
	
	void executeAction(Actions a, LivingEntity player, const Dungeon &dungeon, const TileMap &tiles);
private:

	bool _playerDetected;

	sf::Time _impervious;
	sf::Time _decision;
	sf::Time _heal;
	sf::Time _attack;

	sf::Sprite _attackBall;

	unsigned int _ai;

	std::queue<Actions> _actions;

};

class Coord{
public:
	unsigned int x;
	unsigned int y;

	Coord(){
		x = 0;
		y = 0;
	}

	Coord(sf::Vector2u p){
		this->x = p.x;
		this->y = p.y;
	}

	Coord(unsigned int x, unsigned int y){
		this->x = x;
		this->y = y;
	}

	bool operator== (const Coord& other) const{
		return (this->x == other.x && this->y == other.y);
	}
	bool operator!= (const Coord& other) const{
		return !(*this == other);
	}

	double distancia(Coord dest){
		return std::sqrt((x - dest.x)*(x - dest.x) + (y - dest.y)*(y - dest.y));
	}
};

class CoordHash{
public:
	size_t operator()(const Coord & key) const // <-- don't forget const
	{
		size_t hashVal = 0;
		hashVal += (key.x * 1000);
		hashVal += key.y;
		return hashVal;
	}
};

class Casilla{
public:
	double dist;
	Coord pos;

	Casilla(double dist, Coord pos) :
		pos(pos)
	{
		this->dist = dist;
	}
};

class Compare{
public:
	bool operator()(Casilla a, Casilla b) const{
		return a.dist > b.dist;
	}
};


#endif