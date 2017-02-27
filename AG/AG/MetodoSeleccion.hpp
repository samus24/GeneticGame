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

class SeleccionEstocastica : public MetodoSeleccion {
public:
	void seleccionar(Poblacion* pob) {
		int* seleccionados = new int[pob->_tam];
		double prob;
		int posSuper;
		double seg = 1 / pob->_tam;	
		prob = RandomGen::getRandom(0.f, 1.f) * seg;		
		for (int i = 0; i < pob->_tam; ++i){
			posSuper = 0;
			while ((posSuper < pob->_tam) && (prob > pob->individuos[posSuper].getPuntAcum()))
				posSuper++;
			seleccionados[i] = posSuper;
			prob += seg;
		}
		std::vector<Cromosoma> nuevaPob;
		for (int i = 0; i < pob->_tam; ++i){
			nuevaPob.push_back(pob->individuos[seleccionados[i]]);
		}
		pob->individuos = nuevaPob;
	}
};

class SeleccionTorneo : public MetodoSeleccion {
public:
	void seleccionar(Poblacion* pob) {
		int* seleccionados = new int[pob->_tam];
		int posMejor;
		int indexA, indexB, indexC;
		for (int i = 0; i < pob->_tam; ++i){
			// Torneo con muestreo diferenciado (cada elemento solo una vez)
			indexA = RandomGen::getRandom(0u, pob->_tam-1);
			do{
				indexB = RandomGen::getRandom(0u, pob->_tam-1);
			} while (indexB == indexA);
			do{
				indexC = RandomGen::getRandom(0u, pob->_tam-1);
			} while (indexB == indexC || indexA == indexC);

			if (pob->individuos[indexA].getAdaptacion() > pob->individuos[indexB].getAdaptacion()){
				if (pob->individuos[indexA].getAdaptacion() > pob->individuos[indexC].getAdaptacion()){
					posMejor = indexA;
				}
				else{
					posMejor = indexC;
				}
			}
			else{
				if (pob->individuos[indexC].getAdaptacion() > pob->individuos[indexB].getAdaptacion()){
					posMejor = indexC;
				}
				else{
					posMejor = indexB;
				}
			}

			seleccionados[i] = posMejor;
		}

		std::vector<Cromosoma> nuevaPob;
		for (int i = 0; i < pob->_tam; ++i){
			nuevaPob.push_back(pob->individuos[seleccionados[i]]);
		}
		pob->individuos = nuevaPob;
	}
};


#endif