#ifndef NODO_HPP
#define NODO_HPP
#include <unordered_map>
#include <vector>

enum Operacion {
	ProgN2,
	ProgN3,
	ProgN4,
	SiJugador,
	SiBloqueado,
	Avanza,
	GiraIz,
	GiraDer,
	CambiarEst,
	SiRango,
	SiDetectado,
	BloquearN,
	Atacar,
	Retroceder
};

template <class N> 
class Nodo {
public:
	static const std::unordered_map<Operacion, int> grados{
		{ ProgN2, 2 }
		{ ProgN3, 3 }
		{ ProgN4, 4 }
		{ SiJugador, 2 }
		{ SiBloqueado, 2 }
		{ Avanza, 0 }
		{ GiraIz, 0 }
		{ GiraDer, 0 }
		{ CambiarEst, 0 }
		{ SiRango, 2 }
		{ SiDetectado, 2 }
		{ BloquearN, 0 }
		{ Atacar, 0 }
		{ Retroceder, 0 }
	};
	

	Nodo<N>(N _elem, Nodo<N> padre, int nHijos) {
		this->_padre = padre;
		this->_nHijos = nHijos;
		this->_elem = elem
	}

	bool addHijo(N h, int pos) {
		if (pos >= _nHijos) return false;
		this->_hijos.at(pos) = Nodo<N>(h, this->, grados.get(h));
		return true;
	}

	bool addHijo(Nodo<N> h, int pos) {
		if (pos >= nHijos) return false;
		this->_hijos.at(pos) = h;
		return true;
	}

	void addHijo(N h) {
		this->_hijos.push_back(Nodo<N>(h, this->, grados.get(h));
		this->_nHijos++;
	}
	void addHijo(Nodo<N> h) {
		this->_hijos.push_back(h);
		this->_nHijos++;
	}

	bool esTerminal() {
		return (this->_nHijos == 0);
	}

	N getElem() {
		return _elem;
	}

	void setElem(N elem) {
		this->_elem = elem;
	}

	std::vector<Nodo<N>> getHijos() {
		return _hijos;
	}

	void setHijos(std::vector<Nodo<N>> hijos) {
		this->_hijos = hijos;
	}

	int getNhijos() {
		return _nHijos;
	}

	void setNhijos(int nHijos) {
		this->_nHijos = nHijos;
	}

	Nodo<N> getPadre() {
		return _padre;
	}

	void setPadre(Nodo<N> padre) {
		this->_padre = padre;
	}

private:
	Nodo<N> _padre;
	int _nHijos;
	N _elem;
	std::vector<Nodo<N>> _hijos
};

#endif