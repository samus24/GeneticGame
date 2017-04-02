#ifndef POBLACION_HPP
#define POBLACION_HPP

#include <vector>
#include <algorithm>
#include <functional>
#include <array>
#include "cromosoma.hpp"
#include "Mapa.hpp"

class poblacion {
public:
	int _tam;
	std::vector<Cromosoma> individuos;

	void generaPoblacionAleatoria(int tam, int profMin, int profMax) {
		this->_tam = tam;
		this->individuos = std::vector<Cromosoma>(tam);
		for (std::size_t i = 0; i < tam; ++i) {
			this->individuos[i] = Cromosoma(profMin, profMax);
		}
	}

	void evalua(std::vector<Mapa> m) {
		for (std::size_t i = 0; i < _tam; ++i) {
			for (std::size_t j = 0; j < m.size(); ++j) {
				this->individuos[i].evalua(m[j]);
			}
		}
	}

	void ordenar(){
		std::sort(individuos.begin(), individuos.end(), [](Cromosoma a, Cromosoma b){
			return a.getAdaptacion() > b.getAdaptacion();
		});
	}

	void bloating(unsigned int maxNodos){
		for (size_t i = 0; i < individuos.size(); ++i){
			individuos[i].bloating(maxNodos);
		}
	}
};

#endif