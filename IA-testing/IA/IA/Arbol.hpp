#ifndef ARBOL_HPP
#define ARBOL_HPP

#include <algorithm>
#include <random>
#include <vector>
#include <unordered_map>
#include <set>
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
private:
	Nodo _raiz;
};

#endif