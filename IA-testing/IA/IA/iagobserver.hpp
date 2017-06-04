#ifndef IAGOBSERVER_HPP
#define IAGOBSERVER_HPP

#include "Cromosoma.hpp"
#include "poblacion.hpp"

class IAGObserver{
public:
	virtual ~IAGObserver() {};

	virtual void onGeneracionTerminada(double, double, double) = 0;	// mejor, mejorGen, media

	virtual void onAGTerminado(Cromosoma, double, double, double, double, double, double, poblacion) = 0;
	//cromosoma mejor,tiempo total ej,  tiempo medio seleccion, cruce, mutacion, inicializacion, evaluacion, poblacion
};

#endif