#include "GameState.hpp"
#include "GameOverState.hpp"
#include "PowerUp.hpp"
#include "PauseState.hpp"
#include "LoadingState.hpp"
#include "ResourceHolder.hpp"
#include "Utils.hpp"

GameState::GameState(StateStack& stack, Context context)
	: State(stack, context),
	_playerHasKey(false),
	_isPlayerAttack(false),
	_isPlayerBlocking(false),
	_hasReleasedAttack(true),
	_player(context.textures->get(Textures::Player), MAX_HEALTH, 3, sf::Vector2f(0,0), 1),
	_tpCoolDown(TPCOOLDOWN),
	_damageCoolDown(DAMAGECOOLDOWN)
{
	_debugInfo.setFont(context.fonts->get(Fonts::Main));
	_debugInfo.setFillColor(sf::Color::White);
	_debugInfo.setCharacterSize(12);
	_debugInfo.setPosition(0, WINDOW_HEIGHT*0.8);

	_roomNo.setFont(context.fonts->get(Fonts::Alagard));
	_roomNo.setFillColor(sf::Color::White);
	_roomNo.setCharacterSize(20);
	_roomNo.setPosition(WINDOW_WIDTH*0.9, 0);

	sf::Texture& texture = context.textures->get(Textures::Title);
	_buttonMenu.setTexture(texture);

	_buttonMenu.setScale(0.4, 0.4);
	_buttonMenu.setPosition(10, 10);

	sf::Texture& texture2 = context.textures->get(Textures::Key);
	_key.setTexture(texture2);
	_key.setTextureRect(LOCKED_KEY);

	_key.setScale(0.6, 0.6);
	_key.setPosition(WINDOW_WIDTH - LOCKED_KEY.width, WINDOW_HEIGHT - LOCKED_KEY.height);

	sf::Texture& texture3 = context.textures->get(Textures::PlayerMods);
	_attackBoost.setTexture(texture3);
	_speedBoost.setTexture(texture3);
	_attackBoost.setTextureRect(ATTACK_BOOST);
	_speedBoost.setTextureRect(SPEED_BOOST);
	_attackBoost.scale(2.f, 2.f);
	_speedBoost.scale(2.f, 2.f);
	_attackBoost.setPosition(WINDOW_WIDTH - LOCKED_KEY.width*2, WINDOW_HEIGHT - LOCKED_KEY.height);
	_speedBoost.setPosition(WINDOW_WIDTH - LOCKED_KEY.width*1.5, WINDOW_HEIGHT - LOCKED_KEY.height);
	sf::Vector2f pos(WINDOW_WIDTH - (FULL_HEART.width*1.5 * MAX_HEALTH), WINDOW_HEIGHT - FULL_HEART.height*1.5);
	for (size_t i = 0; i < MAX_HEALTH; ++i){
		sf::Sprite s;
		s.setTexture(texture3);
		s.setTextureRect(FULL_HEART);
		s.scale(1.5, 1.5);
		s.setPosition(pos);
		pos.x += FULL_HEART.width * 1.5;
		_playerHealth.push_back(s);
	}
	updatePlayerHealth();
}

void GameState::loadDungeon(Cromosoma mejor){
	_mejor = mejor;
	_dungeon.generateRooms(_mejor);
	Dungeon::Matrix room = _dungeon.getRoom(_dungeon.getSelectedRoom());
	_tiles.load(TILEPATH, TILESIZE, room, room.width, room.height);
	sf::Vector2f spawnCoords = _tiles.getCoordsFromCell(room.width / 2, room.height / 2);
	_player.setPosition(spawnCoords);
	_roomNo.setString("Room " + std::to_string(_dungeon.getSelectedRoom()));
	_mejor.getMejorCC().getNodos();
	generateEnemies(_mejor.getMejorCC().getNodos()[_dungeon.getSelectedRoom()].getEnemigos());
}

void GameState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
	window.draw(_tiles);
	window.draw(_buttonMenu);
	window.draw(_key);
	if (_player.getAttackPuTime() > sf::Time::Zero){
		window.draw(_attackBoost);
	}
	if (_player.getSpeedPuTime() > sf::Time::Zero){
		window.draw(_speedBoost);
	}
	for (auto e : enemies){
		window.draw(e);
	}

	window.draw(_player);

	for (auto s : _playerHealth){
		window.draw(s);
	}
	window.draw(_roomNo);
	window.draw(_debugInfo);
}

