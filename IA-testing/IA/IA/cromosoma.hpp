#ifndef CROMOSOMA_HPP
#define CROMOSOMA_HPP

#include "Arbol.hpp"

class Cromosoma {
public:
	Cromosoma(int profMin, int profMax) {
		genotipo = arbol(Operacion::Avanza);
		genotipo->creaArbolAleatorio(profMin, profMax);
	}

	Arbol getGenotipo() {
		return _genotipo;
	}

	void setGenotipo(Arbol genotipo) {
		this->genotipo = genotipo;
	}

	double getPunt() {
		return _punt
	}

	void setPunt(double punt) {
		this->_punt = punt;
	}

private:
	Arbol _genotipo;
	double _punt;
	double _puntAcum;
	double _adaptacion;
}

#endif