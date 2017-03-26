#ifndef POBLACION_HPP
#define POBLACION_HPP

#include <vector>
#include "cromosoma.hpp"
#include "Mapa.hpp"

class poblacion {
public:
	int tam;
	std::vector<Cromosoma> individuos;

	void generaPoblacionAleatoria(int tam, int profMin, int profMax) {
		this->tam = tam;
		this->individuos = std::vector<Cromosoma>(tam);
		for (std::size_t i = 0; i < tam; ++i) {
			this->individuos[i] = Cromosoma(profMin, profMax);
		}
	}

	void evalua(std::vector<Mapa> m) {
		for (std::size_t i = 0; i < tam; ++i) {
			for (std::size_t j = 0; j < m.size(); ++j) {
				this->individuos[i].evalua(m[j]);
			}
		}
	}
};

#endif