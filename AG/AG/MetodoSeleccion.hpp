#ifndef METODOSELECCION_HPP
#define METODOSELECCION_HPP

#include "RandomGen.hpp"
#include"Poblacion.hpp"

class MetodoSeleccion {
public:

	virtual void seleccionar(Poblacion*) = 0;

};

class SeleccionRuleta : public MetodoSeleccion {
public:
	void seleccionar(Poblacion* pob){
		int* seleccionados = new int[pob->_tam];
		double prob;
		int posSuper;
		for (int i = 0; i < pob->_tam; ++i){
			prob = RandomGen::getRandom(0.f, 1.f);
			posSuper = 0;
			while ((posSuper < pob->_tam) && (prob > pob->individuos[posSuper].getPuntAcum()))
				posSuper++;
			seleccionados[i] = posSuper;
		}
		std::vector<Cromosoma> nuevaPob;
		for (int i = 0; i < pob->_tam; ++i){
			nuevaPob.push_back(pob->individuos[seleccionados[i]]);
		}
		pob->individuos = nuevaPob;
	}

};


#endif