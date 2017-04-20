#ifndef CROMOSOMA_HPP
#define CROMOSOMA_HPP

#include <stack>
#include "Arbol.hpp"
#include "myrandom.hpp"

class Cromosoma {
public:

	Cromosoma() {
		_punt = 0;
		_puntAcum = 0;
		_adaptacion = 0;
	}

	Cromosoma(int profMin, int profMax) {
		this->_genotipo[0].creaArbolAleatorio(profMin, profMax);
		this->_genotipo[1].creaArbolAleatorio(profMin, profMax);
	}

	void crear(int profMin, int profMax){
		this->_genotipo[0].creaArbolAleatorio(profMin, profMax);
		this->_genotipo[1].creaArbolAleatorio(profMin, profMax);
		if (!this->_genotipo[0].compruebaIntegridad()){
			return;
		}
		if (!this->_genotipo[1].compruebaIntegridad()){
			return;
		}
	}

	Arbol getGenotipo(int pos) {
		return _genotipo[pos];
	}

	void setGenotipo(Arbol genotipo, int pos) {
		this->_genotipo[pos] = genotipo;
	}

	double getPunt() {
		return _punt;
	}

	void setPunt(double punt) {
		this->_punt = punt;
	}

	double getPuntAcum() {
		return this->_puntAcum;
	}

	void setPuntAcum(double p) {
		this->_puntAcum = p;
	}

	double getAdaptacion() {
		return this->_adaptacion;
	}

	void setAdaptacion(double a) {
		this->_adaptacion = a;
	}

	Nodo getTerminalAleatorio(int pos) {
		return _genotipo[pos].getTerminalAleatorio();
	}

	Nodo getNodoFuncionAleatorio(int pos) {
		return _genotipo[pos].getNodoFuncionAleatorio();
	}

	void bloating(int prof) {
		for (std::size_t i = 0; i < 2; ++i) {
			_genotipo[i].bloating(prof);
		}
	}

