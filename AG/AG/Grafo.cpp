#include "Grafo.hpp"

template<class N>
std::unordered_map< unsigned int, std::set<unsigned int> > Grafo<N>::getAdyacencia(){
	return _ady;
}

template<class N>
std::vector< Pair<unsigned int, N> > Grafo<N>::getNodos(){
	return _nodos;
}

template<class N>
unsigned int Grafo<N>::size(){
	return _nodos.size();
}

/**
\brief Anade un nodo al grafo
\param nodo  Nuevo nodo a anadir
@throws Invalid argument si el id del nodo ya esta en el grafo
*/
template<class N>
void Grafo<N>::anadeNodo(N nodo, int id = -1){
	if (id < 0){
		if (_ady.find(_nodos.size()) == _ady.end()){
			_ady.emplace(_nodos.size(), std::set<unsigned int>());
			_nodos.push_back(Pair<unsigned int, N>(_nodos.size(), nodo));

		}
		else{
			throw std::invalid_argument("El nodo con id " + std::to_string(_nodos.size()) + "ya existe en el grafo");
		}
	}
	else{
		if (_ady.find(id) == _ady.end()){
			_ady.emplace(id, std::set<unsigned int>());
			_nodos.push_back(Pair<unsigned int, N>(id, nodo));
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
template<class N>
bool Grafo<N>::anadeArista(unsigned int v, unsigned int w){
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
template<class N>
bool Grafo<N>::borraArista(unsigned int v, unsigned int w){
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
template<class N>
unsigned int Grafo<N>::getGradoNodo(unsigned int v){
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
template<class N>
std::vector<Grafo<N>> Grafo<N>::divideGrafo(unsigned int v){
	if (v >= _nodos.size())
		throw std::invalid_argument("Punto de corte invalido");
	std::vector<Grafo<N>> subGrafos(2);
	for (std::size_t i = 0; i < _nodos.size(); ++i){
		if (i <= v){
			// Para el 1er subgrafo
			subGrafos.at(0).anadeNodo(_nodos[i].second);
		}
		else{
			// Para el 2o subgrafo
			subGrafos.at(1).anadeNodo(_nodos[i].second);
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
template<class N>
Grafo<N> Grafo<N>::unirGrafo(Grafo<N> a, Grafo<N> b){
	Grafo<N> ret;
	std::vector<N> nodosA = a.getNodos();
	std::vector<N> nodosB = b.getNodos();
	unsigned int sizeA = nodosA.size();
	for (std::size_t i = 0; i < sizeA; ++i){
		ret.anadeNodo(nodosA[i]);
	}
	for (std::size_t i = 0; i < nodosB.size(); ++i){
		ret.anadeNodo(nodosB[i]);
	}

	std::vector<std::set<unsigned int>> adyA = a.getAdyacencia();
	std::vector<std::set<unsigned int>> adyB = b.getAdyacencia();
	for (std::size_t i = 0; i < sizeA; ++i){
		auto it = adyA[i].begin();
		unsigned int elem;
		for (std::size_t j = 0; j < adyA[i].size(); ++j){
			elem = *(it);
			ret.anadeArista(i, elem);
			it++;
		}
	}
	for (std::size_t i = 0; i < adyB.size(); ++i){
		auto it = adyB[i].begin();
		unsigned int elem;
		for (std::size_t j = 0; j < adyB[i].size(); ++j){
			elem = *(it);
			ret.anadeArista(i + sizeA, elem + sizeA);
			// Cuando se añaden las aristas del subgrafo B, todos los nodos de B en el grafo de retorno estan desplazados sizeA posiciones a la derecha
			// por tanto, es necesario referirse a estos nodos sumandoles el tamaño del subgrafo A
			it++;
		}
	}

	unsigned int nuevas = getRandom(1, (int)(sizeA * nodosB.size() * 0.75));	// Para evitar demasiada densidad, se limita al 75% de las posibles nuevas aristas
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
template<class N>
Grafo<N> Grafo<N>::unirGrafos(std::vector<Grafo<N>> subs){
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

template<class N>
std::vector<Grafo<N>> Grafo<N>::divideEnGrafos(unsigned int n){
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

template<class N>
Grafo<N> Grafo<N>::getCopia(){
	Grafo<N> ret;
	ret._nodos = this->getNodos();
	ret._ady = this->getAdyacencia();
	return ret;
}

template<class N>
std::vector< Grafo<N> > Grafo<N>::getComponentesConexas(){

	return std::vector< Grafo<N> >();
}