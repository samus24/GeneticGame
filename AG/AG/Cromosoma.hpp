#ifndef CROMOSOMA_HPP
#define CROMOSOMA_HPP

#include "Grafo.hpp"
#include "Gen.hpp"
#include "ParametrosEval.hpp"
#include <cmath>
#include <queue>
#include <iostream>

enum RolSala{
	Inicio,
	Fin,
	Llave,
	RolCount
};

class Cromosoma {
public:
	Cromosoma(){
		_punt = 0;
		_puntAcum = 0;
		_adaptacion = 0;
		_indexMejorCC = 0;

		double auxpesos[] = { 0.5, 0.16, 0.03, 0.02, 0, 0.19, 0.10 };	// {NumNodos,  MediaGrad, MediaAlto, MediaAncho, Ciclos, NumEnemigos, NumCofres}
		for (size_t i = 0; i < 7; ++i){
			_pesos[i] = auxpesos[i];
		}
	}

	Cromosoma(unsigned int minNodos, unsigned int maxNodos, double densidad) :
		_grafo(minNodos, maxNodos, densidad),
		_rolesSala(RolSala::RolCount)
	{
		_rolesSala[RolSala::Inicio] = RandomGen::getRandom(0, int(_grafo.size() - 1));
		do{
			_rolesSala[RolSala::Fin] = RandomGen::getRandom(0, int(_grafo.size() - 1));
		} while (_rolesSala[RolSala::Inicio] == _rolesSala[RolSala::Fin]);

		do{
			_rolesSala[RolSala::Llave] = RandomGen::getRandom(0, int(_grafo.size() - 1));
		} while (_rolesSala[RolSala::Llave] == _rolesSala[RolSala::Inicio] || _rolesSala[RolSala::Llave] == _rolesSala[RolSala::Fin]);

		double auxpesos[] = { 0.5, 0.16, 0.03, 0.02, 0, 0.19, 0.10 };	// {NumNodos,  MediaGrad, MediaAlto, MediaAncho, Ciclos, NumEnemigos, NumCofres}
		for (size_t i = 0; i < 7; ++i){
			_pesos[i] = auxpesos[i];
		}
	}

	Grafo<Gen> getGenotipo() const{
		return _grafo;
	}
	
	Grafo<Gen> getMejorCC() const{
		return _grafo.getComponentesConexas()[_indexMejorCC];
	}

