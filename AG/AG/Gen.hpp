#ifndef GEN_HPP
#define GEN_HPP

#include <time.h>
#include <random>

class Gen{
public:
	Gen(){
		std::srand(time(NULL));
		_ancho = getRandom(_MINSIZE, _MAXSIZE);
		_alto = getRandom(_MINSIZE, _MAXSIZE);
		_nEnemigos = getRandom(_MINENEMIGOS, _MAXENEMIGOS);
		_nCofres = getRandom(_MINCOFRES, _MAXCOFRES);
	}

	Gen& operator=(Gen other){
		_ancho = other._ancho;
		_alto = other._alto;
		_nEnemigos = other._nEnemigos;
		_nCofres = other._nCofres;
		return *this;
	}

	unsigned int getAlto(){
		return _alto;
	}

	unsigned int getAncho(){
		return _ancho;
	}

	unsigned int getEnemigos(){
		return _nEnemigos;
	}

	unsigned int getCofres(){
		return _nCofres;
	}

	static int getRandom(int from, int to){
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(from, to);
		return dis(gen);
	}
private:
	unsigned int _ancho;
	unsigned int _alto;
	unsigned int _nEnemigos;
	unsigned int _nCofres;
	const unsigned int _MAXSIZE = 40;
	const unsigned int _MINSIZE = 5;
	const unsigned int _MINENEMIGOS = 0;
	const unsigned int _MAXENEMIGOS = 8;
	const unsigned int _MINCOFRES = 0;
	const unsigned int _MAXCOFRES = 2;
};

#endif