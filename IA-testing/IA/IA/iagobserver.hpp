#ifndef IAGOBSERVER_HPP
#define IAGOBSERVER_HPP

#include "Cromosoma.hpp"
#include "poblacion.hpp"

class IAGObserver{
public:
	virtual ~IAGObserver() {};

	virtual void onGeneracionTerminada(Cromosoma, double, double, double, double) = 0;	// mejor, mejorGen, media, mediaSeleccion

<<<<<<< HEAD:IA-testing/IA/IA/iagobserver.hpp
	virtual void onAGTerminado(Cromosoma, double, double, double, double, double, double, poblacion) = 0;
	//cromosoma mejor,tiempo total ej,  tiempo medio seleccion, cruce, mutacion, inicializacion, evaluacion, poblacion
=======
	virtual void onAGTerminado(Cromosoma, double, double, double, double, double, double) = 0;
	//cromosoma mejor,tiempo total ej,  tiempo medio seleccion, cruce, mutacion, inicializacion, evaluacion
>>>>>>> refs/remotes/origin/master:AG/AG/IAGObserver.hpp
};

#endif