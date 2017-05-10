#ifndef METODOMUTACION_HPP
#define METODOMUTACION_HPP

#include "RandomGen.hpp"
#include "Cromosoma.hpp"

class MetodoMutacion {
public:

	virtual void mutar(Cromosoma* c) = 0;

	virtual std::string toString() = 0;
};

class MutacionArista : public MetodoMutacion {
public:
	void mutar(Cromosoma* c){
		Grafo<Gen> g = c->getGenotipo();
		auto ady = g.getAdyacencia();
		unsigned int randNodo, randArista;
		unsigned int vecinos;
		unsigned intentos = 5;	// Se limita el numero de intentos
		
		if (RandomGen::getRandom(0u, 1u)){
			// Se crea una arista aleatoria
			unsigned int randNodo2, vecinos2;
			bool ok = false;
			do{
				randNodo = RandomGen::getRandom(0u, ady.size() - 1);
				randNodo2 = RandomGen::getRandom(0u, ady.size() - 1);
				ok = randNodo != randNodo2 &&
					(ady[randNodo].find(randNodo2) == ady[randNodo].cend());
					//&&(ady[randNodo2].find(randNodo) == ady[randNodo2].cend());	// Si el grafo esta bien construido, se puede evitar esta llamada
				if (ok){
					ady[randNodo].emplace(randNodo2);
					ady[randNodo2].emplace(randNodo);
				}
				else{
					intentos--;
				}
			} while (!ok && intentos > 0);
		}
		else{
			// Se borra una arista aleatoria
			do{
				randNodo = RandomGen::getRandom(0u, ady.size()-1);
				vecinos = ady[randNodo].size();
				if (vecinos > 0){
					randArista = RandomGen::getRandom(0u, vecinos-1);
					auto otroNodo = ady[randNodo].begin();
					std::advance(otroNodo, randArista);
					unsigned int idOtroNodo = *otroNodo;
					ady[randNodo].erase(otroNodo);
					auto esteNodo = ady[idOtroNodo].find(randNodo);
					ady[idOtroNodo].erase(esteNodo);
				}
				else{
					intentos--;
				}
			} while (vecinos == 0 && intentos > 0);
		}
		g.setAdyacencia(ady);
		c->setGenotipo(g);	
	}

	std::string toString(){
		return "Mutacion Arista";
	}
};

class MutacionNodo : public MetodoMutacion {
public:
	void mutar(Cromosoma* c){
		Grafo<Gen> g = c->getGenotipo();
		if (RandomGen::getRandom(0u, 1u)){
			// Se crea una nuevo nodo
			Gen nuevo;
			bool ok = false;
			unsigned int nuevoId = g.size();
			do{
				try{
					g.anadeNodo(nuevo, nuevoId);
					ok = true;
				}
				catch (std::invalid_argument e){
					ok = false;
					nuevoId++;
				}
			} while (!ok);
		}
		else{
			if (g.size() <= 3) return;	// no se puede hacer esta mutacion con 3 menos nodos
			auto roles = c->getRolesSala();
			unsigned int randNodo;
			auto nodos = g.getNodos();
			auto ady = g.getAdyacencia();
			// Se borra un nodo aleatorio
			randNodo = RandomGen::getRandom(0u, ady.size() - 1);
			nodos.erase(randNodo);
			for (auto i : ady[randNodo]){
				ady[i].erase(randNodo);
			}
			ady.erase(randNodo);
			g.setNodos(nodos);
			g.setAdyacencia(ady);

			// Si el nodo borrado tenia un rol, se asigna ese rol a otro nodo
			for (size_t i = 0; i < roles.size(); ++i){
				if (roles[i] = randNodo){
					bool ok;
					do{
						ok = true;
						roles[i] = RandomGen::getRandom(0, int(ady.size() - 1));
						for (size_t j = 0; j < roles.size() && ok; ++j){
							if (i != j){
								if (roles[i] == roles[j]){
									ok = false;
								}
							}
						}
					} while (!ok);
				}
			}
			c->setRolesSala(roles);
		}
		c->setGenotipo(g);
	}

	std::string toString(){
		return "Mutacion Nodo";
	}
};

class MutacionSala : public MetodoMutacion {
public:
	void mutar(Cromosoma* c){
		Grafo<Gen> g = c->getGenotipo();
		if (g.size() <= 3) return;	// no se puede hacer esta mutacion con 3 menos nodos
		auto roles = c->getRolesSala();
		unsigned int randRol = RandomGen::getRandom(0u, roles.size() - 1);
		unsigned int lastNodo = roles[randRol];
		unsigned int randNodo;
		bool ok;
		do{
			ok = true;
			do{
				randNodo = RandomGen::getRandom(0u, g.size() - 1);
			} while (randNodo == lastNodo);
			roles[randRol] = randNodo;
			for (size_t j = 0; j < roles.size() && ok; ++j){
				if (randRol != j){
					if (roles[randRol] == roles[j]){
						ok = false;
					}
				}
			}
		} while (!ok);
		c->setRolesSala(roles);
	}

