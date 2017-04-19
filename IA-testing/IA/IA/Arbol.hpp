#ifndef ARBOL_HPP
#define ARBOL_HPP

#include <algorithm>
#include <vector>
#include <set>
#include <random>
#include <time.h>
#include <queue>
#include <stack>
#include <string>
#include "Nodo.hpp"

class Arbol {
public:
	Arbol() {}

	Arbol(Operacion raiz) :
		_raiz(raiz, nullptr, GRADOS[raiz], 0){}

	void insertaNodo(int padre, Operacion elem, int pos) {
		Nodo buscado = buscaNodo(&this->_raiz, padre);
		if (&buscado != nullptr) {
			buscado.addHijo(elem, pos);
		}
	}

	void insertaNodo(Nodo* padre, Operacion elem, int pos) {
		if (padre != nullptr) {
			padre->addHijo(elem, pos);
		}
	}

	void insertaNodo(Nodo* padre, Nodo hijo, int pos) {
		if (padre != nullptr) {
			padre->addHijo(hijo, pos);
		}
	}

	Nodo buscaNodo(int numeroBuscado) {
		Nodo buscado = buscaNodo(&_raiz, numeroBuscado);
		return buscado;
	}

	Nodo buscaNodo(Nodo* origen, int numeroBuscado) {
		std::queue<Nodo> q;
		std::queue<Nodo> qC;
		q.push(_raiz);
		qC.push(_raiz);
		while (!q.empty()) {
			Nodo head = q.front();
			q.pop();
			for (std::size_t i = 0; i < head.getNhijos(); ++i) {
				q.push(head.getHijos()[i]);
				qC.push(head.getHijos()[i]);
			}
		}
		for (std::size_t i = 0; i < numeroBuscado; ++i) {
			qC.pop();
		}
		return qC.front();
	}

	int getNumNodos() {
		return _raiz.getNumNodos();
	}

	void creaArbolAleatorio(int profMin, int profMax) {
		this->_profMin = profMin;
		this->_profMax = profMax;
		this->_raiz = creaArbol(nullptr, &_raiz, profMin, profMax, 0);
	}

