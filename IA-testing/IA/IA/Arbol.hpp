#ifndef ARBOL_HPP
#define ARBOL_HPP

#include <algorithm>
#include <vector>
#include <set>
#include <random>
#include <time.h>
#include <queue>
#include "Nodo.hpp"

class Arbol {
public:
	Arbol();

	Arbol(Operacion raiz) :
		_raiz(raiz, nullptr, GRADOS[raiz], 0){}

	void insertaNodo(int padre, Operacion elem, int pos) {
		Nodo* buscado = buscaNodo(&this->_raiz, padre);
		if (buscado != nullptr) {
			buscado->addHijo(elem, pos);
		}
	}

	void insertaNodo(Nodo* padre, Operacion elem, int pos) {
		if (padre != nullptr) {
			padre->addHijo(elem, pos);
		}
	}

	void insertaNodo(Nodo* padre, Nodo hijo, int pos) {
		if (padre != nullptr) {
			padre->addHijo(hijo, pos);
		}
	}

	Nodo buscaNodo(int numeroBuscado) {
		Nodo* buscado = buscaNodo(&_raiz, numeroBuscado);
		return *buscado;
	}

	Nodo* buscaNodo(Nodo* origen, int numeroBuscado) {
		std::queue<Nodo> q;
		std::queue<Nodo> qC;
		q.push(_raiz);
		qC.push(_raiz);
		while (!q.empty()) {
			Nodo head = q.front();
			q.pop();
			for (std::size_t i = 0; i < head.getNhijos(); ++i) {
				q.push(head.getHijos()[i]);
				qC.push(head.getHijos()[i]);
			}
		}
		for (std::size_t i = 0; i < numeroBuscado; ++i) {
			qC.pop();
		}
		return &qC.front();
	}

	int getNumNodos() {
		return _raiz.getNhijos();
	}

	void creaArbolAleatorio(int profMin, int profMax) {
		this->_profMin = profMin;
		this->_profMax = profMax;
		this->_raiz = creaArbol(nullptr, _raiz, profMin, profMax, 0);
	}

	Nodo creaArbol(Nodo* padre, Nodo a, int pMin, int pMax, int pos) {
		if (pMin > 0) {
			int r = myRandom::getRandom(0, 6);
			Operacion op = (Operacion)r;
			a = Nodo(op, padre, GRADOS[op], pos);
			a.setNumNodos(1);
			for (std::size_t i = 0; i < GRADOS[op]; ++i) {
				//como convierto un Nodo a Nodo*??
				Nodo aux = creaArbol(&a, a.getHijos()[i], pMin - 1, pMax - 1, i);
				a.addHijo(aux, i);
				a.setNumNodos(a.getNumNodos() + aux.getNumNodos());
			}
		}
		else if (pMax <= 0) {
			int r = myRandom::getRandom(7, 14);
			Operacion op = (Operacion)r;
			a = Nodo(op, padre, GRADOS[op], pos);
			a.setNumNodos(1);
		}
		else {
			int r = 0 + (rand() % (int)(13 - 0 + 1));
			Operacion op = (Operacion)r;
			a = Nodo(op, padre, GRADOS[op], pos);
			a.setNumNodos(1);
			for (std::size_t i = 0; i < GRADOS[op]; ++i) {
				//como convierto un Nodo a Nodo*??
				Nodo aux = creaArbol(&a, a.getHijos()[i], pMin - 1, pMax - 1, i);
				a.addHijo(aux, i);
				a.setNumNodos(a.getNumNodos() + aux.getNumNodos());
			}
		}
		return a;
	}

	Nodo* getTerminalAleatorio() {
		Nodo actual = _raiz;
		int numHijos = _raiz.getNhijos();
		while (!actual.esTerminal()) {
			int h = 0 + (rand() % (int)(numHijos - 0 + 1));
			actual = actual.getHijos()[h];
			numHijos = actual.getNhijos();
		}
	}

	Nodo* getNodoFuncionAleatorio() {
		if (_raiz.getNumNodos() <= 1) {
			return nullptr;
		}
		Nodo* n = nullptr;
		do {
			int r = myRandom::getRandom(0, _raiz.getNumNodos() + 1);
			n = &buscaNodo(r);
		} while (n->esTerminal());
		return n;
	}

	int getProfMin() {
		return _profMin;
	}

	void setProfMin(int pMin) {
		this->_profMin = pMin;
	}

	int getProxMax() {
		return _profMax;
	}

	void setProfMax(int pMax) {
		this->_profMax = pMax;
	}

	double evalua(std::vector<Mapa> m, npc pnj) {
		return this->_raiz.evalua(m, pnj);
	}

	void actualizaNumNodos() {
		this->_raiz.actualizaNumNodos();
	}

	void bloating(int pMax) {
		this->_raiz.bloating(pMax, 0);
		this->_raiz.actualizaNumNodos();
	}

	void eliminaIntrones() {
		_raiz.eliminaIntrones();
		_raiz.actualizaNumNodos();
	}

	std::string toString() {
		return "Falta hacer el metodo toString";
	}

private:
	Nodo _raiz;
	int _profMin;
	int _profMax;
};

#endif