	double evalua(Mapa m, int posX, int posY) {
		//metodo de evaluación, recorrer los mapas y conseguir el valor acumulado
		//cargar mapa con la posicion del jugador
		//colocar al npc aleatoriamente
		//while de la patrulla si turnos no acabados o no cambiar estado.
		//si cambiar estado, arbol ataque mientras turnos no acabados o jugador no eliminado
		std::stack<Nodo*> pila;
		Mapa explorado = m;
		npc jugador(posX, posY, m.getHeight(), m.getWidth());
		npc enemigo = setRandomPosition(m);
		bool ataque = false;
		Nodo* actual;
		int x, y;
		int incx = 0, incy = 0;
		bool fin = false;
		int cont = 0;

		while (enemigo._turnos < 100 && !ataque) {
			if (pila.empty()) {
				pila.push(_genotipo[0].getRaiz());
			}
			actual = pila.top();
			pila.pop();
			Operacion op = actual->getElem();
			switch (op) {
			case ProgN2:
				pila.push(&actual->getHijos()[1]); //Se introducen en orden inverso que es como se quieren ejecutar.
				pila.push(&actual->getHijos()[0]);
				break;
			case ProgN3:
				pila.push(&actual->getHijos()[2]);
				pila.push(&actual->getHijos()[1]); //Se introducen en orden inverso que es como se quieren ejecutar.
				pila.push(&actual->getHijos()[0]);
				break;
			case ProgN4:
				pila.push(&actual->getHijos()[3]);
				pila.push(&actual->getHijos()[2]);
				pila.push(&actual->getHijos()[1]); //Se introducen en orden inverso que es como se quieren ejecutar.
				pila.push(&actual->getHijos()[0]);
				break;
			case SiBloqueado:
				
				if (!enemigo.getCasillaDelante(x, y)) {
					pila.push(&actual->getHijos()[0]);
				}
				else {
					pila.push(&actual->getHijos()[1]);
				}
				break;
			case SiDetectado:
				
				if (enemigo.getCasillaDelante(x, y)) {
					switch (enemigo.f) {
					case NORTE:
						incy--;
						break;
					case SUR:
						incy++;
						break;
					case ESTE:
						incx++;
						break;
					case OESTE:
						incx++;
						break;
					default:
						break;
					}
					while (!fin) {
						if (jugador._posX == x && jugador._posY == y) {
							fin = true;
							pila.push(&actual->getHijos()[0]);
							explorado[x][y] = 5;
						}
						else if (m[x][y] != m.VACIO) {
							fin = true;
							pila.push(&actual->getHijos()[1]);
						}
						else {
							x += incx;
							y += incy;
							if (x < 0 || x >= m.getWidth() || y < 0 || y >= m.getHeight()) {
								fin = true;
								pila.push(&actual->getHijos()[1]);
							}
							else {
								explorado[x][y] = 1;
							}
						}
					}
				}
				else {
					pila.push(&actual->getHijos()[1]);
				}
				break;
			case Avanza:
				int x, y;
				if (enemigo.getCasillaDelante(x, y)) {
					enemigo.avanza();
				}
				enemigo._turnos++;
				break;
			case GiraIz:
				enemigo.izquierda();
				enemigo._turnos++;
				break;
			case GiraDer:
				enemigo.derecha();
				enemigo._turnos++;
				break;
			case CambiarEst:
				while (!pila.empty()) {
					pila.pop(); //vaciamos la pila por si quedasen nodos sin evaluar pero ya hemos cambiado a ataque.
				}
				enemigo._turnos++;
				ataque = true;
				break;
			default:
				break;
			}
			mueveJugador(jugador, enemigo);
		}

		enemigo.turnosPatrulla = enemigo._turnos;

		while (enemigo._turnos < 100 && jugador.heridas < 3) {
			if (pila.empty()) {
				pila.push(_genotipo[1].getRaiz());
			}
			actual = pila.top();
			pila.pop();
			Operacion op = actual->getElem();
			switch (op) {
			case ProgN2:
				pila.push(&actual->getHijos()[1]); //Se introducen en orden inverso que es como se quieren ejecutar.
				pila.push(&actual->getHijos()[0]);
				break;
			case ProgN3:
				pila.push(&actual->getHijos()[2]);
				pila.push(&actual->getHijos()[1]); //Se introducen en orden inverso que es como se quieren ejecutar.
				pila.push(&actual->getHijos()[0]);
				break;
			case ProgN4:
				pila.push(&actual->getHijos()[3]);
				pila.push(&actual->getHijos()[2]);
				pila.push(&actual->getHijos()[1]); //Se introducen en orden inverso que es como se quieren ejecutar.
				pila.push(&actual->getHijos()[0]);
				break;
			case SiJugador:
				enemigo.getCasillaDelante(x, y);
				if (jugador._posX == x && jugador._posY == y) {
					pila.push(&actual->getHijos()[0]);
				}
				else {
					pila.push(&actual->getHijos()[1]);
				}
				break;
			case SiBloqueado:

				if (!enemigo.getCasillaDelante(x, y)) {
					pila.push(&actual->getHijos()[0]);
				}
				else {
					pila.push(&actual->getHijos()[1]);
				}
				break;
			case SiRango:
				if (enemigo.getCasillaDelante(x, y)) {
					switch (enemigo.f) {
					case NORTE:
						incy--;
						break;
					case SUR:
						incy++;
						break;
					case ESTE:
						incx++;
						break;
					case OESTE:
						incx++;
						break;
					default:
						break;
					}
					while (cont < enemigo.rango && !fin) {
						if (jugador._posX == x && jugador._posY == y) {
							fin = true;
							pila.push(&actual->getHijos()[0]);
						}
						else if (m[x][y] != m.VACIO) {
							fin = true;
							pila.push(&actual->getHijos()[1]);
						}
						else {
							x += incx;
							y += incy;
							if (x < 0 || x >= m.getWidth() || y < 0 || y >= m.getHeight()) {
								fin = true;
								pila.push(&actual->getHijos()[1]);
							}
						}
						cont++;
					}
				}
				else {
					pila.push(&actual->getHijos()[1]);
				}
				break;
			case SiDetectado:
				if (enemigo.getCasillaDelante(x, y)) {
					switch (enemigo.f) {
					case NORTE:
						incy--;
						break;
					case SUR:
						incy++;
						break;
					case ESTE:
						incx++;
						break;
					case OESTE:
						incx++;
						break;
					default:
						break;
					}
					while (!fin) {
						if (jugador._posX == x && jugador._posY == y) {
							fin = true;
							pila.push(&actual->getHijos()[0]);
						}
						else if (m[x][y] != m.VACIO) {
							fin = true;
							pila.push(&actual->getHijos()[1]);
						}
						else {
							x += incx;
							y += incy;
							if (x < 0 || x >= m.getWidth() || y < 0 || y >= m.getHeight()) {
								fin = true;
								pila.push(&actual->getHijos()[1]);
							}
						}
					}
				}
				else {
					pila.push(&actual->getHijos()[1]);
				}
				break;
			case Avanza:
				int x, y;
				if (enemigo.getCasillaDelante(x, y)) {
					enemigo.avanza();
				}
				enemigo._turnos++;
				break;
			case GiraIz:
				enemigo.izquierda();
				enemigo._turnos++;
				break;
			case GiraDer:
				enemigo.derecha();
				enemigo._turnos++;
				break;
			case BloquearN:
				enemigo.bloqueando = true;
				break;
			case Atacar:
				enemigo.getCasillaDelante(x, y);
				if (jugador._posX == x && jugador._posY == y) {
					enemigo.golpes++;
					if (!jugador.bloqueando) {
						jugador.heridas++;
					}
				}
				enemigo._turnos++;
				break;
			case Retroceder:
				if (enemigo.getCasillaDetras(x, y)) {
					enemigo.retroceder();
				}
				enemigo._turnos++;
				break;
			default:
				break;
			}
			mueveJugador(jugador, enemigo);
		}
		double evaluacion = 0;
		double pesos[] = {0.1, 0.4, 0.15, 0.05, 0.30}; //turnosPatrulla (minimizar), casillasExploradas (maximizar), golpes (maximizar), heridas (minimizar), daño (maximizar)
		double dim = m.getHeight() * m.getWidth();
		double optimos[] = { 1.f, dim, 20.f, 0.f, 3.f };
		double valores[5];

		valores[0] = (1 / enemigo._turnos);
		if (valores[0] < 0) valores[0] = 0;

		valores[1] = 1 - (abs(casillasExploradas(explorado) - optimos[1]) / optimos[1]);
		if (valores[1] < 0) valores[1] = 0;

		valores[2] = 1 - (abs(enemigo.golpes - optimos[2]) / optimos[2]);
		if (valores[2] < 0) valores[2] = 0;

		valores[3] = (1 / enemigo.heridas);
		if (valores[3] < 0) valores[3] = 0;

		valores[4] = 1 - (abs(jugador.heridas * optimos[4]) / optimos[4]);
		if (valores[4] < 0) valores[4] = 0;

		for (int i = 0; i < 5; ++i){
			evaluacion += valores[i] * pesos[i];
		}

		return evaluacion;
	}

