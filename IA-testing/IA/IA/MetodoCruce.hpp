#ifndef METODOCRUCE_HPP
#define METODOCRUCE_HPP

#include <iostream>
#include <string>
#include "cromosoma.hpp"

class metodoCruce {
public:
	virtual void cruzar(Cromosoma a, Cromosoma b) = 0;
	virtual std::string toString() = 0;
};

class cruceSimple : public metodoCruce {
public:
	void cruzar(Cromosoma a, Cromosoma b) {
		Arbol arbA = a.getGenotipo();
		Arbol arbB = b.getGenotipo();
		int corteA, corteB;
		arbA.actualizaNumNodos();
		arbB.actualizaNumNodos();
		if (arbA.getNumNodos() == 1) {
			corteA = 0;
		}
		else {
			int r = myRandom::getRandom(1, arbA.getNumNodos());
			corteA = r;
		}
		if (arbB.getNumNodos() == 1) {
			corteB = 0;
		}
		else {
			int r = myRandom::getRandom(1, arbB.getNumNodos());
			corteB = r;
		}
		Nodo nodoA = arbA.buscaNodo(corteA);
		Nodo nodoB = arbB.buscaNodo(corteB);
		try {
			arbA.insertaNodo(nodoA.getPadre(), nodoB, nodoA.getPos());
			arbB.insertaNodo(nodoB.getPadre(), nodoA, nodoB.getPos());
		}
		catch (int a) {
			if (&nodoA == nullptr) {
				std::cout << arbA.toString() << std::endl;
				std::cout << "NodoA es nulo (corte: " << corteA << ")";
			}
			if (&nodoB == nullptr) {
				std::cout << arbB.toString() << std::endl;
				std::cout << "NodoB es nulo (corte: " << corteB << ")";
			}
		}

		int aux = nodoA.getPos();
		nodoA.setPos(nodoB.getPos());
		nodoB.setPos(aux);

		arbA.actualizaNumNodos();
		arbB.actualizaNumNodos();
	}

	std::string toString() {
		return "Cruce simple";
	}
};

#endif