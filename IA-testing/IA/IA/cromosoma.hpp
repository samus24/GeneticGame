#ifndef CROMOSOMA_HPP
#define CROMOSOMA_HPP

#include "Arbol.hpp"

class Cromosoma {
public:
	Cromosoma(int profMin, int profMax) {
		genotipo = arbol(Operacion::Avanza);
		genotipo->creaArbolAleatorio(profMin, profMax);
	}


private:
	Arbol genotipo;
	double punt;
	double puntAcum;
	double adaptacion;
}

#endif