#ifndef VISTATERMINAL_HPP
#define VISTATERMINAL_HPP

#include <iostream>
#include "controlador.hpp"
#include "iagobserver.hpp"

class VistaTerminal : public IAGObserver {
public:
	VistaTerminal(Controlador& cont){
		c = &cont;
		gen = 0;
		c->addObserver(*(this));
	}

	void ejecuta(){
		c->run();
	}

	void onGeneracionTerminada(double mejor, double mejorGen, double media){
		std::cout << "Generacion " << gen << ": " << mejor << ", " << media << std::endl;
		gen++;
	}

	void onAGTerminado(Cromosoma mejor, double tEjec, double tmSel, double tmCruce, double tmMut, double tInit, double adapMejor){
		gen = 0;
		std::cout << " -- AG finalizado -- " << std::endl;
		std::cout << "Fitness mejor: " << adapMejor << std::endl;
	}

private:
	unsigned int gen = 0;
	Controlador* c;
};

#endif 