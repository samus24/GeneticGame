#include "GameState.hpp"
#include "PauseState.hpp"
#include "ResourceHolder.hpp"
#include "AG.hpp"
#include "Utils.hpp"

GameState::GameState(StateStack& stack, Context context)
	: State(stack, context),
	_playerHasKey(false),
	_player(context.textures->get(Textures::Player), 3, 3, sf::Vector2f(0,0), 1),
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
	sf::Vector2f pos(WINDOW_WIDTH - (FULL_HEART.width*1.5 * _player.getHealth()), WINDOW_HEIGHT - FULL_HEART.height*1.5);
	for (size_t i = 0; i < _player.getHealth(); ++i){
		sf::Sprite s;
		s.setTexture(texture3);
		s.setTextureRect(FULL_HEART);
		s.scale(1.5, 1.5);
		s.setPosition(pos);
		pos.x += FULL_HEART.width * 1.5;
		_playerHealth.push_back(s);
	}

	std::cout << "Arrancando" << std::endl;
	Parametros p;
	p.tamPob = 30;							// Tamano de la poblacion
	p.iteraciones = 30;						// Numero maximo de generaciones
	p.minNodos = 10;						// Numero minimo de nodos iniciales
	p.maxNodos = 40;						// Numero maximo de nodos iniciales
	p.densidad = 0.03;						// Densidad de aristas inciales
	p.elitismo = false;						// Elitismo (guarda a los mejores)
	p.bloating = true;						// Bloating	(evita grafos demasiado grandes)
	p.contractividad = false;				// Contractividad (descarta generaciones malas)
	p.probCruce = 0.6;						// Probabilidad de cruce
	p.probMutacion = 0.02;					// Probabilidad de mutacion
	p.seleccion = new SeleccionTorneo();	// Metodo de seleccion (Ver "MetodoSeleccion.hpp")
	p.cruce = new CruceMonopunto();			// Metodo de cruce (Ver "MetodoCruce.hpp")
	p.mutacion = new MutacionCombinada();		// Metodo de mutacion (Ver "MetodoMutacion.hpp")
	AG ag(p);
	Cromosoma mejor = ag.ejecuta();
	std::cout << "Terminado" << std::endl;
	_dungeon.generateRooms(mejor);
	Dungeon::Matrix room = _dungeon.getRoom(_dungeon.getSelectedRoom());
	_tiles.load(TILEPATH, TILESIZE, room.getCells(), room.width, room.height);
	sf::Vector2f spawnCoords = _tiles.getCoordsFromCell(room.width / 2, room.height / 2);
	_player.setPosition(spawnCoords);
	_roomNo.setString("Room " + std::to_string(_dungeon.getSelectedRoom()));
}

void GameState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
	window.draw(_tiles);
	window.draw(_buttonMenu);
	window.draw(_key);
	window.draw(_player);

	for (auto s : _playerHealth){
		window.draw(s);
	}
	window.draw(_roomNo);
	window.draw(_debugInfo);
}

bool GameState::update(sf::Time dt)
{
	auto playerPos = _tiles.getCellFromCoords(_player.getCenter().x, _player.getCenter().y);
	int portal = _dungeon.getCell(playerPos);
	if (portal >= 0 && _tpCoolDown == sf::Time::Zero){
		int lastRoom = _dungeon.getSelectedRoom();
		_dungeon.setSelectedRoom(portal);
		auto room = _dungeon.getRoom(portal);
		_tiles.load(TILEPATH, TILESIZE, room.getCells(), room.width, room.height);
		auto teleportCell = _dungeon.getCellWith(lastRoom);
		_player.setPosition(_tiles.getCoordsFromCell(teleportCell));
		_roomNo.setString("Room " + std::to_string(_dungeon.getSelectedRoom()));
		_tpCoolDown = TPCOOLDOWN;
	}
	else if (portal == Dungeon::KEYBLOCK){
		_dungeon.setCell(playerPos, Dungeon::EMPTY);
		_playerHasKey = true;
	}
	else if (portal == Dungeon::ENDPORTAL){
		_damageCoolDown -= dt;
		if (_playerHasKey){
			// Load a new dungeon
		}
		else if(_damageCoolDown <= sf::Time::Zero){
			_player.increaseHealth(-1);
			_damageCoolDown = DAMAGECOOLDOWN;
		}
	}
	else{
		_tpCoolDown -= dt;
		if (_tpCoolDown <= sf::Time::Zero){
			_tpCoolDown = sf::Time::Zero;
		}
		sf::Vector2f pos = _player.getPosition();
		_player.update(dt);
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
	
	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		sf::Vector2u cell;
		if (event.key.code == sf::Keyboard::D){
			sf::Vector2f actualSpeed = _player.getSpeed();
			actualSpeed.x = std::max(NORMALSPEED, std::abs(actualSpeed.x));
			if (actualSpeed != _player.getSpeed()) _player.setSpeed(actualSpeed);
		}
		else if (event.key.code == sf::Keyboard::A){
			sf::Vector2f actualSpeed = _player.getSpeed();
			actualSpeed.x = std::min(-NORMALSPEED, -std::abs(actualSpeed.x));
			if (actualSpeed != _player.getSpeed()) _player.setSpeed(actualSpeed);
		}
		else if (event.key.code == sf::Keyboard::S){
			sf::Vector2f actualSpeed = _player.getSpeed();
			actualSpeed.y = std::max(NORMALSPEED, std::abs(actualSpeed.y));
			if (actualSpeed != _player.getSpeed()) _player.setSpeed(actualSpeed);
		}
		else if (event.key.code == sf::Keyboard::W){
			sf::Vector2f actualSpeed = _player.getSpeed();
			actualSpeed.y = std::min(-NORMALSPEED, -std::abs(actualSpeed.y));
			if (actualSpeed != _player.getSpeed()) _player.setSpeed(actualSpeed);
		}
		else if (event.key.code == sf::Keyboard::Escape){
			auto pauseState = std::make_shared<PauseState>(*_stack, _context);
			requestStackPush(pauseState);
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
	}
	return false;
}