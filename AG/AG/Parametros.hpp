#ifndef PARAMETROS_HPP
#define PARAMETROS_HPP

#include "MetodoCruce.hpp"
#include "MetodoMutacion.hpp"
#include "MetodoSeleccion.hpp"

class Parametros{
public:
	

	unsigned int tamPob;			// Tamano de la poblacion
	unsigned int iteraciones;		// Numero maximo de generaciones
	unsigned int minNodos;			// Numero minimo de nodos iniciales
	unsigned int maxNodos;			// Numero maximo de nodos iniciales
	double densidad;				// Densidad de aristas inciales
	bool elitismo;					// Elitismo (guarda a los mejores)
	bool bloating;					// Bloating	(evita grafos demasiado grandes)
	bool contractividad;			// Contractividad (descarta generaciones malas)
	double probCruce;				// Probabilidad de cruce
	double probMutacion;			// Probabilidad de mutacion
	int seed;						// Semilla aleatoria (NO USADO)
	MetodoCruce* cruce;				// Metodo de seleccion (Ver "MetodoSeleccion.hpp")
	MetodoMutacion* mutacion;		// Metodo de cruce (Ver "MetodoCruce.hpp")
	MetodoSeleccion* seleccion;		// Metodo de mutacion (Ver "MetodoMutacion.hpp")

	Parametros(){

	}

	Parametros(unsigned int tam, unsigned int iter,
				unsigned int minNodos, unsigned int maxNodos, double densidad,
				bool elitismo, bool bloating, bool contractividad,
				double probCruce, double probMutacion, int seed,
				MetodoCruce* cruce, MetodoMutacion* mutacion, MetodoSeleccion* seleccion){
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
		this->cruce = cruce;
		this->mutacion = mutacion;
		this->seleccion = seleccion;
	}
};



#endif