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
			Sala s(itSalas->second.getAncho(), itSalas->second.getAlto());	// Las salas se crean vacias
			unsigned int nUniones = uniones[itSalas->first].size();
			auto itUniones = uniones[itSalas->first].begin();
			while (itUniones != uniones[itSalas->first].cend()){
				// !!!!!!!!!!! TODO !!!!!!!!!!!!
				// Falta todo vaya, rellenar la sala, empezando por las puertas
				++itUniones;
			}
			++itSalas;
		}
	}
private:
	std::vector<Pair<int, int>> posicionesPreferidas(unsigned int ancho, unsigned int alto, unsigned int nUniones){
		std::vector<Pair<int, int>> ret;
		// Tengo ideas para esto, pero tengo que darlo una vuelta...
	}

//	static const 

};


#endif