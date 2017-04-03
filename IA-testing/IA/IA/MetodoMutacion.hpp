#ifndef METODOMUTACION_HPP
#define METODOMUTACION_HPP

#include <vector>
#include "cromosoma.hpp"

class metodoMutacion {
public:
	virtual void mutar(Cromosoma c) = 0;
	virtual std::string toString() = 0;
};

class mutacionArbol : public metodoMutacion {
public:
	void mutar(Cromosoma c) {
		Arbol arb = c.getGenotipo();
		arb.actualizaNumNodos();
		Nodo* n = nullptr;
		n = arb.getNodoFuncionAleatorio();
		if (n != nullptr) {
			Nodo nuevo = arb.creaArbol(n->getPadre(), *n, arb.getProfMin(), arb.getProxMax(), n->getPos());
			n->setElem(nuevo.getElem());
			n->setHijos(nuevo.getHijos());
			n->setNhijos(nuevo.getNhijos());
			arb.actualizaNumNodos();
		}
	}

	std::string toString() {
		return "Arbol";
	}
};

class mutacionFuncion : public metodoMutacion {
public:
	void mutar(Cromosoma c) {
		Nodo* n = c.getNodoFuncionAleatorio();
		int r;
		if (n == nullptr) return;
		else {
			switch (n->getElem()) {
			case Operacion::Atacar:
			case Operacion::Avanza:
			case Operacion::GiraDer:
			case Operacion::GiraIz:
			case Operacion::CambiarEst:
			case Operacion::BloquearN:
			case Operacion::Retroceder:
				break; //son terminales, no se pueden mutar as�
			case Operacion::ProgN2:
				r = myRandom::getRandom(0, 4);
				if (r == 0) {
					n->setElem(Operacion::SiBloqueado);
				}
				else if (r == 1) {
					n->setElem(Operacion::SiDetectado);
				}

				else if (r == 2) {
					n->setElem(Operacion::SiJugador);
				}

				else {
					n->setElem(Operacion::SiRango);
				}
				break;
			case Operacion::ProgN3:
				break; //No hay m�s operaciones de grado 3
			case Operacion::ProgN4:
				break; //No hay m�s operaciones de grado 4
			case Operacion::SiBloqueado:
				r = myRandom::getRandom(0, 4);
				if (r == 0) {
					n->setElem(Operacion::SiBloqueado);
				}
				else if (r == 1) {
					n->setElem(Operacion::SiDetectado);
				}

				else if (r == 2) {
					n->setElem(Operacion::SiJugador);
				}

				else {
					n->setElem(Operacion::SiRango);
				}
				break;
			case Operacion::SiDetectado:
				r = myRandom::getRandom(0, 4);
				if (r == 0) {
					n->setElem(Operacion::SiBloqueado);
				}
				else if (r == 1) {
					n->setElem(Operacion::ProgN2);
				}

				else if (r == 2) {
					n->setElem(Operacion::SiJugador);
				}

				else {
					n->setElem(Operacion::SiRango);
				}
				break;
			case Operacion::SiJugador:
				r = myRandom::getRandom(0, 4);
				if (r == 0) {
					n->setElem(Operacion::SiBloqueado);
				}
				else if (r == 1) {
					n->setElem(Operacion::SiDetectado);
				}

				else if (r == 2) {
					n->setElem(Operacion::ProgN2);
				}

				else {
					n->setElem(Operacion::SiRango);
				}
				break;
			case Operacion::SiRango:
				r = myRandom::getRandom(0, 4);
				if (r == 0) {
					n->setElem(Operacion::SiBloqueado);
				}
				else if (r == 1) {
					n->setElem(Operacion::SiDetectado);
				}

				else if (r == 2) {
					n->setElem(Operacion::SiJugador);
				}

				else {
					n->setElem(Operacion::ProgN2);
				}
				break;
			default:
				break;
			}
		}
	}

	std::string toString() {
		return "funcion";
	}
};

class mutacionTerminal : public metodoMutacion {
public:
	void mutar(Cromosoma c) {
		int r = myRandom::getRandom(0, 6); //Hay 7 terminales
		Operacion op;
		switch (r) {
		case 0:
			op = Operacion::Atacar;
				break;
		case 1:
			op = Operacion::Avanza;
				break;
		case 2:
			op = Operacion::BloquearN;
				break;
		case 3:
			op = Operacion::CambiarEst;
				break;
		case 4:
			op = Operacion::GiraDer;
				break;
		case 5:
			op = Operacion::GiraIz;
				break;
		case 6:
			op = Operacion::Retroceder;
				break;
		default:
			break;
		}
		Nodo* n = c.getTerminalAleatorio();
		n->setElem(op);
	}

	std::string toString() {
		return "Terminal";
	}
};

#endif