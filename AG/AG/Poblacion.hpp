#ifndef POBLACION_HPP
#define POBLACION_HPP

#include <vector>
#include <algorithm>
#include <functional>
#include <array>

#include "Cromosoma.hpp"

class Poblacion {
public:
	std::vector<Cromosoma> individuos;

	Poblacion(){

	}

	void generaPoblacionAleatoria(unsigned int tam, unsigned int minNodos, unsigned int maxNodos, double densidad){
		_tam = tam;
		for (std::size_t i = 0; i < tam; ++i){
			individuos.push_back(Cromosoma(minNodos, maxNodos, densidad);
		}
	}

	void evalua(){
		for (Cromosoma i : individuos){
			i.evalua();
		}
	}

	void ordenar(){
		std::sort(individuos.begin(), individuos.end(), [](Cromosoma a, Cromosoma b){
			return a.evalua() > b.evalua();
		});
	}

	void bloating(unsigned int maxNodos){
		for (Cromosoma i : individuos){
			i.bloating(maxNodos);
		}
	}

private:
	unsigned int _tam;
	
};

#endif