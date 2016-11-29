#ifndef METODOSELECCION_HPP
#define METODOSELECCION_HPP

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
			prob = getRandom(0, 1);
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

private:
	double getRandom(double from, double to){
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(from, to);
		return dis(gen);
	}

};


#endif