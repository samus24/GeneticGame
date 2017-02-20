#ifndef CROMOSOMA_HPP
#define CROMOSOMA_HPP

#include "Grafo.hpp"
#include "Gen.hpp"
#include "ParametrosEval.hpp"
#include <cmath>

class Cromosoma {
public:
	Cromosoma(){
		_punt = 0;
		_puntAcum = 0;
		_adaptacion = 0;
		_indexMejorCC = 0;
	}

	Cromosoma(unsigned int minNodos, unsigned int maxNodos, double densidad) :
		_grafo(minNodos, maxNodos, densidad)
	{
		
	}

	Grafo<Gen> getGenotipo() const{
		return _grafo;
	}
	
	Grafo<Gen> getMejorCC() const{
		return _grafo.getComponentesConexas()[_indexMejorCC];
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

	void setGenotipo(Grafo<Gen> g, ParametrosEval param){
		_grafo = g;
		evalua(param);
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

	double evalua(ParametrosEval param){
		// IMPORTANTE, contamos con que la funcion evalua establece automaticamente el valor de _adaptacion
		double mejorCC = -1;
		_indexMejorCC = -1;
		double componente;
		std::vector<Grafo<Gen>::ComponenteConexa<Gen>> CC = _grafo.getComponentesConexas();
		for (std::size_t i = 0; i < CC.size(); ++i) {
			componente = evaluaCC(CC[i], param);
			if (componente > mejorCC){
				mejorCC = componente;
				_indexMejorCC = i;
			}
		}
		this->_adaptacion = mejorCC;
		return _adaptacion;
	}
	
	void bloating(unsigned int maxNodos, ParametrosEval param){
		if (_grafo.size() > maxNodos){
			_grafo = _grafo.divideGrafo(maxNodos).at(0);
			evalua(param);
		}
	}

	Cromosoma& operator=(Cromosoma other){
		std::swap(_grafo, other._grafo);
		_adaptacion = other.getAdaptacion();
		_punt = other.getPunt();
		_puntAcum = other.getPuntAcum();
		return *this;
	}

private:
	double evaluaCC(Grafo<Gen>::ComponenteConexa<Gen> CC, ParametrosEval param) {
		//[NumNodos, Media - Grado - CC, media - tama�o - sala, penalizar - ciclos]
		//[25, 2.5, 30x20(favorece la resoluci�n), min(ciclos)]
		double pesos[] = { 0.275, 0.45, 0.05, 0.05, 0.075, 0.05, 0.05 }; // {NumNodos,  MediaGrad, MediaAlto, MediaAncho, Ciclos, NumEnemigos, NumCofres}
		double valores[7];
		double evaluacion = 0;

		int numNodos = CC.size();
		double mediaGrado = 0;
		double mediaAlto = 0;
		double mediaAncho = 0;
		double diferencias[5];
		int ciclosActuales;
		int enemigosActuales = 0;
		int cofresActuales = 0;
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
			enemigosActuales += itNodos->second.getEnemigos();
			cofresActuales += itNodos->second.getCofres();
			itNodos++;
		}
		mediaAlto /= numNodos;
		mediaAncho /= numNodos;
		ciclosActuales = CC.tieneCiclos();

		// 1 - abs(x - ideal) / ideal;
		//[NumNodos, Media - Grado - CC, media - tama�o - sala, penalizar - ciclos]
		valores[0] = 1 - (abs(int(numNodos - param.nodosOptimos)) / (float)param.nodosOptimos);
		if (valores[0] < 0)	valores[0] = 0;

		valores[1] = 1 - (abs(mediaGrado - param.gradoOptimo) / param.gradoOptimo);
		if (valores[1] < 0)	valores[1] = 0;

		valores[2] = 1 - (abs(mediaAlto - param.altoOptimo) / (float)param.altoOptimo);
		if (valores[2] < 0)	valores[2] = 0;

		valores[3] = 1 - (abs(mediaAncho - param.anchoOptimo) / (float)param.anchoOptimo);
		if (valores[3] < 0)	valores[3] = 0;

		valores[4] = 1 - (abs(int(ciclosActuales - param.ciclosOptimos)) / (float)param.ciclosOptimos);
		if (valores[4] < 0)	valores[4] = 0;

		valores[5] = 1 - (abs(int(enemigosActuales - param.enemigosOptimos)) / (float)param.enemigosOptimos);
		if (valores[5] < 0)	valores[5] = 0;

		valores[6] = 1 - (abs(int(cofresActuales - param.cofresOptimos)) / (float)param.cofresOptimos);
		if (valores[6] < 0)	valores[6] = 0;

		for (int i = 0; i < 7; ++i){
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
	unsigned int _indexMejorCC;
};

#endif