bool GameState::update(sf::Time dt)
{
	if (_player.getHealth() <= 0){
		requestStackPop();
		auto gameOverState = std::make_shared<GameOverState>(*_stack, _context);
		requestStackPush(gameOverState);
	}
	_tpCoolDown -= dt;
	if (_tpCoolDown <= sf::Time::Zero){
		_tpCoolDown = sf::Time::Zero;
	}
	sf::Vector2f pos = _player.getPosition();
	_player.update(dt);
	sf::Vector2f enemyPos;
	auto playerBounds = _player.getBounds();
	auto it = enemies.begin();
	while(it != enemies.end()){
		enemyPos = it->getPosition();
		it->update(dt, _player, _dungeon, _tiles);
		if (playerBounds.intersects(it->getBounds())){
			it->setPosition(enemyPos);
		}
		auto corners = it->getCorners();
		for (auto c : corners){
			auto cell = _tiles.getCellFromCoords(c);
			if (Dungeon::isLocked(_dungeon.getCell(cell))){
				it->setPosition(enemyPos);
				break;
			}
		}
		if (it->_isAttacking){
			auto atZone = it->getAttackZone();
			if (atZone.intersects(_player.getBounds())){
				if (!_isPlayerBlocking){
					auto posP = _player.getCenter();
					auto posE = it->getCenter();
					auto v = posP - posE;
					v /= 2.f;
					_player.move(v);
					_player.increaseHealth(-it->getAttack());
				}
			}
		}
		++it;
	}
	auto playerPos = _tiles.getCellFromCoords(_player.getCenter().x, _player.getCenter().y);
	int portal = _dungeon.getCell(playerPos);
	if (portal >= 0 && _tpCoolDown == sf::Time::Zero){
		int lastRoom = _dungeon.getSelectedRoom();
		_dungeon.setSelectedRoom(portal);
		auto room = _dungeon.getRoom(portal);
		_tiles.load(TILEPATH, TILESIZE, room, room.width, room.height);
		auto teleportCell = _dungeon.getCellWith(lastRoom);
		_player.setPosition(_tiles.getCoordsFromCell(teleportCell));
		_roomNo.setString("Room " + std::to_string(_dungeon.getSelectedRoom()));
		generateEnemies(_mejor.getMejorCC().getNodos()[_dungeon.getSelectedRoom()].getEnemigos());
		_tpCoolDown = TPCOOLDOWN;
	}
	else if (portal == Dungeon::KEYBLOCK){
		_dungeon.setCell(playerPos, Dungeon::EMPTY);
		auto room = _dungeon.getRoom(_dungeon.getSelectedRoom());
		_tiles.load(TILEPATH, TILESIZE, room, room.width, room.height);
		_key.setTextureRect(UNLOCKED_KEY);
		_playerHasKey = true;
	}
	else{
		auto corners = _player.getCorners();
		for (auto c : corners){
			auto cell = _tiles.getCellFromCoords(c);
			if (Dungeon::isLocked(_dungeon.getCell(cell))){
				_player.setPosition(pos);
				_player.setSpeed(sf::Vector2f(0, 0));

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
					sf::Vector2f actualSpeed = _player.getSpeed();
					actualSpeed.x = std::max(NORMALSPEED, std::abs(actualSpeed.x));
					if (actualSpeed != _player.getSpeed()) _player.setSpeed(actualSpeed);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
					sf::Vector2f actualSpeed = _player.getSpeed();
					actualSpeed.x = std::min(-NORMALSPEED, -std::abs(actualSpeed.x));
					if (actualSpeed != _player.getSpeed()) _player.setSpeed(actualSpeed);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
					sf::Vector2f actualSpeed = _player.getSpeed();
					actualSpeed.y = std::max(NORMALSPEED, std::abs(actualSpeed.y));
					if (actualSpeed != _player.getSpeed()) _player.setSpeed(actualSpeed);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
					sf::Vector2f actualSpeed = _player.getSpeed();
					actualSpeed.y = std::min(-NORMALSPEED, -std::abs(actualSpeed.y));
					if (actualSpeed != _player.getSpeed()) _player.setSpeed(actualSpeed);
				}

				break;
			}
		}
	}
	if (portal == Dungeon::ENDPORTAL){
		_damageCoolDown -= dt;
		if (_playerHasKey){
			requestStackPop();
			auto loadState = std::make_shared<LoadingState>(*_stack, _context);
			unsigned int rand = myRandom::getRandom(0u, LOADINGMSG.size() - 1);
			loadState->launch(LOADINGMSG[rand]);
			requestStackPush(loadState);
		}
		else if (_damageCoolDown <= sf::Time::Zero){
			_player.increaseHealth(-1);
			updatePlayerHealth();
			_damageCoolDown = DAMAGECOOLDOWN;
		}
	}

	if (_isPlayerAttack){
		switch (_player.getFacing()){
		case LivingEntity::Facing::NORTH:
			playerPos.y -= 1;
			_player.setTextureRect(ATTACKUP);
			break;
		case LivingEntity::Facing::WEST:
			playerPos.x -= 1;
			_player.setTextureRect(ATTACKLEFT);
			break;
		case LivingEntity::Facing::SOUTH:
			playerPos.y += 1;
			_player.setTextureRect(ATTACKDOWN);
			break;
		case LivingEntity::Facing::EAST:
			playerPos.x += 1;
			_player.setTextureRect(ATTACKRIGHT);
			break;
		}
		portal = _dungeon.getCell(playerPos);
		if (portal == Dungeon::CLOSED_CHEST){
			_dungeon.setCell(playerPos, Dungeon::OPENED_CHEST);
			PowerUp pu;
			pu.applyBoost(_player);
			updatePlayerHealth();
			auto room = _dungeon.getRoom(_dungeon.getSelectedRoom());
			_tiles.load(TILEPATH, TILESIZE, room, room.width, room.height);
		}
		else{
			sf::IntRect attackZone = _player.getAttackZone();
			auto it = enemies.begin();
			while (it != enemies.end()){
				if (attackZone.intersects(it->getBounds()) && !it->_isBlocking){
					auto posP = _player.getCenter();
					auto posE = it->getCenter();
					auto v = posE - posP;
					v /= 2.f;
					it->move(v);
					auto corners = it->getCorners();
					for (auto c : corners){
						auto cell = _tiles.getCellFromCoords(c);
						if (Dungeon::isLocked(_dungeon.getCell(cell))){
							v = -v;
							it->move(v);
							break;
						}
					}

					if (it->increaseHealth(-_player.getAttack()) <= 0){
						it = enemies.erase(it);
					}
					else{
						++it;
					}
				}
				else{
					++it;
				}
			}
		}
		_isPlayerAttack = false;
	}
	
	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		sf::Vector2u cell;
		if (event.key.code == sf::Keyboard::D && !_isPlayerBlocking){
			sf::Vector2f actualSpeed = _player.getSpeed();
			actualSpeed.x = std::max(NORMALSPEED, std::abs(actualSpeed.x));
			if (actualSpeed != _player.getSpeed()) _player.setSpeed(actualSpeed);
		}
		else if (event.key.code == sf::Keyboard::A && !_isPlayerBlocking){
			sf::Vector2f actualSpeed = _player.getSpeed();
			actualSpeed.x = std::min(-NORMALSPEED, -std::abs(actualSpeed.x));
			if (actualSpeed != _player.getSpeed()) _player.setSpeed(actualSpeed);
		}
		else if (event.key.code == sf::Keyboard::S && !_isPlayerBlocking){
			sf::Vector2f actualSpeed = _player.getSpeed();
			actualSpeed.y = std::max(NORMALSPEED, std::abs(actualSpeed.y));
			if (actualSpeed != _player.getSpeed()) _player.setSpeed(actualSpeed);
		}
		else if (event.key.code == sf::Keyboard::W && !_isPlayerBlocking){
			sf::Vector2f actualSpeed = _player.getSpeed();
			actualSpeed.y = std::min(-NORMALSPEED, -std::abs(actualSpeed.y));
			if (actualSpeed != _player.getSpeed()) _player.setSpeed(actualSpeed);
		}
		else if (event.key.code == sf::Keyboard::Escape){
			auto pauseState = std::make_shared<PauseState>(*_stack, _context);
			requestStackPush(pauseState);
		}
		else if (event.key.code == sf::Keyboard::K && _hasReleasedAttack && !_isPlayerBlocking){
			_isPlayerAttack = true;
			_hasReleasedAttack = false;
		}
		else if (event.key.code == sf::Keyboard::L && _hasReleasedAttack){
			_isPlayerBlocking = true;
			_player.setSpriteColor(sf::Color::Cyan);
		}
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		if ((event.key.code == sf::Keyboard::D) || (event.key.code == sf::Keyboard::A)){
			sf::Vector2f actualSpeed = _player.getSpeed();
			actualSpeed.x = 0;
			_player.setSpeed(actualSpeed);
		}
		else if ((event.key.code == sf::Keyboard::S) || (event.key.code == sf::Keyboard::W)){
			sf::Vector2f actualSpeed = _player.getSpeed();
			actualSpeed.y = 0;
			_player.setSpeed(actualSpeed);
		}
		else if (event.key.code == sf::Keyboard::K){
			_hasReleasedAttack = true;
			_player.setSpeed(_player.getSpeed());
		}
		else if (event.key.code == sf::Keyboard::L){
			_isPlayerBlocking = false;
			_player.setSpriteColor(sf::Color::White);
		}
	}
	return false;
}

void GameState::updatePlayerHealth(){
	for (size_t i = 0; i < MAX_HEALTH; ++i){
		if (i < _player.getHealth()){
			_playerHealth[i].setTextureRect(FULL_HEART);
		}
		else{
			_playerHealth[i].setTextureRect(EMPTY_HEART);
		}
	}
}

void GameState::generateEnemies(int nEnemies){
	enemies.clear();
	auto room = _dungeon.getRoom(_dungeon.getSelectedRoom());
	int x, y;
	for (size_t i = 0; i < nEnemies; ++i){
		do{
			x = myRandom::getRandom(1u, room.width - 2);
			y = myRandom::getRandom(1u, room.height - 2);
		} while (Dungeon::isLocked(room[x][y]));
		enemies.emplace_back(getContext().textures->get(Textures::Enemy), 3, 3, sf::Vector2f(0, 0), 1);
		enemies.at(i).setPosition(_tiles.getCoordsFromCell(x, y));
	}
}