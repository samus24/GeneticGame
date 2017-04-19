#ifndef POBLACION_HPP
#define POBLACION_HPP

#include <vector>
#include <algorithm>
#include <functional>
#include <array>
#include <iterator>
#include "cromosoma.hpp"
#include "Mapa.hpp"

class poblacion {
public:
	int _tam;
	Cromosoma* individuos;

	void generaPoblacionAleatoria(int tam, int profMin, int profMax) {
		this->_tam = tam;
		this->individuos = new Cromosoma[tam];
		for (std::size_t i = 0; i < tam; ++i) {
			this->individuos[i].crear(profMin, profMax);// = Cromosoma(profMin, profMax);
		}
	}

	void evalua(std::vector<Mapa> m) {
		for (std::size_t i = 0; i < _tam; ++i) {
			for (std::size_t j = 0; j < m.size(); ++j) {
				this->individuos[i].evalua(m[j], 0, 0);
			}
		}
	}

	void ordenar(){
		std::sort(individuos, individuos + _tam, [](Cromosoma a, Cromosoma b){
			return a.getAdaptacion() > b.getAdaptacion();
		});
	}

	void bloating(unsigned int maxNodos){
		for (size_t i = 0; i < _tam; ++i){
			individuos[i].bloating(maxNodos);
		}
	}
};

#endif