#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "State.hpp"
#include "Dungeon.hpp"
#include "TileMap.hpp"
#include "LivingEntity.hpp"

class GameState : public State {
public:
	GameState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);

private:
	Dungeon _dungeon;
	TileMap _tiles;
	//LivingEntity _player;
};

#endif