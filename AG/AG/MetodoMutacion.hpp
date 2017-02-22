#ifndef METODOMUTACION_HPP
#define METODOMUTACION_HPP

#include "RandomGen.hpp"
#include "Cromosoma.hpp"

class MetodoMutacion {
public:

	virtual void mutar(Cromosoma &c);

};

class MutacionArista : public MetodoMutacion {
public:
	void mutar(Cromosoma &c){
		Grafo<Gen> g = c.getGenotipo();
		auto ady = g.getAdyacencia();
		unsigned int randNodo, randArista;
		
		if (RandomGen::getRandom(0u, 1u)){
			// Se crea una arista aleatoria
		}
		else{
			// Se borra una arista aleatoria
			randNodo = RandomGen::getRandom(0u, ady.size());
			randArista = RandomGen::getRandom(0u, ady[randNodo].size());
			auto otroNodo = ady[randNodo].begin();
			std::advance(otroNodo, randArista);

			// AQUIII SEGUIR, se debe borrar la arista de ambos nodos!!!
		}
		
		
	}

private:
	

};

#endif