	std::string toString(){
		return "Mutacion Sala";
	}
};

class MutacionCombinada : public MetodoMutacion {
public:
	void mutar(Cromosoma* c){
		Grafo<Gen> g = c->getGenotipo();
		unsigned int opcion = RandomGen::getRandom(0u, 2u);
		if (opcion == 0){
			// Mutacion Nodo
			if (RandomGen::getRandom(0u, 1u)){
				// Se crea una nuevo nodo
				Gen nuevo;
				bool ok = false;
				unsigned int nuevoId = g.size();
				do{
					try{
						g.anadeNodo(nuevo, nuevoId);
						ok = true;
					}
					catch (std::invalid_argument e){
						ok = false;
						nuevoId++;
					}
				} while (!ok);
			}
			else{
				if (g.size() <= 3) return;	// no se puede hacer esta mutacion con 3 menos nodos
				auto roles = c->getRolesSala();
				unsigned int randNodo;
				auto nodos = g.getNodos();
				auto ady = g.getAdyacencia();
				// Se borra un nodo aleatorio
				randNodo = RandomGen::getRandom(0u, ady.size() - 1);
				nodos.erase(randNodo);
				for (auto i : ady[randNodo]){
					ady[i].erase(randNodo);
				}
				ady.erase(randNodo);
				g.setNodos(nodos);
				g.setAdyacencia(ady);

				// Si el nodo borrado tenia un rol, se asigna ese rol a otro nodo
				for (size_t i = 0; i < roles.size(); ++i){
					if (roles[i] = randNodo){
						bool ok;
						do{
							ok = true;
							roles[i] = RandomGen::getRandom(0, int(ady.size() - 1));
							for (size_t j = 0; j < roles.size() && ok; ++j){
								if (i != j){
									if (roles[i] == roles[j]){
										ok = false;
									}
								}
							}
						} while (!ok);
					}
				}
				c->setRolesSala(roles);
			}
		}
		else if (opcion == 1){
			// Mutacion Arista
			auto ady = g.getAdyacencia();
			unsigned int randNodo, randArista;
			unsigned int vecinos;
			unsigned intentos = 5;	// Se limita el numero de intentos

			if (RandomGen::getRandom(0u, 1u)){
				// Se crea una arista aleatoria
				unsigned int randNodo2, vecinos2;
				bool ok = false;
				do{
					randNodo = RandomGen::getRandom(0u, ady.size() - 1);
					randNodo2 = RandomGen::getRandom(0u, ady.size() - 1);
					ok = randNodo != randNodo2 &&
						(ady[randNodo].find(randNodo2) == ady[randNodo].cend());
					//&&(ady[randNodo2].find(randNodo) == ady[randNodo2].cend());	// Si el grafo esta bien construido, se puede evitar esta llamada
					if (ok){
						ady[randNodo].emplace(randNodo2);
						ady[randNodo2].emplace(randNodo);
					}
					else{
						intentos--;
					}
				} while (!ok && intentos > 0);
			}
			else{
				// Se borra una arista aleatoria
				do{
					randNodo = RandomGen::getRandom(0u, ady.size() - 1);
					vecinos = ady[randNodo].size();
					if (vecinos > 0){
						randArista = RandomGen::getRandom(0u, vecinos - 1);
						auto otroNodo = ady[randNodo].begin();
						std::advance(otroNodo, randArista);
						unsigned int idOtroNodo = *otroNodo;
						ady[randNodo].erase(otroNodo);
						auto esteNodo = ady[idOtroNodo].find(randNodo);
						ady[idOtroNodo].erase(esteNodo);
					}
					else{
						intentos--;
					}
				} while (vecinos == 0 && intentos > 0);
			}
			g.setAdyacencia(ady);
		}
		else if (opcion == 2){
			// Mutacion Sala
			if (g.size() <= 3) return;	// no se puede hacer esta mutacion con 3 menos nodos
			auto roles = c->getRolesSala();
			unsigned int randRol = RandomGen::getRandom(0u, roles.size() - 1);
			unsigned int lastNodo = roles[randRol];
			unsigned int randNodo;
			bool ok;
			do{
				ok = true;
				do{
					randNodo = RandomGen::getRandom(0u, g.size() - 1);
				} while (randNodo == lastNodo);
				roles[randRol] = randNodo;
				for (size_t j = 0; j < roles.size() && ok; ++j){
					if (randRol != j){
						if (roles[randRol] == roles[j]){
							ok = false;
						}
					}
				}
			} while (!ok);
			c->setRolesSala(roles);
		}
		
		c->setGenotipo(g);
	}

	std::string toString(){
		return "Mutacion Combinada";
	}
};

#endif