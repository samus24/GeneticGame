#ifndef NODO_HPP
#define NODO_HPP
#include <unordered_map>
#include <vector>
#include "npc.hpp"
#include "Mapa.hpp"
#include "myrandom.hpp"

enum Operacion {
	ProgN2,
	ProgN3,
	ProgN4,
	SiJugador,
	SiBloqueado,
	SiRango,
	SiDetectado,
	CambiarEst,
	Avanza,
	GiraIz,
	GiraDer,
	BloquearN,
	Atacar,
	Retroceder
};

static std::unordered_map<Operacion, int> GRADOS{  // 0-6 No hojas, 7-13 hojas
	{ ProgN2, 2 },
	{ ProgN3, 3 },
	{ ProgN4, 4 },
	{ SiJugador, 2 },
	{ SiBloqueado, 2 },
	{ SiRango, 2 },
	{ SiDetectado, 2 },
	{ CambiarEst, 0 },
	{ Avanza, 0 },
	{ GiraIz, 0 },
	{ GiraDer, 0 },
	{ BloquearN, 0 },
	{ Atacar, 0 },
	{ Retroceder, 0 }
};

class Nodo {
public:	

	Nodo() {}

	Nodo(Operacion elem, Nodo* padre, int nHijos, int pos) {
		this->_padre = padre;
		this->_nHijos = nHijos;
		this->_elem = elem;
		this->setPos(pos);
		for (std::size_t i = 0; i < _nHijos; ++i) {
			//se rellena el vector con nodos para reservar espacio, son indiferentes
			this->_hijos.push_back(Nodo(Operacion::Avanza, nullptr, 0, 0));
		}
	}


	bool addHijo(Operacion h, int pos) {
		if (pos >= _nHijos) return false;
		this->_hijos.at(pos) = Nodo(h, this, GRADOS[h], pos);
		return true;
	}


	bool addHijo(Nodo h, int pos) {
		if (pos >= _nHijos) return false;
		this->_hijos.at(pos) = h;
		return true;
	}


	bool esTerminal() {
		return (this->_nHijos == 0);
	}

	Operacion getElem() {
		return _elem;
	}


	void setElem(Operacion elem) {
		this->_elem = elem;
	}


	std::vector<Nodo> getHijos() {
		return _hijos;
	}


	void setHijos(std::vector<Nodo> hijos) {
		this->_hijos = hijos;
	}


	int getNhijos() {
		return _nHijos;
	}


	void setNhijos(int nHijos) {
		this->_nHijos = nHijos;
	}


	Nodo* getPadre() {
		return _padre;
	}


	void setPadre(Nodo* padre) {
		this->_padre = padre;
	}

	int getPos() {
		return _pos;
	}

	void setPos(int pos) {
		this->_pos = pos;
	}

	int getNumNodos() {
		return _numNodos;
	}

	void setNumNodos(int _numNodos) {
		this->_numNodos = _numNodos;
	}

	bool esOperacionTerminal(Operacion o) {
		return ((o == Operacion::Atacar) || (o == Operacion::Avanza) || (o == Operacion::BloquearN) || (o == Operacion::CambiarEst) || (o == Operacion::GiraDer) || (o == Operacion::GiraIz) || (o == Operacion::Retroceder));
	}

	double evalua(std::vector<Mapa> m, npc pnj) {
		//aqu� habr�a que ejecutar el arbol sobre el mapa
		return 0.f;
	}
	void bloating(int pMax, int nivel) {
		if (nivel == pMax) {
			std::vector<Nodo> _hijosAux(0);
			this->_hijos = _hijosAux;
			int r = myRandom::getRandom(7, 14);
			Operacion op = (Operacion)r;
			this->_elem = op;
			this->_nHijos = 0;
		}
		else {
			for (std::size_t i = 0; i < this->_nHijos; ++i) {
				this->_hijos[i].bloating(pMax, nivel + 1);
			}
		}
	}

