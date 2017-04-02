#ifndef CROMOSOMA_HPP
#define CROMOSOMA_HPP

#include "Arbol.hpp"

class Cromosoma {
public:

	Cromosoma() {
		_punt = 0;
		_puntAcum = 0;
		_adaptacion = 0;
	}

	Cromosoma(int profMin, int profMax) {
		this->_genotipo = Arbol(Operacion::Avanza);
		this->_genotipo.creaArbolAleatorio(profMin, profMax);
	}

	Arbol getGenotipo() {
		return _genotipo;
	}

	void setGenotipo(Arbol genotipo) {
		this->_genotipo = genotipo;
	}

	double getPunt() {
		return _punt;
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

	void bloating(int prof) {
		_genotipo.bloating(prof);
	}

	double evalua(Mapa m) {
		//metodo de evaluación, recorrer los mapas y conseguir el valor acumulado
	}

	void eliminaIntrones() {
		_genotipo.eliminaIntrones();
	}

private:
	Arbol _genotipo;
	double _punt;
	double _puntAcum;
	double _adaptacion;
};

#endif