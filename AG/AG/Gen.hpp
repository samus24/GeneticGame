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
	}

	Gen& operator=(Gen other){
		_ancho = other._ancho;
		_alto = other._alto;
		return *this;
	}

	unsigned int getAlto(){
		return _alto;
	}

	unsigned int getAncho(){
		return _ancho;
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
	const unsigned int _MAXSIZE = 40;
	const unsigned int _MINSIZE = 5;
};

#endif