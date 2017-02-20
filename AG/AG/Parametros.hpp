#ifndef PARAMETROS_HPP
#define PARAMETROS_HPP

#include "MetodoCruce.hpp"
#include "MetodoMutacion.hpp"
#include "MetodoSeleccion.hpp"

class Parametros{
public:
	unsigned int tamPob;
	unsigned int iteraciones;
	unsigned int minNodos;
	unsigned int maxNodos;
	double densidad;
	bool elitismo;
	bool bloating;
	bool contractividad;
	double probCruce;
	double probMutacion;
	int seed;
	MetodoCruce* cruce;
	//MetodoMutacion mutacion;
	MetodoSeleccion* seleccion;

	Parametros(){

	}

	Parametros(unsigned int tam, unsigned int iter,
				unsigned int minNodos, unsigned int maxNodos, double densidad,
				bool elitismo, bool bloating, bool contractividad,
				double probCruce, double probMutacion, int seed,
				MetodoCruce* cruce, MetodoMutacion mutacion, MetodoSeleccion* seleccion){
		this->tamPob = tam;
		this->iteraciones = iter;
		this->minNodos = minNodos;
		this->maxNodos = maxNodos;
		this->densidad = densidad;
		this->elitismo = elitismo;
		this->bloating = bloating;
		this->contractividad = contractividad;
		this->probCruce = probCruce;
		this->probMutacion = probMutacion;
		this->seed = seed;
		/*
		this->cruce = cruce;
		this->mutacion = mutacion;*/
		this->seleccion = seleccion;
	}
};



#endif