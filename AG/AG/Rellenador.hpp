#ifndef RELLENADOR_HPP
#define RELLENADOR_HPP

#include "Mazmorra.hpp"

class Rellenador {
public:
	// Considerar hacer esta clase Singleton
	Rellenador(){

	}

	Mazmorra rellenaMazmorra(Grafo<Gen> grafo){
		// La entrada de este metodo se considera la MEJOR COMPONENTE CONEXA
		Mazmorra m;
		std::unordered_map< unsigned int, Gen > salas = grafo.getNodos();
		std::unordered_map< unsigned int, std::set<unsigned int> > uniones = grafo.getAdyacencia();
		auto itSalas = salas.begin();
		while (itSalas != salas.cend()){
			Sala s(itSalas->first, itSalas->second.getAncho(), itSalas->second.getAlto());	// Las salas se crean vacias
			unsigned int nUniones = uniones[itSalas->first].size();
			auto itUniones = uniones[itSalas->first].begin();
			while (itUniones != uniones[itSalas->first].cend()){
				
				++itUniones;
			}
			++itSalas;
		}
	}
private:
	std::vector<Pair<int, int>> posicionesPreferidas(unsigned int ancho, unsigned int alto, unsigned int nUniones){
		std::vector<Pair<int, int>> ret;
		// Las dimensiones de las salas son min=5 y max=40 (por como se generan en el grafo)

		int puertasPorLado[4];	// Con este array, indicamos cuantas puertas tendra cada pared;

		// A todos se les distribuyen las mismas puertas
		puertasPorLado[UP] = nUniones / 4;	
		puertasPorLado[LEFT] = nUniones / 4;
		puertasPorLado[DOWN] = nUniones / 4;
		puertasPorLado[RIGHT] = nUniones / 4;

		// Si hay puertas sobrantes, se asignan de forma aleatoria
		int resto = nUniones % 4;
		while (resto > 0){
			puertasPorLado[getRandom(0, 3)]++;
			resto--;
		}

	}

	static int getRandom(int from, int to){
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(from, to);
		return dis(gen);
	}

	static const int UP = 0;
	static const int LEFT = 1;
	static const int DOWN = 2;
	static const int RIGHT = 3;

};


#endif