	int casillasExploradas(Mapa m) {
		int ret = 0;
		for (std::size_t i = 0; i < m.getWidth(); ++i) {
			for (std::size_t j = 0; j < m.getHeight(); ++j) {
				if (m[i][j] > 0) {
					ret += m[i][j];
				}
			}
		}
		return ret;
	}

	void mueveJugador(npc &jugador, npc &enemigo) {
		jugador.bloqueando = false;
		Operacion op = (Operacion)myRandom::getRandom(Operacion::Avanza, Operacion::Retroceder);
		switch (op) {
		case Avanza:
			int x, y;
			if (jugador.getCasillaDelante(x, y)) {
				jugador.avanza();
			}
			break;
		case GiraIz:
			jugador.izquierda();
			break;
		case GiraDer:
			jugador.derecha();
			break;
		case BloquearN:
			jugador.bloqueando = true;
			break;
		case Atacar:
			jugador.getCasillaDelante(x, y);
			if (enemigo._posX == x && enemigo._posY == y) {
				if (!enemigo.bloqueando) {
					enemigo.heridas++;
				}
			}
			break;
		case Retroceder:
			if (jugador.getCasillaDetras(x, y)) {
				jugador.retroceder();
			}
			break;
		default:
			break;
		}
		enemigo.bloqueando = false;
	}

	npc setRandomPosition(Mapa &m) {
		int x, y;
		do{
			x = myRandom::getRandom(0, m.getWidth() -1);
			y = myRandom::getRandom(0, m.getHeight() - 1);
		} while (m[x][y] != m.VACIO);
		npc enemigo(x, y, m.getHeight(), m.getWidth());
		return enemigo;
	}



	void eliminaIntrones() {
		for (std::size_t i = 0; i < 2; ++i) {
			_genotipo[i].eliminaIntrones();
		}
	}

private:
	Arbol _genotipo[2]; //la primera posición es el árbol de patrulla y la segunda el de ataque.
	double _punt;
	double _puntAcum;
	double _adaptacion;
};

#endif