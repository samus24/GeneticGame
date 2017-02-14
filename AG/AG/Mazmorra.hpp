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

private:
	std::vector<Sala> _salas;
};

#endif
