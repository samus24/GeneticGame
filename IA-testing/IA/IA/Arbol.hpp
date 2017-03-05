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
		this->_raiz = new Nodo<N>(raiz, this->_raiz, Nodo.grados.get(raiz));
	}
private:
	Nodo<N> _raiz;
};

#endif