	unsigned int getIndexElMejor() const{
		return _indexMejorCC;
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

	double* getValores(){
		return _mejoresValores;
	}

	double* getPesos(){
		return _pesos;
	}

	std::vector<unsigned int> getRolesSala() const{
		return _rolesSala;
	}

	void setRolesSala(std::vector<unsigned int> roles){
		_rolesSala = roles;
	}

	void setIndexElMejor(unsigned int i){
		_indexMejorCC = i;
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
		double mejorCC = -999999999999999;
		_indexMejorCC = -1;
		double componente;
		std::vector<Grafo<Gen>::ComponenteConexa<Gen>> CC = _grafo.getComponentesConexas();
		int tam = CC.size();
		for (std::size_t i = 0; i < CC.size(); ++i) {
			componente = evaluaCC(CC[i], param);
			if (componente > mejorCC){
				mejorCC = componente;
				_indexMejorCC = i;
				for (size_t i = 0; i < 7; ++i){
					_mejoresValores[i] = _valores[i];
				}
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
		_rolesSala = other.getRolesSala();
		return *this;
	}

private:
	double evaluaCC(Grafo<Gen>::ComponenteConexa<Gen> CC, ParametrosEval param) {
		/*
		La nueva funcion de evaluacion debe:
		- Minimizar el numero de nodos de la CC						:: Pendiente
		- Maximizar las distancias entre Inicio-Llave y Llave-Fin	:: Pendiente
		- Minimizar la media de grado de las salas					:: Pendiente
		- Maximizar la dispersion de enemigos y cofres				:: Pendiente
		*/
		unsigned int numNodos = CC.size();
		unsigned int distanciaTotal = 0;
		if (CC.contieneNodo(_rolesSala[RolSala::Inicio]) && CC.contieneNodo(_rolesSala[RolSala::Llave])){
			distanciaTotal += CC.bfs(_rolesSala[RolSala::Inicio], _rolesSala[RolSala::Llave]);
		}
		if (CC.contieneNodo(_rolesSala[RolSala::Fin]) && CC.contieneNodo(_rolesSala[RolSala::Llave])){
			distanciaTotal += CC.bfs(_rolesSala[RolSala::Fin], _rolesSala[RolSala::Llave]);
		}

		double mediaGrado = 0;
		auto adyacencia = CC.getAdyacencia();
		auto itAdy = adyacencia.begin();
		while (itAdy != adyacencia.cend()){
			mediaGrado += itAdy->second.size();
			itAdy++;
		}
		mediaGrado /= numNodos;

		int enemigosTotales = 0;
		int cofresTotales = 0;
		auto nodos = CC.getNodos();
		auto itNodos = nodos.begin();
		while (itNodos != nodos.cend()){
			enemigosTotales += itNodos->second.getEnemigos();
			cofresTotales += itNodos->second.getCofres();
			++itNodos;
		}

		double dispCofres = 0;
		for (auto it = nodos.begin(); it != nodos.end(); ++it){
			unsigned int cofres = it->second.getCofres();
			double mediaDist = 0;
			if (cofres > 0){
				auto distancias = CC.bfsDist(it->first);
				for (auto it2 = nodos.begin(); it2 != nodos.end(); ++it2){
					unsigned int cofres2 = it2->second.getCofres();
					if (cofres2 > 0){
						mediaDist += distancias[it2->first] * cofres2;
					}
				}
				if (cofresTotales > 1)
					mediaDist /= (cofresTotales - 1);
				dispCofres += (mediaDist * cofres);
			}
		}
		if (cofresTotales > 0)
			dispCofres /= cofresTotales;


		double dispEnemigos = 0;
		for (auto it = nodos.begin(); it != nodos.end(); ++it){
			unsigned int enemigos = it->second.getEnemigos();
			double mediaDist = 0;
			if (enemigos > 0){
				auto distancias = CC.bfsDist(it->first);
				for (auto it2 = nodos.begin(); it2 != nodos.end(); ++it2){
					unsigned int enemigos2 = it2->second.getEnemigos();
					if (enemigos2 > 0){
						mediaDist += distancias[it2->first] * enemigos2;
					}
				}
				if (enemigosTotales > 1)
					mediaDist /= (enemigosTotales - 1);
				dispEnemigos += (mediaDist * enemigos);
			}
		}
		if (enemigosTotales > 0)
			dispEnemigos /= enemigosTotales;
		/*
		double dispCofres = 0;
		for (auto it = nodos.begin(); it != nodos.end(); ++it){
			unsigned int cofres = it->second.getCofres();
			double mediaDist = 0;
			if (cofres > 0){
				for (auto it2 = nodos.begin(); it2 != nodos.end(); ++it2){
					unsigned int cofres2 = it2->second.getCofres();
					if (cofres2 > 0){
						mediaDist += CC.bfs(it->first, it2->first) * cofres2;
					}
				}
				if (cofresTotales > 1)
					mediaDist /= (cofresTotales - 1);
				dispCofres += (mediaDist * cofres);
			}
		}
		if (cofresTotales > 0)
			dispCofres /= cofresTotales;

		double dispEnemigos = 0;
		for (auto it = nodos.begin(); it != nodos.end(); ++it){
			unsigned int enemigos = it->second.getEnemigos();
			double mediaDist = 0;
			if (enemigos > 0){
				for (auto it2 = nodos.begin(); it2 != nodos.end(); ++it2){
					unsigned int enemigos2 = it2->second.getEnemigos();
					if (enemigos2 > 0){
						mediaDist += CC.bfs(it->first, it2->first) * enemigos2;
					}
				}
				if (enemigosTotales > 1)
					mediaDist /= (enemigosTotales - 1);
				dispEnemigos += (mediaDist * enemigos);
			}
		}
		if (enemigosTotales > 0)
			dispEnemigos /= enemigosTotales;
		*/
		double pesos = {};
		double fitness = distanciaTotal*6 - mediaGrado - numNodos + dispCofres + dispEnemigos;
		std::cout << "Fitness: " << fitness << " Dist: " << distanciaTotal << " Media Grado: " << mediaGrado << " NumNodos: " << numNodos << " Disp Cofres: " << dispCofres << " Disp Enem: " << dispEnemigos << std::endl;
		this->_adaptacion = fitness;
		return fitness;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
		//double _pesos[] = { 0.375, 0.35, 0.05, 0.05, 0.05, 0.07, 0.055 }; 
		//double _pesos[] = { 0.45, 0.2, 0.03, 0.02, 0, 0.195, 0.105 };
		// Notese que el peso de los ciclos es 0. En ninguna de las ejecuciones he visto que alguno de los
		// individuos sume algo de "nota" con los ciclos. Es decir, que se está limitando el fitness máximo
		// que se puede lograr. Supongo que, en parte, esto está provocado por la forma de evaluar si una CC
		// tiene ciclos. Mientras no se encuentre una forma más adecuada de hacerlo, sugiero dejarlo a 0, dado
		// que tras algunas pruebas, los individuos resultantes son bastante buenos. Creo que haber bajado el
		// parametro "Grado optimo" de 2.5 a 2 ayuda a reducir los ciclos en general. -- Alvaro.
		double suma_pesos = 0;
		double evaluacion = 0;

		for (size_t i = 0; i < 7; ++i){
			suma_pesos += _pesos[i];
		}
		double error = 0.00001;
		if (suma_pesos > 1+error || suma_pesos < 1 - error){
			throw std::exception("Balance de _pesos incorrecto");
		}
		int numNodos = CC.size();
		double mediaGrado = 0;
		double mediaAlto = 0;
		double mediaAncho = 0;
		int ciclosActuales = 0;
		int enemigosActuales = 0;
		int cofresActuales = 0;
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
		//ciclosActuales = CC.tieneCiclos();

		// 1 - abs(x - ideal) / ideal;
		//[NumNodos, Media - Grado - CC, media - tama�o - sala, penalizar - ciclos]
		_valores[0] = 1 - (abs(int(numNodos - param.nodosOptimos)) / (float)param.nodosOptimos);
		if (_valores[0] < 0)	_valores[0] = 0;

		_valores[1] = 1 - (abs(mediaGrado - param.gradoOptimo) / param.gradoOptimo);
		if (_valores[1] < 0)	_valores[1] = 0;

		_valores[2] = 1 - (abs(mediaAlto - param.altoOptimo) / (float)param.altoOptimo);
		if (_valores[2] < 0)	_valores[2] = 0;

		_valores[3] = 1 - (abs(mediaAncho - param.anchoOptimo) / (float)param.anchoOptimo);
		if (_valores[3] < 0)	_valores[3] = 0;

		/*
		_valores[4] = 1 - (abs(int(ciclosActuales - param.ciclosOptimos)) / (float)param.ciclosOptimos);
		if (_valores[4] < 0)	_valores[4] = 0;
		//
		_valores[4] = 0;

		_valores[5] = 1 - (abs(int(enemigosActuales - param.enemigosOptimos)) / (float)param.enemigosOptimos);
		if (_valores[5] < 0)	_valores[5] = 0;

		_valores[6] = 1 - (abs(int(cofresActuales - param.cofresOptimos)) / (float)param.cofresOptimos);
		if (_valores[6] < 0)	_valores[6] = 0;

		for (int i = 0; i < 7; ++i){
			evaluacion += _valores[i] * _pesos[i];
		}

		return evaluacion;*/
	}

	Grafo<Gen> _grafo;
	std::vector<unsigned int> _rolesSala;

	double _punt;
	double _puntAcum;
	double _adaptacion;
	unsigned int _indexMejorCC;
	double _valores[7];
	double _mejoresValores[7];
	
	double _pesos[7];
};

#endif