#ifndef METODOMUTACION_HPP
#define METODOMUTACION_HPP

#include <vector>
#include "cromosoma.hpp"

class metodoMutacion {
public:
	virtual void mutar(Cromosoma* c, TipoArbol tipo) = 0;
	virtual std::string toString() = 0;
};

class mutacionArbol : public metodoMutacion {
public:
	void mutar(Cromosoma* c, TipoArbol tipo) {
		Arbol arb = c->getGenotipo(tipo);
		Nodo n;
		n = arb.getNodoFuncionAleatorio();
		if (n.getPadre() != nullptr) {
			Nodo nuevo = arb.creaArbol(n.getPadre(), &n, arb.getProfMin(), arb.getProfMax(), n.getPos(), tipo);
			n.getPadre()->addHijo(nuevo, n.getPos());
			arb.actualizaNumNodos();
			c->setGenotipo(arb, tipo);
		}
	}

	std::string toString() {
		return "Arbol";
	}
};


class mutacionFuncion : public metodoMutacion {
public:
	void mutar(Cromosoma* c, TipoArbol tipo) {
		Arbol arb = c->getGenotipo(tipo);
		Nodo n;
		n = arb.getNodoFuncionAleatorio();
		if (n.getPadre() != nullptr) {
			if (n.getNhijos() == 2){
				// Solo se pueden intercambiar nodos con 2 hijos, el resto son terminales o no hay mas con los mismos hijos
				Operacion elem;
				do{
					elem = Nodo::getNoTerminalAleatorio(tipo);
				} while (elem == n.getElem() || GRADOS[elem] != 2);
				n.setElem(elem);
				arb.actualizaNumNodos();
				c->setGenotipo(arb, tipo);
			}
		}
	}

	std::string toString() {
		return "funcion";
	}
};

class mutacionTerminal : public metodoMutacion {
public:
	void mutar(Cromosoma* c, TipoArbol tipo) {
		Arbol arb = c->getGenotipo(tipo);
		Nodo n;
		n = arb.getTerminalAleatorio();
		if (n.getPadre() != nullptr) {
			Operacion elem;
			do{
				elem = Nodo::getTerminalAleatorio(tipo);
			} while (elem == n.getElem());
			n.setElem(elem);
			arb.actualizaNumNodos();
			c->setGenotipo(arb, tipo);
		}
	}

	std::string toString() {
		return "Terminal";
	}
};


#endif
