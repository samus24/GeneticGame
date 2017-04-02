#ifndef METODOSELECCION_HPP
#define METODOSELECCION_HPP

#include <random>
#include <ctime>
#include "poblacion.hpp"

class metodoSeleccion {
public:
	virtual void seleccionar(poblacion pob, bool maximizar) = 0;
	virtual std::string toString() = 0;
};

class seleccionEstocastica : public metodoSeleccion {
	
	void seleccionar(poblacion pob, bool maximizar) {
		std::vector<int> seleccionados(pob.tam);
		double prob;
		int posSuper;
		double segmento = 1 / pob.tam; //El segmento es 1/N
		prob = myRandom::getRandom(0.f, segmento);
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

class seleccionRuleta : public metodoSeleccion {
	void seleccionar(poblacion pob, bool maximizar) {
		std::vector<int> seleccionados(pob.tam);
		double prob;
		int posSuper;
		for (std::size_t i = 0; i < pob.tam; ++i) {
			prob = myRandom::getRandom(0.f, 1.f); //un double entre 0.0 y 1.0
			posSuper = 0;
			while ((posSuper < pob.tam) && (prob > pob.individuos[posSuper].getPuntAcum()))
				posSuper++;
			seleccionados[i] = posSuper;
		}
		std::vector<Cromosoma> nuevaPob(pob.tam);
		for (std::size_t i = 0; i < pob.tam; ++i) {
			nuevaPob[i] = pob.individuos[seleccionados[i]];
		}

		pob.individuos = nuevaPob;
	}

	std::string toString() {
		return "Ruleta";
	}
};

class seleccionTorneo : public metodoSeleccion {

	void seleccionar(poblacion pob, bool maximizar) {
		std::vector<int> seleccionados(pob.tam);
		int posMejor, indexA, indexB, indexC;

		for (std::size_t i = 0; i < pob.tam; ++i) {
			indexA = (int)(myRandom::getRandom(0.f, 1.f) * pob.tam);
			do {
				indexB = (int)(myRandom::getRandom(0.f, 1.f) * pob.tam);
			} while (indexB == indexA);
			do {
				indexC = (int)(myRandom::getRandom(0.f, 1.f) * pob.tam);
			} while (indexB == indexC || indexA == indexC);

			if (maximizar) {
				if (pob.individuos[indexA].getAdaptacion() > pob.individuos[indexB].getAdaptacion()) {
					if (pob.individuos[indexA].getAdaptacion() > pob.individuos[indexC].getAdaptacion()) {
						posMejor = indexA;
					}
					else {
						posMejor = indexC;
					}
				}
				else {
					if (pob.individuos[indexC].getAdaptacion() > pob.individuos[indexB].getAdaptacion()) {
						posMejor = indexC;
					}
					else {
						posMejor = indexB;
					}
				}
			}
			else {
				if (pob.individuos[indexA].getAdaptacion() < pob.individuos[indexB].getAdaptacion()){
					if (pob.individuos[indexA].getAdaptacion() < pob.individuos[indexC].getAdaptacion()){
						posMejor = indexA;
					}
					else{
						posMejor = indexC;
					}
				}
				else{
					if (pob.individuos[indexC].getAdaptacion() < pob.individuos[indexB].getAdaptacion()){
						posMejor = indexC;
					}
					else{
						posMejor = indexB;
					}
				}
			}
			seleccionados[i] = posMejor;
		}
		std::vector<Cromosoma> nuevaPob(pob.tam);
		for (std::size_t i = 0; i < pob.tam; ++i) {
			nuevaPob[i] = pob.individuos[seleccionados[i]];
		}

		pob.individuos = nuevaPob;
	}

	std::string toString() {
		return "Torneo";
	}
};

#endif