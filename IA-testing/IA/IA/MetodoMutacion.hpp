#ifndef METODOMUTACION_HPP
#define METODOMUTACION_HPP

#include <vector>
#include "cromosoma.hpp"

class metodoMutacion {
public:
	virtual void mutar() = 0;
	virtual std::string toString() = 0;
protected:
	std::vector<Operacion> terminales = { Operacion::Avanza, Operacion::GiraDer, Operacion::GiraIz, Operacion::CambiarEst, Operacion::Atacar, Operacion::BloquearN, Operacion::Retroceder };
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
				break; //son terminales, no se pueden mutar así
			case Operacion::ProgN2:
				int r = 0 + (rand() % (int)(4 - 0 + 1)); //Hay 4 operaciones distintas más con grado 2
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
				break; //No hay más operaciones de grado 3
			case Operacion::ProgN4:
				break; //No hay más operaciones de grado 4
			case Operacion::SiBloqueado:
				int r = 0 + (rand() % (int)(4 - 0 + 1)); //Hay 4 operaciones distintas más con grado 2
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
				int r = 0 + (rand() % (int)(4 - 0 + 1)); //Hay 4 operaciones distintas más con grado 2
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
				int r = 0 + (rand() % (int)(4 - 0 + 1)); //Hay 4 operaciones distintas más con grado 2
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
				int r = 0 + (rand() % (int)(4 - 0 + 1)); //Hay 4 operaciones distintas más con grado 2
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
		int r = 0 + (rand() % (int)(this->terminales.size() - 0 + 1)); //Hay 4 operaciones distintas más con grado 2
		Operacion op = this->terminales[r];
		Nodo* n = c.getTerminalAleatorio();
		n->setElem(op);
	}

	std::string toString() {
		return "Terminal";
	}
};

#endif