#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP

#include "AG.hpp"
#include "IAGObserver.hpp"
<<<<<<< HEAD:IA-testing/IA/IA/controlador.hpp
#include "ICromosomaObserver.hpp"
=======
>>>>>>> refs/remotes/origin/master:AG/AG/Controlador.hpp

class Controlador {
public:
	Controlador(AG& ag)
	{
		_ag = &ag;
	}

	Cromosoma run(){
		return _ag->ejecuta();
	}

<<<<<<< HEAD:IA-testing/IA/IA/controlador.hpp
	void addObserver(IAGObserver& o){
		_ag->addObserver(o);
	}

	void addCromosomaObserver(ICromosomaObserver& o){
		_ag->addCromosomaObserver(o);
	}

	std::vector<Mapa> getMapas(){
		return _ag->getMapas();
	}

	void setParametros(Parametros p){
		_ag->setParametros(p);
	}

=======
	void updateParam(Parametros p, ParametrosEval pEval){
		_ag->updateParam(p, pEval);
	}

	void addObserver(IAGObserver& o){
		_ag->addObserver(o);
	}

	void addObserverCruce(IObserverCruce& o){
		_ag->addObserverCruce(o);
	}
>>>>>>> refs/remotes/origin/master:AG/AG/Controlador.hpp
private:
	AG* _ag;

};

#endif