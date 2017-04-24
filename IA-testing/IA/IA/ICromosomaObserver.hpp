#ifndef ICROMOSOMAOBSERVER_HPP
#define ICROMOSOMAOBSERVER_HPP

#include "npc.hpp"
#include "Mapa.hpp"

class ICromosomaObserver{
public:
	virtual ~ICromosomaObserver() {};

	virtual void onTurno(Arbol, Arbol, npc, npc, Mapa, Mapa) = 0;	// jugador, enemigo, mapa, explorado

	virtual void onSimulacionTerminada(double) = 0;
};

#endif