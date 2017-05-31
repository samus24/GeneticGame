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
	unsigned int _tam;

	Poblacion(){

	}

	void generaPoblacionAleatoria(unsigned int tam, unsigned int minNodos, unsigned int maxNodos, double densidad){
		individuos.clear();
		_tam = tam;
		for (std::size_t i = 0; i < tam; ++i){
			individuos.push_back(Cromosoma(minNodos, maxNodos, densidad));
		}
	}

	void evalua(){
		for (size_t i = 0; i < individuos.size(); ++i){
			individuos[i].evalua();
		}
	}

	void evaluaMarcados(std::set<unsigned int> marcados) {
		for (auto it = marcados.begin(); it != marcados.end(); ++it){
			this->individuos[(*it)].evalua();
		}
	}

	void ordenar(){
		std::sort(individuos.begin(), individuos.end(), [](Cromosoma a, Cromosoma b){
			return a.getAdaptacion() > b.getAdaptacion();
		});
	}

	void bloating(unsigned int maxNodos, std::set<unsigned int> marcados){
		for (size_t i = 0; i < individuos.size(); ++i){
			individuos[i].bloating(maxNodos);
			marcados.insert(i);
		}
	}

};

#endif