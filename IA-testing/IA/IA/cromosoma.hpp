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

	double getPuntAcum() {
		return this->_puntAcum;
	}

	void setPuntAcum(double p) {
		this->_puntAcum = p;
	}

	double getAdaptacion() {
		return this->_adaptacion;
	}

	void setAdaptacion(double a) {
		this->_adaptacion = a;
	}

	Nodo* getTerminalAleatorio() {
		return _genotipo.getTerminalAleatorio();
	}

	Nodo* getNodoFuncionAleatorio() {
		return _genotipo.getNodoFuncionAleatorio();
	}

	void bloating() {
		_genotipo.bloating();
	}

	double evalua(std::vector<Mapa> m) {
		//metodo de evaluación, recorrer los mapas y conseguir el valor acumulado
	}

private:
	Arbol _genotipo;
	double _punt;
	double _puntAcum;
	double _adaptacion;
};

#endif