#ifndef ARBOL_HPP
#define ARBOL_HPP

#include <algorithm>
#include <random>
#include <vector>
#include <unordered_map>
#include <set>
#include <random>
#include <time.h>
#include "Nodo.hpp"

class Arbol {
public:
	Arbol(Nodo raiz) :
		_raiz(raiz){}

	void insertaNodo(int padre, Operacion elem, int pos) {
		Nodo* buscado = buscaNodo(&this->_raiz, 0, padre);
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

	Nodo* buscaNodo(Nodo* origen, int numeroActual, int numeroBuscado) {
		if (numeroActual == numeroBuscado) return origen;
		else if (origen->esTerminal()) return nullptr;
		else {
			int i = 0;
			Nodo* buscado = nullptr;
			while (buscado == nullptr && i < origen->getNhijos()) {
				buscado = buscaNodo(&origen->getHijos().at(i), numeroActual + i + 1, numeroBuscado);
				if (buscado == nullptr) ++i;
			}
			return buscado;
		}
	}

	int getNumNodos() {
		_raiz.getNhijos();
	}

	void creaArbolAleatorio(int profMin, int profMax) {
		this->_profMin = profMin;
		this->_profMax = profMax;
		this->_raiz = creaArbol(nullptr, _raiz, profMin, profMax, 0);
	}

	Nodo creaArbol(Nodo* padre, Nodo a, int pMin, int pMax, int pos) {
		if (pMin > 0) {
			int r = 0 + (rand() % (int)(6 - 0 + 1)); //los valores entre 0 y 6 no son hoja
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
			int r = 0 + (rand() % (int)(6 - 0 + 1)); 
			r += 7; //los valores entre 7-13 son hojas
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
	}

private:
	Nodo _raiz;
	int _profMin;
	int _profMax;
};

#endif