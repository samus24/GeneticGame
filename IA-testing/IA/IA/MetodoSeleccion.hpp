#ifndef METODOSELECCION_HPP
#define METODOSELECCION_HPP

#include <random>
#include <ctime>
#include "poblacion.hpp"

class metodoSeleccion {
public:
	virtual void seleccionar(poblacion pob, bool maximizar) = 0;
	virtual std::string toString() = 0;

	double fRand(double fMin, double fMax)
	{
		double f = (double)rand() / RAND_MAX;
		return fMin + f * (fMax - fMin);
	}
};

class seleccionEstocastica : public metodoSeleccion {
	
	void seleccionar(poblacion pob, bool maximizar) {
		std::vector<int> seleccionados(pob.tam);
		double prob;
		int posSuper;
		double segmento = 1 / pob.tam; //El segmento es 1/N
		prob = fRand(0, segmento);
		for (std::size_t i = 0; i < pob.tam; ++i) {
			posSuper = 0;
			while ((posSuper < pob.tam) && (prob > pob.individuos[posSuper].getPuntAcum())) {
				posSuper++;
			}
			seleccionados[i] = posSuper;
			prob += segmento;
		}

		std::vector<Cromosoma> nuevaPob(pob.tam);
		for (std::size_t i = 0; i < pob.tam; ++i) {
			nuevaPob[i] = pob.individuos[seleccionados[i]];
		}

		pob.individuos = nuevaPob;
	}

	std::string toString() {
		return "Estocastico";
	}

};

#endif