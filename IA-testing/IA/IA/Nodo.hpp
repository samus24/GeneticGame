#ifndef NODO_HPP
#define NODO_HPP
#include <unordered_map>
#include <vector>
#include <string>
#include "npc.hpp"
#include "Mapa.hpp"
#include "myrandom.hpp"

enum TipoArbol{
	Patrulla,
	Ataque
};

enum Operacion {
	// No Terminales
		SiDetectado,	// Patrulla
		ProgN2,			// Comun
		ProgN3,			// Comun
		ProgN4,			// Comun
		SiBloqueado,	// Comun
		SiJugador,		// Ataque
		SiRango,		// Ataque
	// Terminales
		CambiarEst,		// Patrulla
		Avanza,			// Comun
		GiraIz,			// Comun
		GiraDer,		// Comun
		BloquearN,		// Ataque (?)
		Atacar,			// Ataque
		Retroceder		// Ataque (?)
};

std::string opToString(Operacion op){
	switch (op){
		case ProgN2:
			return "ProgN2";
		case ProgN3:
			return "ProgN3";
		case ProgN4:
			return "ProgN4";
		case SiJugador:
			return "SiJugador";
		case SiBloqueado:
			return "SiBloqueado";
		case SiRango:
			return "SiRango";
		case SiDetectado:
			return "SiDetectado";
		case CambiarEst:
			return "CambiarEst";
		case Avanza:
			return "Avanza";
		case GiraIz:
			return "GiraIz";
		case GiraDer:
			return "GiraDer";
		case BloquearN:
			return "BloquearN";
		case Atacar:
			return "Atacar";
		case Retroceder:
			return "Retroceder";
		default:
			return "Unknown";
	}
}

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
		this->_hijos = new Nodo[nHijos];
		/*
		for (std::size_t i = 0; i < _nHijos; ++i) {
			//se rellena el vector con nodos para reservar espacio, son indiferentes
			this->_hijos.push_back(Nodo(Operacion::Avanza, nullptr, 0, 0));
		}
		*/
	}


	bool addHijo(Operacion h, int pos) {
		if (pos >= _nHijos) return false;
		this->_hijos[pos] = Nodo(h, this, GRADOS[h], pos);
		/* No se si esto hará falta aquí
		for (size_t i = 0; i < h.getNhijos(); ++i){
		h.getHijos()[i].setPadre(&h);
		}
		*/
		return true;
	}


	bool addHijo(Nodo h, int pos) {
		if (pos >= _nHijos) return false;
		this->_hijos[pos] = h;
		for (size_t i = 0; i < h.getNhijos(); ++i){
			h.getHijos()[i].setPadre(&this->_hijos[pos]);
		}
		return true;
	}


	bool esTerminal() {
		return (this->_nHijos == 0);
	}

	Operacion getElem() const{
		return _elem;
	}


	void setElem(Operacion elem) {
		this->_elem = elem;
	}


	Nodo* getHijos() const{
		return _hijos;
	}


	void setHijos(Nodo* hijos) {
		this->_hijos = hijos;
	}


	int getNhijos() const{
		return _nHijos;
	}


	void setNhijos(int nHijos) {
		this->_nHijos = nHijos;
	}


	Nodo* getPadre() const{
		return _padre;
	}


	void setPadre(Nodo* padre) {
		this->_padre = padre;
	}

	int getPos() const{
		return _pos;
	}

	void setPos(int pos) {
		this->_pos = pos;
	}

	int getNumNodos() const{
		return _numNodos;
	}

	void setNumNodos(int _numNodos) {
		this->_numNodos = _numNodos;
	}

	bool esOperacionTerminal(Operacion o) {
		return ((o == Operacion::Atacar) || (o == Operacion::Avanza) || (o == Operacion::BloquearN) || (o == Operacion::CambiarEst) || (o == Operacion::GiraDer) || (o == Operacion::GiraIz) || (o == Operacion::Retroceder));
	}

	double evalua(std::vector<Mapa> m, npc pnj) {
		//aquí habría que ejecutar el arbol sobre el mapa
		return 0.f;
	}
	void bloating(int pMax, int nivel, TipoArbol tipo) {
		if (nivel == pMax) {
			this->_hijos = nullptr;
			Operacion op = Nodo::getTerminalAleatorio(tipo);
			this->_elem = op;
			this->_nHijos = 0;
		}
		else {
			for (std::size_t i = 0; i < this->_nHijos; ++i) {
				this->_hijos[i].bloating(pMax, nivel + 1, tipo);
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
					Operacion op = Nodo::getTerminalAleatorio();
					this->_elem = op;
					Nodo* _hijosAux;
					this->_hijos = _hijosAux;
					this->_nHijos = 0;
				}
			}
			else if (hijoB == Operacion::GiraDer) {
				if (hijoA == Operacion::GiraIz) {
					Operacion op = Nodo::getTerminalAleatorio();
					this->_elem = op;
					Nodo* _hijosAux;
					this->_hijos = _hijosAux;
					this->_nHijos = 0;
				}
			}
			else if (hijoA == Operacion::Avanza) {
				if (hijoB == Operacion::Retroceder) {
					Operacion op = Nodo::getTerminalAleatorio();
					this->_elem = op;
					Nodo* _hijosAux;
					this->_hijos = _hijosAux;
					this->_nHijos = 0;
				}
			}
			else if (hijoB == Operacion::Retroceder) {
				if (hijoA == Operacion::Avanza) {
					Operacion op = Nodo::getTerminalAleatorio();
					this->_elem = op;
					Nodo* _hijosAux;
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
				Nodo* _hijosAux;
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
				Nodo* _hijosAux;
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
				Nodo* _hijosAux;
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
				Nodo* _hijosAux;
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

	std::string toString() const{
		return opToString(_elem);
	}

	bool operator==(const Nodo &other) const{
		if (_padre == other.getPadre()){
			if (_nHijos == other.getNhijos()){
				if (_elem == other.getElem()){
					if (_hijos == other.getHijos()){
						if (_numNodos == other.getNumNodos()){
							if (_pos == other.getPos()){
								return true;
							}
						}
					}
				}
			}
		}
		return false;
	}

	bool operator!=(const Nodo &other) const{
		return !operator==(other);
	}

public:
	static Operacion getTerminalAleatorio(){
		return (Operacion)myRandom::getRandom(7, 13);
	}

	static Operacion getNoTerminalAleatorio(){
		return (Operacion)myRandom::getRandom(0, 6);
	}

	static Operacion getElementoAleatorio(){
		return (Operacion)myRandom::getRandom(0, 13);
	}

	static Operacion getTerminalAleatorio(TipoArbol tipo){
		if (tipo == Ataque){
			return (Operacion)myRandom::getRandom(Operacion::Avanza, Operacion::Retroceder);
		}
		else{// if (tipo == Patrulla){
			return (Operacion)myRandom::getRandom(Operacion::CambiarEst, Operacion::GiraDer);
		}
	}

	static Operacion getNoTerminalAleatorio(TipoArbol tipo){
		if (tipo == Ataque){
			return (Operacion)myRandom::getRandom(Operacion::ProgN2, Operacion::SiRango);
		}
		else{// if (tipo == Patrulla){
			return (Operacion)myRandom::getRandom(Operacion::SiDetectado, Operacion::SiBloqueado);
		}
	}

	static Operacion getElementoAleatorio(TipoArbol tipo){
		if (tipo == Ataque){
			Operacion elem;
			do{
				elem = (Operacion)myRandom::getRandom(Operacion::ProgN2, Operacion::Retroceder);
			} while (elem == Operacion::CambiarEst);
			return elem;
		}
		else{// if (tipo == Patrulla){
			Operacion elem;
			do{
				elem = (Operacion)myRandom::getRandom(Operacion::SiDetectado, Operacion::GiraDer);
			} while (elem == Operacion::SiJugador ||elem == Operacion::SiRango);
			return elem;
		}
	}


private:
	Nodo* _padre;
	int _nHijos;
	Operacion _elem;
	//std::vector<Nodo> _hijos;
	Nodo* _hijos;
	int _numNodos;
	int _pos;
};



#endif