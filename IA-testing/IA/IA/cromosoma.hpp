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
		this->_genotipo[0] = Arbol(Operacion::Avanza);
		this->_genotipo[1] = Arbol(Operacion::Avanza);
		this->_genotipo[0].creaArbolAleatorio(profMin, profMax);
		this->_genotipo[1].creaArbolAleatorio(profMin, profMax);
	}

	std::vector<Arbol> getGenotipo() {
		return _genotipo;
	}

	void setGenotipo(std::vector<Arbol> genotipo) {
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

	Nodo* getTerminalAleatorio(int pos) {
		return _genotipo[pos].getTerminalAleatorio();
	}

	Nodo* getNodoFuncionAleatorio(int pos) {
		return _genotipo[pos].getNodoFuncionAleatorio();
	}

	void bloating(int prof) {
		for (std::size_t i = 0; i < _genotipo.size(); ++i) {
			_genotipo[i].bloating(prof);
		}
	}

	double evalua(Mapa m) {
		//metodo de evaluación, recorrer los mapas y conseguir el valor acumulado
		return 0.f;
	}

	void eliminaIntrones() {
		for (std::size_t i = 0; i < _genotipo.size(); ++i) {
			_genotipo[i].eliminaIntrones();
		}
	}

private:
	std::vector<Arbol> _genotipo; //la primera posición es el árbol de búsqueda y la segunda el de ataque.
	double _punt;
	double _puntAcum;
	double _adaptacion;
};

#endif