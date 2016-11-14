#ifndef CROMOSOMA_HPP
#define CROMOSOMA_HPP

#include "Grafo.hpp"
#include "Gen.hpp"

class Cromosoma {
public:

	double getPunt(){
		return _punt;
	}

	double getPuntAcum(){
		return _puntAcum;
	}

	double getAdaptacion(){
		return _adaptacion;
	}

	void setPunt(double punt){
		_punt = punt;
	}

	void setPuntAcum(double puntAcum){
		_puntAcum = puntAcum;
	}

	void setAdaptacion(double adaptacion){
		_adaptacion = adaptacion;
	}

	double evalua(){
		//[NumNodos, Media - Grado - CC, media - tamaño - sala, penalizar - ciclos]
		//[25, 2.5, 30x20(favorece la resolución), min(ciclos)]
		double pesos[] = {0.25, 0.25, 0.25, 0.25}; // {NumNodos,  MediaGrad, MediaTam, Ciclos}
		std::vector<double> puntuaciones;
		unsigned int numNodosCC;
		unsigned int mejorCC;
		unsigned int mejorPunt;
		std::vector<std::set<unsigned int>> CC = _grafo.getComponentesConexas();

		/*for (std::size_t i = 0;){
		}*/
	}

private:
	Grafo<Gen> _grafo;

	double _punt;
	double _puntAcum;
	double _adaptacion;

	const unsigned int _NODOS_OPTIMOS_CC = 25;
	const double _GRADO_OPTIMO_CC = 2.5;
	const unsigned int _ANCHO_OPTIMO = 30;
	const unsigned int _ALTO_OPTIMO = 20;
};

#endif