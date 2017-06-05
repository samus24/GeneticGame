#include "ParallelTask.hpp"
#include "AG.hpp"


ParallelTask::ParallelTask()
	: mThread(&ParallelTask::runTask, this)
	, mFinished(false)
{
}

void ParallelTask::execute()
{
	mFinished = false;
	mElapsedTime.restart();
	mThread.launch();
}

bool ParallelTask::isFinished()
{
	sf::Lock lock(mMutex);
	return mFinished;
}

float ParallelTask::getCompletion()
{
	sf::Lock lock(mMutex);

	return _generaciones / float(_p.iteraciones);
}

void ParallelTask::runTask()
{
	_generaciones = 0;
	_p.tamPob = 30;							// Tamano de la poblacion
	_p.iteraciones = 30;					// Numero maximo de generaciones
	_p.minNodos = 10;						// Numero minimo de nodos iniciales
	_p.maxNodos = 40;						// Numero maximo de nodos iniciales
	_p.densidad = 0.03;						// Densidad de aristas inciales
	_p.elitismo = false;					// Elitismo (guarda a los mejores)
	_p.bloating = true;						// Bloating	(evita grafos demasiado grandes)
	_p.contractividad = false;				// Contractividad (descarta generaciones malas)
	_p.probCruce = 0.6;						// Probabilidad de cruce
	_p.probMutacion = 0.02;					// Probabilidad de mutacion
	_p.seleccion = new SeleccionTorneo();	// Metodo de seleccion (Ver "MetodoSeleccion.hpp")
	_p.cruce = new CruceMonopunto();		// Metodo de cruce (Ver "MetodoCruce.hpp")
	_p.mutacion = new MutacionCombinada();	// Metodo de mutacion (Ver "MetodoMutacion.hpp")
	AG ag(_p);
	ag.addObserver(*this);
	_mejor = ag.ejecuta();

	{ // mFinished may be accessed from multiple threads, protect it
		sf::Lock lock(mMutex);
		mFinished = true;
	}
}

void ParallelTask::onGeneracionTerminada(Cromosoma){
	_generaciones++;
}

void ParallelTask::onAGTerminado(Cromosoma){

}

Cromosoma ParallelTask::getMejor() const{
	return _mejor;
}