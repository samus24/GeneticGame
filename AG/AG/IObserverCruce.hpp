#ifndef IOBSERVERCRUCE_HPP
#define IOBSERVERCRUCE_HPP

#include "Cromosoma.hpp"

class IObserverCruce{
public:
	virtual ~IObserverCruce() {};

	virtual void onCruceIniciado(const Cromosoma&, const Cromosoma&, unsigned int , unsigned int) = 0;

	virtual void onCruceTerminado(const Cromosoma&, const Cromosoma&, unsigned int, unsigned int) = 0;
};

#endif