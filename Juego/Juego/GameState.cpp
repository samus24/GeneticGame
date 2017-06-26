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
	_player(context.textures->get(Textures::Player), MAX_HEALTH, MAX_HEALTH, sf::Vector2f(0, 0), 1),
	_tpCoolDown(TPCOOLDOWN),
	_damageCoolDown(DAMAGECOOLDOWN),
	_helpTime(sf::Time::Zero),
	_portalHelp(true),
	_healthHelp(true),
	_endPortalHelp(true)
{
	context.musics->get(Musics::Game).play();

	sf::SoundBuffer& bufferSword = context.sounds->get(Sounds::Sword);
	_swordSound.setBuffer(bufferSword);
	sf::SoundBuffer& bufferPortal = context.sounds->get(Sounds::Portal);
	_portalSound.setBuffer(bufferPortal);
	sf::SoundBuffer& bufferKey = context.sounds->get(Sounds::Key);
	_keySound.setBuffer(bufferKey);
	sf::SoundBuffer& bufferHurt = context.sounds->get(Sounds::Hurt);
	_hurtSound.setBuffer(bufferHurt);
	sf::SoundBuffer& bufferFireball = context.sounds->get(Sounds::Fireball);
	_fireballSound.setBuffer(bufferFireball);
	sf::SoundBuffer& bufferChest = context.sounds->get(Sounds::Chest);
	_chestSound.setBuffer(bufferChest);

	_roomNo.setFont(context.fonts->get(Fonts::Alagard));
	_roomNo.setFillColor(sf::Color::White);
	_roomNo.setCharacterSize(20);
	_roomNo.setPosition(WINDOW_WIDTH*0.9, 0);

	sf::Texture& texture = context.textures->get(Textures::Title);
	_buttonMenu.setTexture(texture);
	_buttonMenu.setScale(0.4, 0.4);
	_buttonMenu.setPosition(10, 10);

	_statsText.setFont(context.fonts->get(Fonts::Alagard));
	_statsText.setFillColor(sf::Color::White);
	_statsText.setCharacterSize(20);
	_statsText.setPosition(200, 10);

	sf::Texture& textureControls = context.textures->get(Textures::Controls);
	_controls.setTexture(textureControls);
	_controls.setPosition(10, WINDOW_HEIGHT - textureControls.getSize().y);

	_controlsText.setFont(context.fonts->get(Fonts::Alagard));
	_controlsText.setFillColor(sf::Color::White);
	_controlsText.setCharacterSize(15);
	_controlsText.setPosition(textureControls.getSize().x + 10, _controls.getPosition().y + 10);
	_controlsText.setString("Attack\\Open Chest\n\nBlock");

	_helpInfo.setFont(context.fonts->get(Fonts::Alagard));
	_helpInfo.setFillColor(sf::Color::White);
	_helpInfo.setCharacterSize(15);
	sf::Vector2f helpPos(_controlsText.getPosition().x + _controlsText.getLocalBounds().width - 50, WINDOW_HEIGHT - 20);
	_helpInfo.setPosition(helpPos);

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
	window.draw(_statsText);
	window.draw(_controls);
	window.draw(_controlsText);
	window.draw(_helpInfo);
}

