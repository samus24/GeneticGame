#ifndef ICROMOSOMAOBSERVER_HPP
#define ICROMOSOMAOBSERVER_HPP


#include "npc.hpp"
#include "Mapa.hpp"
#include "Cromosoma.hpp"

class Cromosoma;

class ICromosomaObserver{
public:
	virtual ~ICromosomaObserver() {};

	virtual void onSimulacionIniciada(const Cromosoma*) = 0;

	virtual void onTurno(const Cromosoma*, npc, npc, Mapa, Mapa, Mapa, Mapa) = 0;	// jugador, enemigo, mapa, explorado, andado

	virtual void onMapaTerminado(double fitness) = 0;

	virtual void onSimulacionTerminada(const Cromosoma*) = 0;
};

#endif