#include "GameState.hpp"
#include "ResourceHolder.hpp"
#include "AG.hpp"
#include "Utils.hpp"

GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
	//_player(context.textures->get(Textures::Player), 3, 3, 1, 1)
{
	std::cout << "Arrancando" << std::endl;
	Parametros p;
	p.tamPob = 30;							// Tamano de la poblacion
	p.iteraciones = 60;						// Numero maximo de generaciones
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
	std::cout << mejor.getRolesSala()[RolSala::Inicio] << std::endl;
	_dungeon.generateRooms(mejor);
	std::cout << _dungeon.getSelectedRoom() << std::endl;
	Dungeon::Matrix room = _dungeon.getRoom(_dungeon.getSelectedRoom());
	_tiles.load("Media/Textures/TileMap.png", TILESIZE, room.getCells(), room.width, room.height);

}

void GameState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
	window.draw(_tiles);
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
		
	}
	return false;
}