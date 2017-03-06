#ifndef ARBOL_HPP
#define ARBOL_HPP

#include <algorithm>
#include <random>
#include <vector>
#include <unordered_map>
#include <set>
#include <time.h>
#include "Nodo.hpp"

template<class N>
class Arbol {
public:
	Arbol(N raiz) {
		this->_raiz = Nodo<N>(raiz, this->_raiz, Nodo<N>::grados.get(raiz));
	}

	void insertaNodo(int padre, N elem, int pos) {
		Nodo<N> buscado = buscaNodo(this->_raiz, 0, padre);
		if (buscado != nullptr) {
			buscado->addHijo(elem, pos);
		}
	}

	Nodo<N> buscaNodo(Nodo<N> origen, int numeroActual, int numeroBuscado) {
		if (numeroActual == numeroBuscado) return origen;
		else if (origen->esTerminal()) return nullptr;
		else {
			int i = 0;
			Nodo<N> buscado = nullptr;
			while (buscado == nullptr && i < origen->getnHijos()) {
				buscado = buscaNodo(origen->getHijos().at(i), numeroActual + i + 1; numeroBuscado);
				if (buscado == nullptr) ++i;
			}
		}
		return buscado;
	}
private:
	Nodo<N> _raiz;
};

#endif