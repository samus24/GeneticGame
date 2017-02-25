#ifndef METODOCRUCE_HPP
#define METODOCRUCE_HPP

#include "RandomGen.hpp"
#include"Cromosoma.hpp"

class MetodoCruce {
public:

	virtual void cruzar(Cromosoma*, Cromosoma*, ParametrosEval p) = 0;

};

class CruceMonopunto : public MetodoCruce{
public:
	void cruzar(Cromosoma* a, Cromosoma* b, ParametrosEval p){
		Grafo<Gen> grafoA = a->getGenotipo();
		Grafo<Gen> grafoB = a->getGenotipo();
		unsigned int adaptacionA = a->getAdaptacion();
		unsigned int adaptacionB = b->getAdaptacion();
		if (grafoA.size() > 2 && grafoB.size() > 2){	// Solo se cruza si ambos tienen suficientes nodos como para cortar en 2 trozos
			std::vector<Grafo<Gen>> subsA = grafoA.divideGrafo(RandomGen::getRandom(1u, grafoA.size() - 2));	// comienza en 1 y termina en size-2 para evitar subgrafos sin nodos
			std::vector<Grafo<Gen>> subsB = grafoB.divideGrafo(RandomGen::getRandom(1u, grafoB.size() - 2));

			Grafo<Gen> hijo1 = Grafo<Gen>::unirGrafo(subsA[0], subsB[1]);
			Grafo<Gen> hijo2 = Grafo<Gen>::unirGrafo(subsB[0], subsA[1]);
			
			a->setGenotipo(hijo1, p);
			b->setGenotipo(hijo2, p);

			unsigned int adapHijoA = a->getAdaptacion();
			unsigned int adapHijoB = b->getAdaptacion();
			if (adapHijoA < adaptacionA){
				// Si el hijo A no es mejor que el padre A se restituye
				a->setGenotipo(grafoA, p);
			}
			if (adapHijoB < adaptacionB){
				// Si el hijo A no es mejor que el padre A se restituye
				b->setGenotipo(grafoB, p);
			}
		}
		
	}

private:
};

#endif