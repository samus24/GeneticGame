#ifndef CROMOSOMA_HPP
#define CROMOSOMA_HPP

#include "Grafo.hpp"
#include "Gen.hpp"
#include <cmath>

class Cromosoma {
public:
	Cromosoma(){
		_punt = 0;
		_puntAcum = 0;
		_adaptacion = 0;
	}

	Cromosoma(unsigned int minNodos, unsigned int maxNodos, double densidad) :
		_grafo(minNodos, maxNodos, densidad)
	{
		
	}

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
		// IMPORTANTE, contamos con que la funcion evalua establece automaticamente el valor de _adaptacion
		double mejorCC = -1;
		double componente;
		std::vector<Grafo<Gen>::ComponenteConexa<Gen>> CC = _grafo.getComponentesConexas();
		for (std::size_t i = 0; i < CC.size(); ++i) {
			componente = evaluaCC(CC[i]);
			if (componente > mejorCC)
				mejorCC = componente;
		}
		this->_adaptacion = mejorCC;
		return _adaptacion;
	}
	
	void bloating(unsigned int maxNodos){
		if (_grafo.size() > maxNodos)
			_grafo = _grafo.divideGrafo(maxNodos).at(0);
	}

	Cromosoma& operator=(Cromosoma other){
		std::swap(_grafo, other._grafo);
		_adaptacion = other.getAdaptacion();
		_punt = other.getPunt();
		_puntAcum = other.getPuntAcum();
		return *this;
	}

private:
	double evaluaCC(Grafo<Gen>::ComponenteConexa<Gen> CC) {
		//[NumNodos, Media - Grado - CC, media - tama�o - sala, penalizar - ciclos]
		//[25, 2.5, 30x20(favorece la resoluci�n), min(ciclos)]
		double pesos[] = { 0.3, 0.4, 0.05, 0.05, 0.2 }; // {NumNodos,  MediaGrad, MediaAlto, MediaAncho, Ciclos} //me los he inventado los pesos
		double valores[5];
		double evaluacion = 0;

		int numNodos = CC.size();
		double mediaGrado = 0;
		double mediaAlto = 0;
		double mediaAncho = 0;
		double diferencias[5];
		int ciclosActuales = _CICLOS_OPTIMOS;
		//double diferencias[5];
		auto adyacencia = CC.getAdyacencia();
		auto itAdy = adyacencia.begin();

		while (itAdy != adyacencia.cend()){
			mediaGrado += itAdy->second.size();
			itAdy++;
		}
		mediaGrado /= numNodos;

		auto nodos = CC.getNodos();
		auto itNodos = nodos.begin();
		while (itNodos != nodos.cend()){
			mediaAlto += itNodos->second.getAlto();
			mediaAncho += itNodos->second.getAncho();
			itNodos++;
		}
		mediaAlto /= numNodos;
		mediaAncho /= numNodos;
		// 1 - abs(x - ideal) / ideal;
		//[NumNodos, Media - Grado - CC, media - tama�o - sala, penalizar - ciclos]
		valores[0] = 1 - (abs(numNodos - _NODOS_OPTIMOS_CC) / (float)_NODOS_OPTIMOS_CC);
		if (valores[0] < 0)	valores[0] = 0;

		valores[1] = 1 - (abs(mediaGrado - _GRADO_OPTIMO_CC) / _GRADO_OPTIMO_CC);
		if (valores[1] < 0)	valores[1] = 0;

		valores[2] = 1 - (abs(mediaAlto - _ALTO_OPTIMO) / (float)_ALTO_OPTIMO);
		if (valores[2] < 0)	valores[2] = 0;

		valores[3] = 1 - (abs(mediaAncho - _ANCHO_OPTIMO) / (float)_ANCHO_OPTIMO);
		if (valores[3] < 0)	valores[3] = 0;

		valores[4] = 1 - (abs(ciclosActuales - _CICLOS_OPTIMOS) / (float)_CICLOS_OPTIMOS);
		if (valores[4] < 0)	valores[4] = 0;

		for (int i = 0; i < 5; ++i){
			evaluacion += valores[i] * pesos[i];
		}
		//ciclosActuales = CC.getCiclos(); //o función que devuelva los ciclos

		/*
		(numNodos < _NODOS_OPTIMOS_CC) ? diferencias[0] = std::abs(numNodos - _NODOS_OPTIMOS_CC) / _NODOS_OPTIMOS_CC :	diferencias[0] = std::abs(numNodos - _NODOS_OPTIMOS_CC) / numNodos;

		(mediaGrado < _GRADO_OPTIMO_CC) ? diferencias[1] = std::abs(mediaGrado - _GRADO_OPTIMO_CC) / _GRADO_OPTIMO_CC : diferencias[1] = std::abs(mediaGrado - _GRADO_OPTIMO_CC) / mediaGrado;

		(mediaAlto < _ALTO_OPTIMO) ? diferencias[2] = std::abs(mediaAlto - _ALTO_OPTIMO) / _ALTO_OPTIMO :	diferencias[2] = std::abs(mediaAlto - _ALTO_OPTIMO) / mediaAlto;

		(mediaAncho < _ANCHO_OPTIMO) ? diferencias[3] = std::abs(mediaAncho - _ANCHO_OPTIMO) / _ANCHO_OPTIMO : diferencias[3] = std::abs(mediaAncho - _ANCHO_OPTIMO) / mediaAncho;

		(ciclosActuales < _CICLOS_OPTIMOS) ? diferencias[4] = std::abs(ciclosActuales - _CICLOS_OPTIMOS) / _CICLOS_OPTIMOS : diferencias[4] = std::abs(ciclosActuales - _CICLOS_OPTIMOS) / ciclosActuales;

		for (std::size_t i = 0; i < 5; ++i) {
		evaluacion -= diferencias[i] * pesos[i];
		}
		*/

		return evaluacion;
	}

	Grafo<Gen> _grafo;

	double _punt;
	double _puntAcum;
	double _adaptacion;

	const int _NODOS_OPTIMOS_CC = 25;
	const double _GRADO_OPTIMO_CC = 2.5;
	const int _ANCHO_OPTIMO = 30;
	const int _ALTO_OPTIMO = 20;
	const int _CICLOS_OPTIMOS = 3;
};

#endif