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

	void onAGTerminado(Cromosoma mejor, double tEjec, double tmSel, double tmCruce, double tmMut, double tInit, double eval, poblacion pob){
		gen = 0;
		std::cout << " -- AG finalizado -- " << std::endl;
		std::cout << "--Arbol patrulla--" << std::endl;
		std::cout << mejor.getGenotipo(0).toString() << std::endl;
		std::cout << "--Arbol ataque--" << std::endl;
		std::cout << mejor.getGenotipo(1).toString() << std::endl;
		std::cout << "Fitness mejor: " << mejor.getAdaptacion() << std::endl;
		std::cout << "casillasExploradas, golpes, heridasBloqueadas, daño";
		std::cout << "Valores mejor: ";
		for (size_t i = 0; i < 4; ++i){
			std::cout << mejor.getValores()[i];
			if (i != 3) std::cout << ", ";
		}
		std::cout << std::endl;
		std::cout << "Ponderacion mejor: ";
		for (size_t i = 0; i < 4; ++i){
			std::cout << mejor.getValores()[i] * mejor.getPesos()[i] << "(x" << mejor.getPesos()[i] << ")";
			if (i != 3) std::cout << ", ";
		}
		std::cout << std::endl;
	}

private:
	unsigned int gen = 0;
	Controlador* c;
};

#endif 