	Nodo creaArbol(Nodo* padre, Nodo* a, int pMin, int pMax, int pos) {
		std::stack<Nodo*> padres;
		std::stack<int> nHijos;
		std::stack<int> posiciones;
		padres.push(padre);
		posiciones.push(pos);
		nHijos.push(1);
		Nodo* actual = a;
		while (!padres.empty()){
			if (pMin > 0) {
				// No se ha alcanzado la prof minima, el nodo NO puede ser hoja
				Operacion op = Nodo::getNoTerminalAleatorio();
				*actual = Nodo(op, padres.top(), GRADOS[op], posiciones.top());
				padres.push(actual);
				nHijos.push(GRADOS[op]);
				actual = &(actual->getHijos()[0]);
				posiciones.push(0);
				pMin--;
				pMax--;
			}
			else if (pMax <= 0) {
				// Se ha alcanzado la prof maxima, el nodo DEBE ser hoja
				Operacion op = Nodo::getTerminalAleatorio();
				*actual = Nodo(op, padres.top(), GRADOS[op], posiciones.top());
				actual->setNumNodos(1);
				int hijos = nHijos.top() - 1;
				nHijos.pop();
				int pos = posiciones.top() + 1;
				posiciones.pop();
				if (hijos == 0){
					if (padres.size() > 1){	// Se evita el padre nullptr
						int numNodos = 1;
						for (std::size_t i = 0; i < pos; ++i){
							numNodos += padres.top()->getHijos()[i].getNumNodos();
						}
						padres.top()->setNumNodos(numNodos);
					}
					padres.pop();
					pMin++;
					pMax++;
					do{
						hijos = nHijos.top() - 1;
						nHijos.pop();
						nHijos.push(hijos);
						pos = posiciones.top() + 1;
						posiciones.pop();
						posiciones.push(pos);
						if (hijos == 0){
							if (padres.size() > 1){	// Se evita el padre nullptr
								int numNodos = 1;
								for (std::size_t i = 0; i < pos; ++i){
									numNodos += padres.top()->getHijos()[i].getNumNodos();
								}
								padres.top()->setNumNodos(numNodos);
							}
							padres.pop();
							nHijos.pop();
							posiciones.pop();
						}
						else{
							actual = &(padres.top()->getHijos()[posiciones.top()]);
						}
					} while (hijos == 0 && !padres.empty());
					
				}
				else{
					nHijos.push(hijos);
					posiciones.push(pos);
					actual = &(padres.top()->getHijos()[pos]);
				}
			}
			else {
				// Altura intermedia, puede o no ser hoja
				Operacion op = Nodo::getElementoAleatorio();
				*actual = Nodo(op, padres.top(), GRADOS[op], posiciones.top());
				if (GRADOS[op] > 0){
					padres.push(actual);
					nHijos.push(GRADOS[op]);
					actual = &(actual->getHijos()[0]);
					posiciones.push(0);
					pMin--;
					pMax--;
				}
				else{
					actual->setNumNodos(1);
					int hijos = nHijos.top() - 1;
					nHijos.pop();
					int pos = posiciones.top() + 1;
					posiciones.pop();
					if (hijos == 0){
						if (padres.size() > 1){	// Se evita el padre nullptr
							int numNodos = 1;
							for (std::size_t i = 0; i < pos; ++i){
								numNodos += padres.top()->getHijos()[i].getNumNodos();
							}
							padres.top()->setNumNodos(numNodos);
						}
						padres.pop();
						pMin++;
						pMax++;
						do{
							hijos = nHijos.top() - 1;
							nHijos.pop();
							nHijos.push(hijos);
							pos = posiciones.top() + 1;
							posiciones.pop();
							posiciones.push(pos);
							if (hijos == 0){
								if (padres.size() > 1){	// Se evita el padre nullptr
									int numNodos = 1;
									for (std::size_t i = 0; i < pos; ++i){
										numNodos += padres.top()->getHijos()[i].getNumNodos();
									}
									padres.top()->setNumNodos(numNodos);
								}
								padres.pop();
								nHijos.pop();
								posiciones.pop();
							}
							else{
								actual = &(padres.top()->getHijos()[posiciones.top()]);
							}
						} while (hijos == 0 && !padres.empty());
					}
					else{
						nHijos.push(hijos);
						posiciones.push(pos);
						actual = &(padres.top()->getHijos()[pos]);
					}
				}
			}
		}
		return *a;
	}

	Nodo getTerminalAleatorio() {
		Nodo actual = _raiz;
		int numHijos = _raiz.getNhijos();
		while (!actual.esTerminal()) {
			int h = 0 + (rand() % (int)(numHijos - 0 + 1));
			actual = actual.getHijos()[h];
			numHijos = actual.getNhijos();
		}
		return actual;
	}

	Nodo getNodoFuncionAleatorio() {
		if (_raiz.getNumNodos() <= 1) {
			return _raiz;
		}
		Nodo n;
		do {
			int r = myRandom::getRandom(0, _raiz.getNumNodos()-1);
			n = buscaNodo(r);
		} while (n.esTerminal());
		return n;
	}

	int getProfMin() {
		return _profMin;
	}

	void setProfMin(int pMin) {
		this->_profMin = pMin;
	}

	int getProfMax() {
		return _profMax;
	}

	Nodo* getRaiz() {
		return &this->_raiz;
	}

	void setProfMax(int pMax) {
		this->_profMax = pMax;
	}

	double evalua(std::vector<Mapa> m, npc pnj) {
		return this->_raiz.evalua(m, pnj);
	}

	void actualizaNumNodos() {
		this->_raiz.actualizaNumNodos();
	}

	void bloating(int pMax) {
		this->_raiz.bloating(pMax, 0);
		this->_raiz.actualizaNumNodos();
	}

	void eliminaIntrones() {
		_raiz.eliminaIntrones();
		_raiz.actualizaNumNodos();
	}

	std::string toString() {
		unsigned int pos = 0;
		std::string ret = "";
		std::queue<Nodo> nodos;
		nodos.push(_raiz);
		while (!nodos.empty()){
			ret += nodos.front().toString() + "[" + std::to_string(pos) + "], ";
			for (size_t i = 0; i < nodos.front().getNhijos(); ++i){
				nodos.push(nodos.front().getHijos()[i]);
			}
			nodos.pop();
			pos++;
		}

		return ret;
	}

private:
	Nodo _raiz;
	int _profMin;
	int _profMax;
};

#endif