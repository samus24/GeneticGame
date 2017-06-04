#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP

#include "AG.hpp"
#include "IAGObserver.hpp"
<<<<<<< HEAD:AG/AG/Controlador.hpp
=======
#include "ICromosomaObserver.hpp"
>>>>>>> refs/remotes/origin/IA-Testing:IA-testing/IA/IA/controlador.hpp

class Controlador {
public:
	Controlador(AG& ag)
	{
		_ag = &ag;
	}

	Cromosoma run(){
		return _ag->ejecuta();
	}

<<<<<<< HEAD:AG/AG/Controlador.hpp
	void updateParam(Parametros p, ParametrosEval pEval){
		_ag->updateParam(p, pEval);
	}

=======
>>>>>>> refs/remotes/origin/IA-Testing:IA-testing/IA/IA/controlador.hpp
	void addObserver(IAGObserver& o){
		_ag->addObserver(o);
	}

<<<<<<< HEAD:AG/AG/Controlador.hpp
	void addObserverCruce(IObserverCruce& o){
		_ag->addObserverCruce(o);
	}
=======
	void addCromosomaObserver(ICromosomaObserver& o){
		_ag->addCromosomaObserver(o);
	}

	std::vector<Mapa> getMapas(){
		return _ag->getMapas();
	}

	void setParametros(Parametros p){
		_ag->setParametros(p);
	}

>>>>>>> refs/remotes/origin/IA-Testing:IA-testing/IA/IA/controlador.hpp
private:
	AG* _ag;

};

#endif