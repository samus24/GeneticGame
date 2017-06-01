#include "GameState.hpp"
#include "ResourceHolder.hpp"
#include "AG.hpp"
#include "Utils.hpp"

GameState::GameState(StateStack& stack, Context context)
	: State(stack, context),
	_playerHasKey(false),
	_player(context.textures->get(Textures::Player), 3, 3, 1, 1)
{
	sf::Texture& texture = context.textures->get(Textures::Title);
	_buttonMenu.setTexture(texture);

	_buttonMenu.setScale(0.4, 0.4);
	_buttonMenu.setPosition(10, 10);

	sf::Texture& texture2 = context.textures->get(Textures::Key);
	_key.setTexture(texture2);
	_key.setTextureRect(LOCKED_KEY);

	_key.setScale(0.6, 0.6);
	_key.setPosition(WINDOW_WIDTH - LOCKED_KEY.width, WINDOW_HEIGHT - LOCKED_KEY.height);

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
	_tiles.load("Media/Textures/TileMap.png", TILESIZE, room.getCells(), room.width, room.height);
	sf::Vector2f origin((WINDOW_WIDTH - (room.width * TILESIZE.x)) / 2.f, (WINDOW_HEIGHT - (room.height * TILESIZE.y)) / 2.f);
	sf::Vector2f center(room.width / 2.f, room.height / 2.f);
	center.x *= TILESIZE.x;
	center.y *= TILESIZE.y;
	_player.setPosition(origin + center);
}

void GameState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
	window.draw(_tiles);
	window.draw(_buttonMenu);
	window.draw(_key);
	window.draw(_player);
}

bool GameState::update(sf::Time)
{
	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::D){
			_dungeon.setSelectedRoom(_dungeon.getSelectedRoom() + 1);
			Dungeon::Matrix room = _dungeon.getRoom(_dungeon.getSelectedRoom());
			_tiles.load("Media/Textures/TileMap.png", TILESIZE, room.getCells(), room.width, room.height);
		}
		else if (event.key.code == sf::Keyboard::A){
			_dungeon.setSelectedRoom(_dungeon.getSelectedRoom() - 1);
			Dungeon::Matrix room = _dungeon.getRoom(_dungeon.getSelectedRoom());
			_tiles.load("Media/Textures/TileMap.png", TILESIZE, room.getCells(), room.width, room.height);
		}
		else if (event.key.code == sf::Keyboard::S){
			_player.move(sf::Vector2f(0, TILESIZE.y));
		}
		else if (event.key.code == sf::Keyboard::W){
			_player.move(sf::Vector2f(0, -int(TILESIZE.y)));
		}
		else if (event.key.code == sf::Keyboard::Q){
			_key.setTextureRect(UNLOCKED_KEY);
		}
	}
	return false;
}