bool GameState::update(sf::Time dt)
{
	_helpTime -= dt;
	if (_helpTime >= sf::Time::Zero){
		sf::Color helpColor = _helpInfo.getFillColor();
		helpColor.a = 255 * (_helpTime.asSeconds() / HELPTIME.asSeconds());
		_helpInfo.setFillColor(helpColor);
	}
	if (!_isPlayerBlocking)
		_player.setSpriteColor(sf::Color::White);
	_playerStats.timeAlive += dt;
	if (_player.getHealth() <= 0){
		getContext().musics->get(Musics::Game).stop();
		requestStackPop();
		auto gameOverState = std::make_shared<GameOverState>(*_stack, _context);
		gameOverState->setPlayerStats(_playerStats);
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
			if(_fireballSound.getStatus() == sf::Sound::Status::Stopped)_fireballSound.play();
			if (atZone.intersects(_player.getBounds())){
				if (!_isPlayerBlocking){
					auto posP = _player.getCenter();
					auto posE = it->getCenter();
					auto v = posP - posE;
					v /= 3.f;
					_player.move(v);
					_player.increaseHealth(-it->getAttack());
					_hurtSound.play();
					updatePlayerHealth();
					if (_healthHelp){
						_helpInfo.setString("Caution! If you receive enough damage you'll die.");
						_helpTime = HELPTIME;
						_healthHelp = false;
					}
				}
			}
		}
		++it;
	}
	auto playerPos = _tiles.getCellFromCoords(_player.getCenter().x, _player.getCenter().y);
	int portal = _dungeon.getCell(playerPos);
	if (portal >= 0 && _tpCoolDown == sf::Time::Zero){
		if (enemies.size() <= 0){
			_portalSound.play();
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
		else{
			if (_portalHelp){
				_helpInfo.setString("You must defeat your enemies in this room to continue.");
				_helpTime = HELPTIME;
				_portalHelp = false;
			}
		}
	}
	else if (portal == Dungeon::KEYBLOCK){
		_helpInfo.setString("You have the key. Now, find the exit portal!");
		_helpTime = HELPTIME;
		_keySound.play();
		_dungeon.setCell(playerPos, Dungeon::EMPTY);
		auto room = _dungeon.getRoom(_dungeon.getSelectedRoom());
		_tiles.load(TILEPATH, TILESIZE, room, room.width, room.height);
		_key.setTextureRect(UNLOCKED_KEY);
		_playerStats.rawPoints += KEYPOINTS;
		_playerHasKey = true;
	}
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
	if (portal == Dungeon::ENDPORTAL){
		_damageCoolDown -= dt;
		if (_playerHasKey){
			_playerStats.rawPoints += DUNGEONPOINTS;
			_playerStats.dungeonsCompleted++;
			_portalSound.play();
			getContext().musics->get(Musics::Game).stop();
			requestStackPop();
			auto loadState = std::make_shared<LoadingState>(*_stack, _context);
			unsigned int rand = myRandom::getRandom(0u, LOADINGMSG.size() - 1);
			loadState->setPlayerStats(_playerStats);
			loadState->launch(LOADINGMSG[rand]);
			requestStackPush(loadState);
		}
		else if (_damageCoolDown <= sf::Time::Zero){
			if (_endPortalHelp){
				_helpInfo.setString("You must find the key to cross this portal");
				_helpTime = HELPTIME;
				_endPortalHelp = false;
			}
			_hurtSound.play();
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
			_chestSound.play();
			_playerStats.rawPoints += CHESTPOINTS;
			_dungeon.setCell(playerPos, Dungeon::OPENED_CHEST);
			PowerUp pu;
			pu.applyBoost(_player);
			updatePlayerHealth();
			auto room = _dungeon.getRoom(_dungeon.getSelectedRoom());
			_tiles.load(TILEPATH, TILESIZE, room, room.width, room.height);
		}
		else{
			_swordSound.play();
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
					_hurtSound.play();
					if (it->increaseHealth(-_player.getAttack()) <= 0){
						_playerStats.rawPoints += KILLPOINTS;
						_playerStats.killedEnemies++;
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
	std::string s = "Points: " + std::to_string(_playerStats.rawPoints) + "\t";
	s += "Kills: " + std::to_string(_playerStats.killedEnemies) + "\t";
	s += "Dungeons: " + std::to_string(_playerStats.dungeonsCompleted) + "\t";
	s += "Alive: " + std::to_string((int)_playerStats.timeAlive.asSeconds()) + "s\t";

	_statsText.setString(s);
	
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
			getContext().musics->get(Musics::Game).pause();
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
		enemies.emplace_back(getContext().textures->get(Textures::Enemy), getContext().textures->get(Textures::PlayerMods), 3, 3, sf::Vector2f(0, 0), 1);
		enemies.at(i).setPosition(_tiles.getCoordsFromCell(x, y));
	}
}

void GameState::setPlayerStats(Stats s){
	_playerStats = s;
}

Stats GameState::getPlayerStats() const{
	return _playerStats;
}