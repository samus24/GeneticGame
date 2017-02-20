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
			std::vector<Pair<int, int>> puertas = posicionesPuertas(itSalas->second.getAncho(), itSalas->second.getAlto(), nUniones);
			for (Pair<int, int> p : puertas){
				s.setCelda(p.first, p.second, *itUniones);
				++itUniones;
			}
			m.anadeSala(s);
			++itSalas;
		}
		return m;
	}
private:
	std::vector<Pair<int, int>> posicionesPuertas(unsigned int ancho, unsigned int alto, unsigned int nUniones){
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

		unsigned int step = ancho / puertasPorLado[UP];
		for (size_t i = 0; i < puertasPorLado[UP]; ++i)
			ret.push_back(Pair<int, int>(step*(i+1), 0));
		step = ancho / puertasPorLado[DOWN];
		for (size_t i = 0; i < puertasPorLado[DOWN]; ++i)
			ret.push_back(Pair<int, int>(step*(i + 1), alto-1));
		step = alto / puertasPorLado[LEFT];
		for (size_t i = 0; i < puertasPorLado[LEFT]; ++i)
			ret.push_back(Pair<int, int>(0,step*(i + 1)));
		step = alto / puertasPorLado[RIGHT];
		for (size_t i = 0; i < puertasPorLado[RIGHT]; ++i)
			ret.push_back(Pair<int, int>(ancho-1,step*(i + 1)));

		return ret;
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