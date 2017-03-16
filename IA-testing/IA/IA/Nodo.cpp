#include "Nodo.hpp"


Nodo::Nodo(Operacion elem, Nodo* padre, int nHijos, int pos) {
	this->_padre = padre;
	this->_nHijos = nHijos;
	this->_elem = elem;
	this->setPos(pos);
	for (std::size_t i = 0; i < _nHijos; ++i) {
		this->_hijos.push_back(Nodo(Operacion::Avanza, nullptr, 0, 0)); //se rellena el vector con nodos para reservar espacio, son indiferentes
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

void evalua(std::vector<Mapa>, npc pnj) {
	//aquí habría que ejecutar el arbol sobre el mapa
}