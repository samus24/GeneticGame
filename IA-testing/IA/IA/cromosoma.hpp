#ifndef CROMOSOMA_HPP
#define CROMOSOMA_HPP

#include "Arbol.hpp"
#include "myrandom.hpp"

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

	Arbol getGenotipo(int pos) {
		return _genotipo[pos];
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
		//cargar mapa con la posicion del jugador
		//colocar al npc aleatoriamente
		//while de la patrulla si turnos no acabados o no cambiar estado.
		//si cambiar estado, arbol ataque mientras turnos no acabados o jugador no eliminado
		setRandomPosition(m);
		int turnos = 0;
		int turnosPatrulla = 0;
		bool ataque = false;
		Nodo* actual = _genotipo[0].getRaiz();
		while (turnos < 100 && !ataque) {

		}
		return 0.f;
	}

	npc setRandomPosition(Mapa &m) {
		int x, y;
		npc enemigo();
		do{
			x = myRandom::getRandom(0, m.getWidth -1);
			y = myRandom::getRandom(0, m.getHeight - 1);
		} while (m[x][y] != m.VACIO);
	}

	void eliminaIntrones() {
		for (std::size_t i = 0; i < _genotipo.size(); ++i) {
			_genotipo[i].eliminaIntrones();
		}
	}

private:
	std::vector<Arbol> _genotipo; //la primera posición es el árbol de patrulla y la segunda el de ataque.
	double _punt;
	double _puntAcum;
	double _adaptacion;
};

#endif