#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>

template<class N>
class Grafo{
public:
	Grafo<N>() 
	{
		
	}

	/**
		\brief Anade un nodo al grafo
		\param nodo  Nuevo nodo a anadir
	*/
	void anadeNodo(N nodo){
		_nodos.push_back(nodo);
		_ady.push_back(std::vector<unsigned int>());
	}

	/**
		\brief Anade una arista al grafo
		\param v  Indice del nodo v
		\param w  Indice del nodo w

		\return false si no existia alguno de los nodos, true en otro caso
	*/
	bool anadeArista(unsigned int v, unsigned int w){
		if (v >= _nodos.size() || w >= _nodos.size())
			return false;
		_ady.at(v).push_back(w);
		_ady.at(w).push_back(v);
		return true;
	}

	/**
		\brief Borra un nodo del grafo y sus aristas
		\param v  Indice del nodo a borrar

		\return false si no existia el nodo v, true en otro caso
	*/
	bool borraNodo(unsigned int v){
		if (v >= _nodos.size())
			return false;
		_nodos.erase(_nodos.begin()+v);
		_ady.erase(_ady.begin() + v);
		for (std::size_t i = 0; i < _ady.size(); ++i){
			for (std::size_t j = 0; j < _ady.at(i).size(); ++j){
				if (_ady[i][j] == v){
					_ady.at(i).erase(_ady.at(i).begin() + j);
					j--;
				}
			}
		}
		return true;
	}


private:
	std::vector<std::vector<unsigned int>> _ady;
	std::vector<N> _nodos;
};

#endif