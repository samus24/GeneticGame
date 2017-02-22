#ifndef RELLENADOR_HPP
#define RELLENADOR_HPP

#include "Mazmorra.hpp"

class Rellenador {
public:
	// Considerar hacer esta clase Singleton
	Rellenador(){

	}

	static Mazmorra rellenaMazmorra(Grafo<Gen> grafo){
		// La entrada de este metodo se considera la MEJOR COMPONENTE CONEXA
		Mazmorra m;
		std::unordered_map< unsigned int, Gen > salas = grafo.getNodos();
		std::unordered_map< unsigned int, std::set<unsigned int> > uniones = grafo.getAdyacencia();
		auto itSalas = salas.begin();
		while (itSalas != salas.cend()){
			Sala s(itSalas->first, itSalas->second.getAncho(), itSalas->second.getAlto());	// Las salas se crean vacias
			unsigned int nUniones = uniones[itSalas->first].size();
			auto itUniones = uniones[itSalas->first].begin();
			std::vector<Pair<int, int>> puertas = posicionesPuertas(itSalas->second.getAncho(), itSalas->second.getAlto(), nUniones);
			for (Pair<int, int> p : puertas){
				s.setCelda(p.first, p.second, *itUniones);
				++itUniones;
			}
			std::vector<Pair<int, int>> cofres = posicionesCofres(itSalas->second.getAncho(), itSalas->second.getAlto(), s, itSalas->second.getCofres());
			for (Pair<int, int> p : cofres){
				s.setCelda(p.first, p.second, Sala::COFRE);
			}
			std::vector<Pair<int, int>> enemigos = posicionesEnemigos(itSalas->second.getAncho(), itSalas->second.getAlto(), s, itSalas->second.getEnemigos());
			for (Pair<int, int> p : enemigos){
				s.setCelda(p.first, p.second, Sala::ENEMIGO);
			}
			m.anadeSala(s);
			++itSalas;
		}
		return m;
	}
private:
	static std::vector<Pair<int, int>> posicionesPuertas(unsigned int ancho, unsigned int alto, unsigned int nUniones){
		std::vector<Pair<int, int>> ret;
		// Las dimensiones de las salas son min=5 y max=40 (por como se generan en el grafo)

		int puertasPorLado[4];	// Con este array, indicamos cuantas puertas tendra cada pared;

		// A todos se les distribuyen las mismas puertas
		puertasPorLado[UP] = nUniones / 4;	
		puertasPorLado[LEFT] = nUniones / 4;
		puertasPorLado[DOWN] = nUniones / 4;
		puertasPorLado[RIGHT] = nUniones / 4;

		// Si hay puertas sobrantes, se asignan de forma aleatoria
		int resto = nUniones % 4;
		while (resto > 0){
			puertasPorLado[RandomGen::getRandom(0u, 3u)]++;
			resto--;
		}

		unsigned int step;
		if (puertasPorLado[UP] != 0)	step = std::ceil((double)ancho / (puertasPorLado[UP] + 1));
		for (size_t i = 0; i < puertasPorLado[UP]; ++i)
			ret.push_back(Pair<int, int>((step*(i+1))%ancho, 0));
		if (puertasPorLado[DOWN] != 0)	step = std::ceil((double)ancho / (puertasPorLado[DOWN] + 1));
		for (size_t i = 0; i < puertasPorLado[DOWN]; ++i)
			ret.push_back(Pair<int, int>((step*(i + 1)) % ancho, alto - 1));
		if (puertasPorLado[LEFT] != 0)	step = std::ceil((double)alto / (puertasPorLado[LEFT] + 1));
		for (size_t i = 0; i < puertasPorLado[LEFT]; ++i)
			ret.push_back(Pair<int, int>(0,(step*(i + 1)%alto)));
		if (puertasPorLado[RIGHT] != 0)	step = std::ceil((double)alto / (puertasPorLado[RIGHT] + 1));
		for (size_t i = 0; i < puertasPorLado[RIGHT]; ++i)
			ret.push_back(Pair<int, int>(ancho - 1, (step*(i + 1) % alto)));

		return ret;
	}

	static std::vector<Pair<int, int>> posicionesCofres(unsigned int ancho, unsigned int alto, Sala s, unsigned int nCofres){
		std::vector<Pair<int, int>> ret;
		int rand;
		while (nCofres > 0){
			rand = RandomGen::getRandom(0, 3);	// random wall
			switch (rand){
			case UP:
				do{
					rand = RandomGen::getRandom(0u, ancho - 1);
				} while (s[rand][0] >= 0);	// while there was a door in that pos
				ret.push_back(Pair<int, int>(rand, 0));
				break;
			case DOWN:
				do{
					rand = RandomGen::getRandom(0u, ancho - 1);
				} while (s[rand][alto - 1] >= 0);	// while there was a door in that pos
				ret.push_back(Pair<int, int>(rand, alto - 1));
				break;
			case LEFT:
				do{
					rand = RandomGen::getRandom(0u, alto - 1);
				} while (s[0][rand] >= 0);	// while there was a door in that pos
				ret.push_back(Pair<int, int>(0,rand));
				break;
			case RIGHT:
				do{
					rand = RandomGen::getRandom(0u, alto - 1);
				} while (s[ancho-1][rand] >= 0);	// while there was a door in that pos
				ret.push_back(Pair<int, int>(ancho-1, rand));
				break;
			}
			--nCofres;
		}
		return ret;
	}

	static std::vector<Pair<int, int>> posicionesEnemigos(unsigned int ancho, unsigned int alto, Sala s, unsigned int nEnemigos){
		std::vector<Pair<int, int>> ret;
		int randX, randY;
		while (nEnemigos > 0){
			do{
				randX = RandomGen::getRandom(0u, ancho - 1);
				randY = RandomGen::getRandom(0u, alto - 1);
			} while ((s[randX][randY] >= 0) || (s[randX][randY] == Sala::COFRE));	// while there was a door or chest in that pos
			ret.push_back(Pair<int, int>(randX, randY));
			--nEnemigos;
		}
		return ret;
	}

	static const int UP = 0;
	static const int LEFT = 1;
	static const int DOWN = 2;
	static const int RIGHT = 3;

};


#endif