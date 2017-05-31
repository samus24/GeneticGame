#ifndef CROMOSOMA_HPP
#define CROMOSOMA_HPP

#include "Grafo.hpp"
#include "Gen.hpp"
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
		std::vector<std::string> tags = {
			"Distancia I-L-F",
			"Media Grado",
			"Numero Nodos",
			"Disp. Cofres",
			"Disp. Enemigos",
			"Ratio Ancho/Alto",
		};

		_tagValores = tags;
		_valores.resize(tags.size());
	}

	Cromosoma(unsigned int minNodos, unsigned int maxNodos, double densidad) :
		_grafo(minNodos, maxNodos, densidad),
		_rolesSala(RolSala::RolCount)
	{
		_rolesSala[RolSala::Inicio] = myRandom::getRandom(0, int(_grafo.size() - 1));
		do{
			_rolesSala[RolSala::Fin] = myRandom::getRandom(0, int(_grafo.size() - 1));
		} while (_rolesSala[RolSala::Inicio] == _rolesSala[RolSala::Fin]);

		do{
			_rolesSala[RolSala::Llave] = myRandom::getRandom(0, int(_grafo.size() - 1));
		} while (_rolesSala[RolSala::Llave] == _rolesSala[RolSala::Inicio] || _rolesSala[RolSala::Llave] == _rolesSala[RolSala::Fin]);

		std::vector<std::string> tags = {
			"Distancia I-L-F",
			"Media Grado",
			"Numero Nodos",
			"Disp. Cofres",
			"Disp. Enemigos",
			"Ratio Ancho/Alto",
		};

		_tagValores = tags;
		_valores.resize(tags.size());
	}

	Grafo<Gen> getGenotipo() const{
		return _grafo;
	}

	Grafo<Gen> getMejorCC() const{
		auto ccs = _grafo.getComponentesConexas();
		auto tam = ccs.size();
		if (_indexMejorCC >= tam)
			return ccs[0];
		return ccs[_indexMejorCC];
	}

	unsigned int getIndexElMejor() const{
		return _indexMejorCC;
	}

	double getPunt() const{
		return _punt;
	}

	double getPuntAcum() const{
		return _puntAcum;
	}

	double getAdaptacion() const{
		return _adaptacion;
	}

	std::vector<double> getValores() const{
		return _mejoresValores;
	}

	std::vector<std::string> getTagValores() const{
		return _tagValores;
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

	void setGenotipo(Grafo<Gen> g){
		_grafo = g;
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
		double mejorCC = -999999999999999;
		_indexMejorCC = -1;
		double componente;
		std::vector<Grafo<Gen>::ComponenteConexa<Gen>> CC = _grafo.getComponentesConexas();
		int tam = CC.size();
		for (std::size_t i = 0; i < CC.size(); ++i) {
			componente = evaluaCC(CC[i]);
			if (componente > mejorCC){
				mejorCC = componente;
				_indexMejorCC = i;
				_mejoresValores = _valores;
			}
		}
		this->_adaptacion = mejorCC;
		return _adaptacion;
	}

	void bloating(unsigned int maxNodos){
		if (_grafo.size() > maxNodos){
			_grafo = _grafo.divideGrafo(maxNodos).at(0);
		}
	}

	Cromosoma& operator=(Cromosoma other){
		std::swap(_grafo, other._grafo);
		_adaptacion = other.getAdaptacion();
		_punt = other.getPunt();
		_puntAcum = other.getPuntAcum();
		_indexMejorCC = other.getIndexElMejor();
		_mejoresValores = other.getValores();
		_tagValores = other.getTagValores();
		_rolesSala = other.getRolesSala();
		return *this;
	}

private:
	double evaluaCC(Grafo<Gen>::ComponenteConexa<Gen> CC) {
		/*
		La nueva funcion de evaluacion debe:
		- Minimizar el numero de nodos de la CC						:: Hecho
		- Maximizar las distancias entre Inicio-Llave y Llave-Fin	:: Hecho
		- Minimizar la media de grado de las salas					:: Hecho
		- Maximizar la dispersion de enemigos y cofres				:: Hecho
		*/
		unsigned int numNodos = CC.size();
		unsigned int distanciaTotal = 0;

		// Si no contiene las 3, es posible que a la mazmorra final le falte una sala clave
		if (CC.contieneNodo(_rolesSala[RolSala::Inicio]) && CC.contieneNodo(_rolesSala[RolSala::Llave]) && CC.contieneNodo(_rolesSala[RolSala::Fin])){
			distanciaTotal += CC.bfs(_rolesSala[RolSala::Inicio], _rolesSala[RolSala::Llave]);
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

		double mediaAncho = 0;
		double mediaAlto = 0;
		int enemigosTotales = 0;
		int cofresTotales = 0;
		auto nodos = CC.getNodos();
		auto itNodos = nodos.begin();
		while (itNodos != nodos.cend()){
			enemigosTotales += itNodos->second.getEnemigos();
			cofresTotales += itNodos->second.getCofres();
			mediaAncho += itNodos->second.getAncho();
			mediaAlto += itNodos->second.getAlto();
			++itNodos;
		}
		mediaAncho /= numNodos;
		mediaAlto /= numNodos;

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


		double ratioAnchoAlto = mediaAncho / mediaAlto;

		double fitness = distanciaTotal * 1 - mediaGrado - numNodos*0.4 + dispCofres + dispEnemigos + (ratioAnchoAlto - 1);

		fitness += 20;	// Para evitar fitness negativos que afectan a evaluaPob
		_valores[0] = distanciaTotal;
		_valores[1] = mediaGrado;
		_valores[2] = numNodos;
		_valores[3] = dispCofres;
		_valores[4] = dispEnemigos;
		_valores[5] = ratioAnchoAlto;
		this->_adaptacion = fitness;
		return fitness;
	}

	Grafo<Gen> _grafo;
	std::vector<unsigned int> _rolesSala;

	double _punt;
	double _puntAcum;
	double _adaptacion;
	unsigned int _indexMejorCC;
	std::vector<double> _valores;
	std::vector<double> _mejoresValores;
	std::vector<std::string> _tagValores;

};

#endif