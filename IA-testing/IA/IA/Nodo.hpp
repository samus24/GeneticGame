#ifndef NODO_HPP
#define NODO_HPP
#include <unordered_map>
#include <vector>
#include "npc.hpp"
#include "Mapa.hpp"

enum Operacion {
	ProgN2,
	ProgN3,
	ProgN4,
	SiJugador,
	SiBloqueado,
	SiRango,
	SiDetectado,
	Avanza,
	GiraIz,
	GiraDer,
	CambiarEst,
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
	{ Avanza, 0 },
	{ GiraIz, 0 },
	{ GiraDer, 0 },
	{ CambiarEst, 0 },
	{ BloquearN, 0 },
	{ Atacar, 0 },
	{ Retroceder, 0 }
};

class Nodo {
public:	

	Nodo(Operacion _elem, Nodo* padre, int nHijos, int _pos);

	bool addHijo(Operacion h, int _pos);

	bool addHijo(Nodo h, int _pos);

	bool esTerminal();

	Operacion getElem();

	void setElem(Operacion elem);

	std::vector<Nodo> getHijos();

	void setHijos(std::vector<Nodo> hijos);

	int getNhijos();

	void setNhijos(int nHijos);

	Nodo* getPadre();

	void setPadre(Nodo* padre);

	int getNumNodos();

	void setNumNodos(int _numNodos);

	int getPos();

	void setPos(int _pos);

	void evalua(std::vector<Mapa> m, npc pnj);

private:
	Nodo* _padre;
	int _nHijos;
	Operacion _elem;
	std::vector<Nodo> _hijos;
	int _numNodos;
	int _pos;
};

#endif