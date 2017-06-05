#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "State.hpp"
#include "Dungeon.hpp"
#include "TileMap.hpp"
#include "LivingEntity.hpp"
#include "Enemy.hpp"

class GameState : public State {
public:
	GameState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);

	void loadDungeon(Cromosoma mejor);

private:
	void updatePlayerHealth();

	void generateEnemies(int nEnemies);

	Cromosoma _mejor;
	Dungeon _dungeon;
	TileMap _tiles;
	LivingEntity _player;

	std::vector<Enemy> enemies;

	sf::Sprite _buttonMenu;
	bool _playerHasKey;
	bool _isPlayerAttack;
	bool _hasReleasedAttack;
	bool _isPlayerBlocking;
	sf::Sprite _key;
	sf::Sprite _attackBoost;
	sf::Sprite _speedBoost;
	sf::Text _roomNo;

	std::vector<sf::Sprite> _playerHealth;

	sf::Text _debugInfo;
	sf::Time _tpCoolDown;

	sf::Time _damageCoolDown;
};

#endif