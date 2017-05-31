#ifndef METODOCRUCE_HPP
#define METODOCRUCE_HPP

#include "myRandom.hpp"
#include "Cromosoma.hpp"

class MetodoCruce {
public:

	virtual void cruzar(Cromosoma*, Cromosoma*) = 0;

	virtual std::string toString() = 0;

};

class CruceMonopunto : public MetodoCruce{
public:
	void cruzar(Cromosoma* a, Cromosoma* b){
		Grafo<Gen> grafoA = a->getGenotipo();
		Grafo<Gen> grafoB = b->getGenotipo();
		double adaptacionA = a->getAdaptacion();
		double adaptacionB = b->getAdaptacion();
		if (grafoA.size() > 2 && grafoB.size() > 2){	// Solo se cruza si ambos tienen suficientes nodos como para cortar en 2 trozos
			unsigned int corteA = myRandom::getRandom(1u, grafoA.size() - 2);
			unsigned int corteB = myRandom::getRandom(1u, grafoB.size() - 2);

			std::vector<Grafo<Gen>> subsA = grafoA.divideGrafo(corteA);	// comienza en 1 y termina en size-2 para evitar subgrafos sin nodos
			std::vector<Grafo<Gen>> subsB = grafoB.divideGrafo(corteB);

			Grafo<Gen> hijo1 = Grafo<Gen>::unirGrafo(subsA[0], subsB[1]);
			Grafo<Gen> hijo2 = Grafo<Gen>::unirGrafo(subsB[0], subsA[1]);


			// Se comprueba que ninguna sala fuera del rango sea vital (tenga un rol)
			std::vector<unsigned int> rolesA = a->getRolesSala();
			std::vector<unsigned int> rolesACopia = a->getRolesSala();
			for (size_t i = 0; i < rolesA.size(); ++i){
				if (rolesA[i] >= hijo1.size()){
					bool ok;
					do{
						ok = true;
						rolesA[i] = myRandom::getRandom(0, int(hijo1.size() - 1));
						for (size_t j = 0; j < rolesA.size() && ok; ++j){
							if (i != j){
								if (rolesA[i] == rolesA[j]){
									ok = false;
								}
							}
						}
					} while (!ok);
				}
			}

			a->setRolesSala(rolesA);

			std::vector<unsigned int> rolesB = b->getRolesSala();
			std::vector<unsigned int> rolesBCopia = b->getRolesSala();
			for (size_t i = 0; i < rolesB.size(); ++i){
				if (rolesB[i] >= hijo1.size()){
					bool ok;
					do{
						ok = true;
						rolesB[i] = myRandom::getRandom(0, int(hijo1.size() - 1));
						for (size_t j = 0; j < rolesB.size() && ok; ++j){
							if (i != j){
								if (rolesB[i] == rolesB[j]){
									ok = false;
								}
							}
						}
					} while (!ok);
				}
			}

			b->setRolesSala(rolesB);

			a->setGenotipo(hijo1);
			b->setGenotipo(hijo2);

		}

	}

	std::string toString(){
		return "Cruce Monopunto";
	}

private:
};

class CruceMultipunto : public MetodoCruce{
public:
	void cruzar(Cromosoma* a, Cromosoma* b){
		Grafo<Gen> grafoA = a->getGenotipo();
		Grafo<Gen> grafoB = b->getGenotipo();
		double adaptacionA = a->getAdaptacion();
		double adaptacionB = b->getAdaptacion();
		unsigned int cortes = myRandom::getRandom(2u, 4u);

		if (grafoA.size() > cortes + 1 && grafoB.size() > cortes + 1){	// Solo se cruza si ambos tienen suficientes nodos como para cortar
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

			a->setGenotipo(hijo1);
			b->setGenotipo(hijo2);
		}

	}

	std::string toString(){
		return "Cruce Multipunto";
	}

private:
};

#endif