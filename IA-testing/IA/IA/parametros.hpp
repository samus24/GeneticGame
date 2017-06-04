#ifndef PARAMETROS_HPP
#define PARAMETROS_HPP

#include "Nodo.hpp"
#include "metodoCruce.hpp"
#include "metodoMutacion.hpp"
#include "metodoSeleccion.hpp"

class Parametros{
public:
<<<<<<< HEAD:AG/AG/Parametros.hpp
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
=======
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
	metodoCruce* cruce;
	metodoMutacion* mutacion;
	metodoSeleccion* seleccion;

	bool paralelizar;
	std::set<Operacion> opValidas;
>>>>>>> refs/remotes/origin/IA-Testing:IA-testing/IA/IA/parametros.hpp

	Parametros(){

	}

	Parametros(unsigned int tam, unsigned int iter,
		unsigned int minNodos, unsigned int maxNodos, double densidad,
		bool elitismo, bool bloating, bool contractividad,
		double probCruce, double probMutacion, int seed,
		metodoCruce* cruce, metodoMutacion* mutacion, metodoSeleccion* seleccion,
		bool paralelizar){
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
		this->paralelizar = paralelizar;
	}

	void addOp(Operacion o){
		opValidas.insert(o);
	}

	bool delOp(Operacion o){
		if (opValidas.find(o) != opValidas.end()){
			opValidas.erase(o);
			return true;
		}
		return false;
	}


};



#endif