#ifndef IAGOBSERVER_HPP
#define IAGOBSERVER_HPP

#include "Cromosoma.hpp"

class IAGObserver{
public:
	virtual ~IAGObserver() {};

	virtual void onGeneracionTerminada(Cromosoma) = 0;	

	virtual void onAGTerminado(Cromosoma) = 0;
	
};

#endif