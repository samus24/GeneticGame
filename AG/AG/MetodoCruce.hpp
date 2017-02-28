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
		Grafo<Gen> grafoB = b->getGenotipo();
		double adaptacionA = a->getAdaptacion();
		double adaptacionB = b->getAdaptacion();
		if (grafoA.size() > 2 && grafoB.size() > 2){	// Solo se cruza si ambos tienen suficientes nodos como para cortar en 2 trozos
			std::vector<Grafo<Gen>> subsA = grafoA.divideGrafo(RandomGen::getRandom(1u, grafoA.size() - 2));	// comienza en 1 y termina en size-2 para evitar subgrafos sin nodos
			std::vector<Grafo<Gen>> subsB = grafoB.divideGrafo(RandomGen::getRandom(1u, grafoB.size() - 2));

			Grafo<Gen> hijo1 = Grafo<Gen>::unirGrafo(subsA[0], subsB[1]);
			Grafo<Gen> hijo2 = Grafo<Gen>::unirGrafo(subsB[0], subsA[1]);
			
			a->setGenotipo(hijo1, p);
			b->setGenotipo(hijo2, p);

			double adapHijoA = a->getAdaptacion();
			double adapHijoB = b->getAdaptacion();
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

class CruceMultipunto : public MetodoCruce{
public:
	void cruzar(Cromosoma* a, Cromosoma* b, ParametrosEval p){
		Grafo<Gen> grafoA = a->getGenotipo();
		Grafo<Gen> grafoB = b->getGenotipo();
		double adaptacionA = a->getAdaptacion();
		double adaptacionB = b->getAdaptacion();
		unsigned int cortes = RandomGen::getRandom(2u, 4u);
		
		if (grafoA.size() > cortes+1 && grafoB.size() > cortes+1){	// Solo se cruza si ambos tienen suficientes nodos como para cortar
			std::vector<Grafo<Gen>> subsA = grafoA.divideEnGrafos(cortes);
			std::vector<Grafo<Gen>> subsB = grafoB.divideEnGrafos(cortes);

			for (size_t i = 0; i < cortes; ++i){
				if (i % 2 != 0){
					// Se intercambian los subgrafos impares
					Grafo<Gen> aux = subsA[i];
					subsA[i] = subsB[i];
					subsB[i] = aux;
				}
			}

			Grafo<Gen> hijo1 = Grafo<Gen>::unirGrafos(subsA);
			Grafo<Gen> hijo2 = Grafo<Gen>::unirGrafos(subsB);

			a->setGenotipo(hijo1, p);
			b->setGenotipo(hijo2, p);

			double adapHijoA = a->getAdaptacion();
			double adapHijoB = b->getAdaptacion();
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