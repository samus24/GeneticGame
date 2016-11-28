#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <algorithm>
#include <random>
#include <vector>
#include <unordered_map>
#include <set>
#include <iterator>
#include <random>
#include <time.h>

#include "Pair.hpp"

template<class N>
class Grafo{
public:
	Grafo<N>() 
	{
		std::srand(time(NULL));
	}

	Grafo<N>(unsigned int minNodos, unsigned int maxNodos, double densidad)
	{
		std::srand(time(NULL));
		unsigned int nNodos = getRandom(minNodos, maxNodos);
		for (std::size_t i = 0; i < nNodos; ++i){
			N nodo;				
			// El contructor por defecto de Gen inicializa ancho y alto random
			this->anadeNodo(nodo);		
		}
		unsigned int nAristas = sumatorio(nNodos) * densidad;
		unsigned int nodoA, nodoB;
		while (nAristas > 0){
			nodoA = getRandom(0, nNodos - 1);
			nodoB = getRandom(0, nNodos - 1);
			if ((nodoA != nodoB) && !hayAristaEntre(nodoA, nodoB)){
				this->anadeArista(nodoA, nodoB);
				--nAristas;
			}
		}

	}

	std::unordered_map< unsigned int, std::set<unsigned int> > getAdyacencia();

	std::vector< Pair<unsigned int, N> > getNodos();

	unsigned int size();

	/**
		\brief Anade un nodo al grafo
		\param nodo  Nuevo nodo a anadir
		@throws Invalid argument si el id del nodo ya esta en el grafo
	*/
	void anadeNodo(N nodo, int id = -1);

	/**
		\brief Anade una arista al grafo
		\param v  Indice del nodo v
		\param w  Indice del nodo w

		\return false si no existe alguno de los nodos, true en otro caso
	*/
	bool anadeArista(unsigned int v, unsigned int w);

	/**
	\brief Borra una arista del grafo
	\param v  Indice del nodo v
	\param w  Indice del nodo w

	\return false si no existe alguno de los nodos v o w, true en otro caso
	*/
	bool borraArista(unsigned int v, unsigned int w);

	/**
	\brief Devuelve el grado del nodo
	\param v  Indice del nodo v
	@throws Invalid argument si el nodo no existe
	\return el grado del nodo
	*/
	unsigned int getGradoNodo(unsigned int v);

	/**
	\brief Divide el grafo en 2 subgrafos
	\param v  Indice del nodo usado como punto de corte
	@throws Invalid argument si el nodo no existe
	\return Vector con los 2 subgrafos
	*/
	std::vector<Grafo<N>> divideGrafo(unsigned int v);

	/**
	\brief Une 2 subgrafos en 1 grafo
	\param a subgrafo a
	\param b subgrafo b
	\return Grafo unido
	*/
	Grafo<N> unirGrafo(Grafo<N> a, Grafo<N> b);

	/**
	\brief Une varios subgrafos en 1 grafo
	\param subs vector de subgrafos a unir
	@throws Invalid Argument si el vector no tiene al menos 2 grafos
	\return Grafo unido
	*/
	Grafo<N> unirGrafos(std::vector<Grafo<N>> subs);

	/**
	\brief Divide un grafo en n subgrafos
	\param n numero de subgrafos a realizar
	@throws Invalid Argument si n es mayor que el nº de nodos
	\return Vector con los subgrafos
	*/

	std::vector<Grafo<N>> divideEnGrafos(unsigned int n);

	Grafo<N> getCopia();
	
	std::vector< Grafo<N> > getComponentesConexas();


	/*
		IMPORTANTE!!!
		Los siguientes métodos suponen que el grafo es totalmente conexo (es decir, una sola componente conexa)
		Se debe utilizar SÓLO si se asegura que el grafo esta formado por una unica componente conexa.

		PS: Esta chapuza viene derivada del hecho de no poder crear una clase ComponenteConexa que herede de grafo
		porque el compilador da un problema que no he sido capaz de resolver 
		-- Álvaro
	*/



private:

	int getRandom(int from, int to){
		float random = (float)(rand() / (float)RAND_MAX);
		return from + random * (to - from);
	}

	int sumatorio(int n)
	{
		return (n == 0) ? 0 : sumatorio(n - 1) + n;
	}

	bool hayAristaEntre(unsigned int a, unsigned int b){
		if (_ady[a].find(b) != _ady[a].end()){
			// Hay arista entre a y b (con esto bastaría, pero por consistencia, se comprueba en la otra direccion
			if (_ady[b].find(a) != _ady[b].end()){
				// La arista también la contiene b
				return true;
			}
			else{
				throw std::runtime_error("Inconsistencia en las aristas (entre " + std::to_string(a) + " y " + std::to_string(b) + ")");
			}
		}
		else if (_ady[b].find(a) != _ady[b].end()){
			// En este caso, a no contempla esa arista, pero b sí
			throw std::runtime_error("Inconsistencia en las aristas (entre " + std::to_string(a) + " y " + std::to_string(b) + ")");
		}
		return false;
	}

	std::unordered_map< unsigned int, std::set<unsigned int> > _ady;
	std::vector< Pair<unsigned int, N> > _nodos;
};

#endif