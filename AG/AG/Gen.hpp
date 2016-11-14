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

private:

	int getRandom(int from, int to){
		float random = (float)(rand() / (float)RAND_MAX);
		return from + random * (to - from);
	}

	unsigned int _ancho;
	unsigned int _alto;
	const unsigned int _MAXSIZE = 40;
	const unsigned int _MINSIZE = 5;
};

#endif