	void eliminaIntrones() {
		Operacion hijoA;
		Operacion hijoB;
		bool esTerminal;
		switch (this->_elem) {
		case Operacion::Atacar:
		case Operacion::Avanza:
		case Operacion::BloquearN:
		case Operacion::CambiarEst:
		case Operacion::GiraDer:
		case Operacion::GiraIz:
		case Operacion::Retroceder:
			break;
		case Operacion::ProgN2:
			hijoA = this->_hijos[0]._elem;
			hijoB = this->_hijos[1]._elem;
			if (hijoA == Operacion::GiraDer) {
				if (hijoB == Operacion::GiraIz) {
					int r = myRandom::getRandom(7, 14);
					Operacion op = (Operacion)r;
					this->_elem = op;
					std::vector<Nodo> _hijosAux(0);
					this->_hijos = _hijosAux;
					this->_nHijos = 0;
				}
			}
			else if (hijoB == Operacion::GiraDer) {
				if (hijoA == Operacion::GiraIz) {
					int r = myRandom::getRandom(7, 14);
					Operacion op = (Operacion)r;
					this->_elem = op;
					std::vector<Nodo> _hijosAux(0);
					this->_hijos = _hijosAux;
					this->_nHijos = 0;
				}
			}
			else {
				this->_hijos[0].eliminaIntrones();
				this->_hijos[1].eliminaIntrones();
			}
			break;
		case Operacion::ProgN3:
			this->_hijos[0].eliminaIntrones();
			this->_hijos[1].eliminaIntrones();
			this->_hijos[2].eliminaIntrones();
			break;
		case Operacion::ProgN4:
			this->_hijos[0].eliminaIntrones();
			this->_hijos[1].eliminaIntrones();
			this->_hijos[2].eliminaIntrones();
			this->_hijos[3].eliminaIntrones();
			break;
		case Operacion::SiBloqueado:
			hijoA = this->_hijos[0]._elem;
			hijoB = this->_hijos[1]._elem;
			esTerminal = Nodo::esOperacionTerminal(hijoA);
			if (hijoA == hijoB && esTerminal) {
				this->_elem = hijoA;
				std::vector<Nodo> _hijosAux(0);
				this->_hijos = _hijosAux;
				this->_nHijos = 0;
			}
			else {
				this->_hijos[0].eliminaIntrones();
				this->_hijos[1].eliminaIntrones();
			}
			break;
		case Operacion::SiDetectado:
			hijoA = this->_hijos[0]._elem;
			hijoB = this->_hijos[1]._elem;
			esTerminal = Nodo::esOperacionTerminal(hijoA);
			if (hijoA == hijoB && esTerminal) {
				this->_elem = hijoA;
				std::vector<Nodo> _hijosAux(0);
				this->_hijos = _hijosAux;
				this->_nHijos = 0;
			}
			else {
				this->_hijos[0].eliminaIntrones();
				this->_hijos[1].eliminaIntrones();
			}
			break;
		case Operacion::SiJugador:
			hijoA = this->_hijos[0]._elem;
			hijoB = this->_hijos[1]._elem;
			esTerminal = Nodo::esOperacionTerminal(hijoA);
			if (hijoA == hijoB && esTerminal) {
				this->_elem = hijoA;
				std::vector<Nodo> _hijosAux(0);
				this->_hijos = _hijosAux;
				this->_nHijos = 0;
			}
			else {
				this->_hijos[0].eliminaIntrones();
				this->_hijos[1].eliminaIntrones();
			}
			break;
		case Operacion::SiRango:
			hijoA = this->_hijos[0]._elem;
			hijoB = this->_hijos[1]._elem;
			esTerminal = Nodo::esOperacionTerminal(hijoA);
			if (hijoA == hijoB && esTerminal) {
				this->_elem = hijoA;
				std::vector<Nodo> _hijosAux(0);
				this->_hijos = _hijosAux;
				this->_nHijos = 0;
			}
			else {
				this->_hijos[0].eliminaIntrones();
				this->_hijos[1].eliminaIntrones();
			}
			break;
		default:
			break;
		}
	}

	int actualizaNumNodos() {
		int n = 1;
		for (std::size_t i = 0; i < _nHijos; ++i) {
			n += _hijos[i].actualizaNumNodos();
		}
		this->_numNodos = n;
		return n;
	}

private:
	Nodo* _padre;
	int _nHijos;
	Operacion _elem;
	std::vector<Nodo> _hijos;
	int _numNodos;
	int _pos;
};



#endif