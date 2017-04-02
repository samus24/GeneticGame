#include "Nodo.hpp"

Nodo::Nodo(Operacion elem, Nodo* padre, int nHijos, int pos) {
	this->_padre = padre;
	this->_nHijos = nHijos;
	this->_elem = elem;
	this->setPos(pos);
	for (std::size_t i = 0; i < _nHijos; ++i) {
		//se rellena el vector con nodos para reservar espacio, son indiferentes
		this->_hijos.push_back(Nodo(Operacion::Avanza, nullptr, 0, 0));
	}
}


bool Nodo::addHijo(Operacion h, int pos) {
	if (pos >= _nHijos) return false;
	this->_hijos.at(pos) = Nodo(h, this, GRADOS[h], pos);
	return true;
}


bool Nodo::addHijo(Nodo h, int pos) {
	if (pos >= _nHijos) return false;
	this->_hijos.at(pos) = h;
	return true;
}


bool Nodo::esTerminal() {
	return (this->_nHijos == 0);
}


Operacion Nodo::getElem() {
	return _elem;
}


void Nodo::setElem(Operacion elem) {
	this->_elem = elem;
}


std::vector<Nodo> Nodo::getHijos() {
	return _hijos;
}


void Nodo::setHijos(std::vector<Nodo> hijos) {
	this->_hijos = hijos;
}


int Nodo::getNhijos() {
	return _nHijos;
}


void Nodo::setNhijos(int nHijos) {
	this->_nHijos = nHijos;
}


Nodo* Nodo::getPadre() {
	return _padre;
}


void Nodo::setPadre(Nodo* padre) {
	this->_padre = padre;
}

int Nodo::getPos() {
	return _pos;
}

void Nodo::setPos(int pos) {
	this->_pos = pos;
}

bool esOperacionTerminal(Operacion o) {
	return ((o == Operacion::Atacar) || (o == Operacion::Avanza) || (o == Operacion::BloquearN) || (o == Operacion::CambiarEst) || (o == Operacion::GiraDer) || (o == Operacion::GiraIz) || (o == Operacion::Retroceder));
}

void Nodo::evalua(std::vector<Mapa> m, npc pnj) {
	//aquí habría que ejecutar el arbol sobre el mapa
}
void Nodo::bloating(int pMax, int nivel) {
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

void Nodo::eliminaIntrones(){
	Operacion hijoA;
	Operacion hijoB;
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
		if (hijoA == hijoB && esOperacionTerminal(hijoA)) {
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
		if (hijoA == hijoB && esOperacionTerminal(hijoA)) {
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
		if (hijoA == hijoB && esOperacionTerminal(hijoA)) {
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
		if (hijoA == hijoB && esOperacionTerminal(hijoA)) {
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

int Nodo::actualizaNumNodos() {
	int n = 1;
	for (std::size_t i = 0; i < _nHijos; ++i) {
		n += _hijos[i].actualizaNumNodos();
	}
	this->_numNodos = n;
	return n;
}
