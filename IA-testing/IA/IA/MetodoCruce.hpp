#ifndef METODOCRUCE_HPP
#define METODOCRUCE_HPP

#include <iostream>
#include <string>
#include "cromosoma.hpp"

bool sonIguales(const Nodo &a, const Nodo &b){
	if (a.getPadre() == b.getPadre()){
		if (a.getPos() == b.getPos()){
			if (a.getElem() == b.getElem()){
				return true;
			}
		}
	}
	return false;
}

class metodoCruce {
public:
	virtual void cruzar(Cromosoma* a, Cromosoma* b, int pos) = 0;
	virtual std::string toString() = 0;
};

class cruceSimple : public metodoCruce {
public:
	void cruzar(Cromosoma* a, Cromosoma* b, int pos) {
		Arbol arbA = a->getGenotipo(pos);
		Arbol arbB = b->getGenotipo(pos);
		int corteA, corteB;
		arbA.actualizaNumNodos();
		arbB.actualizaNumNodos();

		std::cout << "A: " << arbA.toString() << std::endl;
		std::cout << "B: " << arbB.toString() << std::endl;
		
		if (arbA.getNumNodos() == 1) {
			corteA = 0;
		}
		else {
			int r = myRandom::getRandom(1, arbA.getNumNodos()-1);
			corteA = r;
		}
		if (arbB.getNumNodos() == 1) {
			corteB = 0;
		}
		else {
			int r = myRandom::getRandom(1, arbB.getNumNodos()-1);
			corteB = r;
		}
		Nodo nodoA = arbA.buscaNodo(corteA);
		Nodo nodoB = arbB.buscaNodo(corteB);
		Nodo* pA = nodoA.getPadre();
		Nodo* pB = nodoB.getPadre();

		nodoA.setPadre(pB);
		nodoB.setPadre(pA);

		int posA = nodoA.getPos();
		int posB = nodoB.getPos();

		nodoA.setPos(posB);
		nodoB.setPos(posA);

		try{
			arbA.insertaNodo(pA, nodoB, posA);
		}
		catch (std::exception e){
			std::cerr << e.what() << std::endl;
		}
		try{
			arbB.insertaNodo(pB, nodoA, posB);
		}
		catch (std::exception e){
			std::cerr << e.what() << std::endl;
		}		
		
		if (!arbA.compruebaIntegridad()){
			return;
		}

		if (!arbB.compruebaIntegridad()){
			return;
		}

		arbA.actualizaNumNodos();
		arbB.actualizaNumNodos();
		
	}

	std::string toString() {
		return "Cruce simple";
	}
};

#endif