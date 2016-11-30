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
	template<class N>
	class ComponenteConexa : public Grafo<N>{
	public:
		ComponenteConexa<N>(){

		}
	};

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

	std::unordered_map< unsigned int, std::set<unsigned int> > getAdyacencia(){
		return _ady;
	}

	std::unordered_map< unsigned int, N > getNodos(){
		return _nodos;
	}

	unsigned int size(){
		return _nodos.size();
	}

	/**
		\brief Anade un nodo al grafo
		\param nodo  Nuevo nodo a anadir
		@throws Invalid argument si el id del nodo ya esta en el grafo
	*/
	void anadeNodo(N nodo, int id = -1){
		if (id < 0){
			if (_ady.find(_nodos.size()) == _ady.end()){
				_ady.emplace(_nodos.size(), std::set<unsigned int>());
				_nodos.emplace(_nodos.size(), nodo);
			}
			else{
				throw std::invalid_argument("El nodo con id " + std::to_string(_nodos.size()) + "ya existe en el grafo");
			}
		}
		else{
			if (_ady.find(id) == _ady.end()){
				_ady.emplace(id, std::set<unsigned int>());
				_nodos.emplace(id, nodo);
			}
			else{
				throw std::invalid_argument("El nodo con id " + std::to_string(id) + "ya existe en el grafo");
			}
		}
	}

	/**
		\brief Anade una arista al grafo
		\param v  Indice del nodo v
		\param w  Indice del nodo w

		\return false si no existe alguno de los nodos, true en otro caso
	*/
	bool anadeArista(unsigned int v, unsigned int w){
		if (_ady.find(v) == _ady.end() || _ady.find(w) == _ady.end()){
			return false;
		}
		_ady.at(v).emplace(w);
		_ady.at(w).emplace(v);
		return true;
	}

	/**
	\brief Borra una arista del grafo
	\param v  Indice del nodo v
	\param w  Indice del nodo w

	\return false si no existe alguno de los nodos v o w, true en otro caso
	*/
	bool borraArista(unsigned int v, unsigned int w){
		if (_ady.find(v) == _ady.end() || _ady.find(w) == _ady.end()){
			return false;
		}

		auto it = _ady.at(v).find(w);
		if (it != _ady.at(v).end())
			_ady.at(v).erase(it);

		it = _ady.at(w).find(v);
		if (it != _ady.at(w).end())
			_ady.at(w).erase(it);

		return true;
	}

	/**
	\brief Devuelve el grado del nodo
	\param v  Indice del nodo v
	@throws Invalid argument si el nodo no existe
	\return el grado del nodo
	*/
	unsigned int getGradoNodo(unsigned int v){
		if (_ady.find(v) == _ady.end())
			throw std::invalid_argument("Nodo inexistente");
		return _ady[v].size();
	}

	/**
	\brief Divide el grafo en 2 subgrafos
	\param v  Indice del nodo usado como punto de corte
	@throws Invalid argument si el nodo no existe
	\return Vector con los 2 subgrafos
	*/
	std::vector<Grafo<N>> divideGrafo(unsigned int v){
		if (v >= _nodos.size())
			throw std::invalid_argument("Punto de corte invalido");
		std::vector<Grafo<N>> subGrafos(2);
		for (std::size_t i = 0; i < _nodos.size(); ++i){
			if (i <= v){
				// Para el 1er subgrafo
				subGrafos.at(0).anadeNodo(_nodos[i]);
			}
			else{
				// Para el 2o subgrafo
				subGrafos.at(1).anadeNodo(_nodos[i]);
			}
		}

		for (std::size_t i = 0; i < _ady.size(); ++i){
			auto it = _ady[i].begin();
			unsigned int elem;
			for (std::size_t j = 0; j < _ady[i].size(); ++j){
				elem = *(it);
				if (i <= v){
					if (elem <= v){
						subGrafos.at(0).anadeArista(i, elem);
					}
				}
				else{
					if (elem > v){
						subGrafos.at(1).anadeArista(i - (v + 1), elem - (v + 1));
						// Debemos restar v+1 a los indices en el grafo original, porque para el segundo subgrafo, 
						// estos nodos vuelven a empezar desde 0 hasta su tamaño.
					}
				}
				it++;
			}
		}

		return subGrafos;
	}

	/**
	\brief Une 2 subgrafos en 1 grafo
	\param a subgrafo a
	\param b subgrafo b
	\return Grafo unido
	*/
	static Grafo<N> unirGrafo(Grafo<N> a, Grafo<N> b){
		Grafo<N> ret;
		std::unordered_map< unsigned int, N > nodosA = a.getNodos();
		std::unordered_map< unsigned int, N > nodosB = b.getNodos();
		unsigned int sizeA = nodosA.size();
		auto itA = nodosA.begin();
		while (itA != nodosA.cend()){
			ret.anadeNodo(itA->second);
			itA++;
		}
		auto itB = nodosB.begin();
		while (itB != nodosB.cend()){
			ret.anadeNodo(itB->second);
			itB++;
		}

		std::unordered_map< unsigned int, std::set<unsigned int> > adyA = a.getAdyacencia();
		std::unordered_map< unsigned int, std::set<unsigned int> > adyB = b.getAdyacencia();

		itA = nodosA.begin();
		while (itA != nodosA.cend()){
			auto it = adyA[itA->first].begin();
			unsigned int elem;
			while (it != adyA[itA->first].cend()){
				elem = *(it);
				ret.anadeArista(itA->first, elem);
				it++;
			}
			itA++;
		}
		itB = nodosB.begin();
		while (itB != nodosB.cend()){
			auto it = adyB[itB->first].begin();
			unsigned int elem;
			while (it != adyB[itB->first].cend()){
				elem = *(it);
				ret.anadeArista(itB->first + sizeA, elem + sizeA);
				// Cuando se añaden las aristas del subgrafo B, todos los nodos de B en el grafo de retorno estan desplazados sizeA posiciones a la derecha
				// por tanto, es necesario referirse a estos nodos sumandoles el tamaño del subgrafo A
				it++;
			}
			itB++;
		}

		unsigned int nuevas = Grafo<N>::getRandom(1, (sizeA * nodosB.size() * 0.75));	// Para evitar demasiada densidad, se limita al 75% de las posibles nuevas aristas
		std::vector<Pair<unsigned int, unsigned int>> uniones;
		for (std::size_t i = 0; i < sizeA; ++i){
			for (std::size_t j = 0; j < nodosB.size(); ++j){
				uniones.push_back(Pair<unsigned int, unsigned int>(i, j + sizeA));
			}
		}

		std::random_shuffle(uniones.begin(), uniones.end());
		for (std::size_t i = 0; i < nuevas; ++i){
			ret.anadeArista(uniones[i].first, uniones[i].second);
		}

		return ret;
	}

	/**
	\brief Une varios subgrafos en 1 grafo
	\param subs vector de subgrafos a unir
	@throws Invalid Argument si el vector no tiene al menos 2 grafos
	\return Grafo unido
	*/
	static Grafo<N> unirGrafos(std::vector<Grafo<N>> subs){
		if (subs.size() < 2)
			throw std::invalid_argument("Subgrafos insuficientes");
		Grafo<N> ret = unirGrafo(subs[0], subs[1]);
		for (std::size_t i = 2; i < subs.size(); ++i){
			ret = unirGrafo(ret, subs[i]);
		}
		return ret;
	}

	/**
	\brief Divide un grafo en n subgrafos
	\param n numero de subgrafos a realizar
	@throws Invalid Argument si n es mayor que el nº de nodos
	\return Vector con los subgrafos
	*/

	std::vector<Grafo<N>> divideEnGrafos(unsigned int n){
		std::vector<Grafo<N>> ret, aux;
		aux = divideGrafo(getRandom(0, _nodos.size()));
		for (std::size_t i = 1; i < n; ++i){
			if (i == n - 1){
				ret.push_back(aux[0]);
				ret.push_back(aux[1]);
			}
			else{
				if (aux[0].size() < aux[1].size()){
					ret.push_back(aux[0]);
					aux = aux.at(1).divideGrafo(getRandom(0, aux[1].size()));
				}
				else{
					ret.push_back(aux[1]);
					aux = aux.at(0).divideGrafo(getRandom(0, aux[0].size()));
				}
			}
		}
		return ret;
	}

	Grafo<N> getCopia(){
		Grafo<N> ret;
		ret._nodos = this->getNodos();
		ret._ady = this->getAdyacencia();
		return ret;
	}
	
	std::vector< ComponenteConexa<N> > getComponentesConexas(){
		std::vector< ComponenteConexa<N> > ret;
		std::set<unsigned int> processed;
		auto it = _ady.begin();
		while (it != _ady.end()){
			if (processed.find(it->first) == processed.end()){
				// actual node hasn't been processed yet
				ComponenteConexa<N> cc;
				calculateConnectedComponentFromNode(it, processed, cc);
				ret.push_back(cc);
			}
			it++;
		}
		return ret;
	}

private:

	void calculateConnectedComponentFromNode(std::unordered_map<unsigned int, std::set<unsigned int>>::iterator it, std::set<unsigned int> &processed, ComponenteConexa<N> &cc){
		processed.emplace(it->first);
		cc.anadeNodo(_nodos[it->first], it->first);
		try{
			auto it2 = _ady[it->first].begin();
			while(it2 != _ady[it->first].end()){
				if (processed.find(*it2) == processed.end()){
					// next node hasn't been processed yet
					calculateConnectedComponentFromNode(_ady.find(*it2), processed, cc);
					cc.anadeArista(it->first, *it2);
				}
				else{
					cc.anadeArista(it->first, *it2);
				}
				it2++;
			}
		}
		catch (std::out_of_range e){
			return;
		}
	}

	static int getRandom(int from, int to){
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(from, to);
		return dis(gen);
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
	std::unordered_map< unsigned int, N > _nodos;
};


#endif

