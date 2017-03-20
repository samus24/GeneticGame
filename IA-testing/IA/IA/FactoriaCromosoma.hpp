#ifndef FACTORIA_HPP
#define FACTORIA_HPP

#include "cromosoma.hpp"

class FactoriaCromosoma {
public:
	FactoriaCromosoma(int pMin, int pMax) {
		this->_profMin = pMin;
		this->_profMax = pMax;
	}

	cromosoma creaCromosoma() {
		return cromosoma(_profMin, _profMax);
	}

	int getProfMin() {
		return _profMin;
	}

	int getProfMax() {
		return _profMax;
	}

private:
	int _profMin;
	int _profMax;
};

#endif