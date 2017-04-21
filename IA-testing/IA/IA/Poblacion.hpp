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
			// Esto hay que cambiarlo, se le pasan a cada individuo TODOS los mapas, y él se evalua con cada uno y se hace una media
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

	void bloating(unsigned int maxNodos, std::vector<Mapa> m){
		for (size_t i = 0; i < _tam; ++i){
			// Esto habrá que ponerlo bien, con todos los mapas
			individuos[i].bloating(maxNodos, m[0]);
		}
	}
};

#endif