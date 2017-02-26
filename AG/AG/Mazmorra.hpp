#ifndef MAZMORRA_HPP
#define MAZMORRA_HPP

#include "Sala.hpp"
#include <vector>

class Mazmorra {
public:
	Mazmorra(){

	}

	void anadeSala(Sala s){
		_salas.push_back(s);
	}

	void vaciaSalas(){
		_salas.clear();
	}

	Sala getSala(unsigned int i){
		if (i < _salas.size())
			return _salas[i];
		else
			throw std::exception("Sala inexistente");
	}
	std::vector<Sala> getSalas(){
		return _salas;
	}

	Sala operator[](unsigned int i){
		return getSala(i);
	}

private:
	std::vector<Sala> _salas;
};

#endif
