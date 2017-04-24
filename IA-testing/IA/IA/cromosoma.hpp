#ifndef CROMOSOMA_HPP
#define CROMOSOMA_HPP

#define NOMINMAX

#include <iostream>
#include <stack>
#include <algorithm>
#include <Windows.h>
#include "Arbol.hpp"
#include "myrandom.hpp"

const int AZUL = 9;
const int VERDE = 10;
const int TURQUESA = 11;
const int ROJO = 12;
const int ROSA = 13;
const int AMARILLO = 14;
const int BLANCO = 15;

void cambiaColor(int color){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

void imprimeEstado(Arbol gen, Mapa m, Mapa explorado, npc enemigo, npc jugador, bool ataque){
	system("cls");
	cambiaColor(BLANCO);
	char c;
	std::cout << "Estado: " << ((ataque) ? "Ataque" : "Patrulla") << std::endl;
	std::cout << gen.toString() << std::endl;
	std::cout << "<Enemigo> Tur: " << enemigo._turnos << " G: " << enemigo.golpes << " GE: " << enemigo.golpesEvitados << " B: " << enemigo.bloqueando << std::endl;
	std::cout << "<Jugador> " << "H: " << jugador.heridas << " B: " << jugador.bloqueando << std::endl;

	for (std::size_t j = 0; j < m.getHeight(); ++j) {
		for (std::size_t i = 0; i < m.getWidth(); ++i) {
			if (enemigo._posX == i && enemigo._posY == j){
				cambiaColor(ROJO);
				switch (enemigo.f) {
				case NORTE:
					std::cout << "^";
					break;
				case SUR:
					std::cout << "v";
					break;
				case ESTE:
					std::cout << ">";
					break;
				case OESTE:
					std::cout << "<";
					break;
				default:
					break;
				}
			}
			else if (jugador._posX == i && jugador._posY == j){
				cambiaColor(VERDE);
				switch (jugador.f) {
				case NORTE:
					std::cout << "^";
					break;
				case SUR:
					std::cout << "v";
					break;
				case ESTE:
					std::cout << ">";
					break;
				case OESTE:
					std::cout << "<";
					break;
				default:
					break;
				}
			}
			else{
				c = '-';
				if (explorado.getCasilla(i, j) > 0){
					cambiaColor(AZUL);
				}
				else{
					cambiaColor(BLANCO);
				}

				if (m.getCasilla(i, j) == m.MURO) {
					cambiaColor(TURQUESA);
					c = '#';
				}
				else if (m.getCasilla(i,j) == m.COFRE) {
					cambiaColor(AMARILLO);
					c = '*';
				}
				std::cout << c;
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

class Cromosoma {
public:

	Cromosoma() {
		_punt = 0;
		_puntAcum = 0;
		_adaptacion = 0;
		double pesos[4] = { 0.5, 0.15, 0.05, 0.30 };
		for (size_t i = 0; i < 4; ++i){
			_pesos[i] = pesos[i];
		}
	}

	Cromosoma(int profMin, int profMax) {
		this->_genotipo[0].creaArbolAleatorio(profMin, profMax, TipoArbol::Patrulla);
		this->_genotipo[1].creaArbolAleatorio(profMin, profMax, TipoArbol::Ataque);
		double pesos[4] = { 0.5, 0.15, 0.05, 0.30 };
		for (size_t i = 0; i < 4; ++i){
			_pesos[i] = pesos[i];
		}
	}

	void crear(int profMin, int profMax){
		this->_genotipo[0].creaArbolAleatorio(profMin, profMax, TipoArbol::Patrulla);
		this->_genotipo[1].creaArbolAleatorio(profMin, profMax, TipoArbol::Ataque);
		double pesos[4] = { 0.5, 0.15, 0.05, 0.30 };
		for (size_t i = 0; i < 4; ++i){
			_pesos[i] = pesos[i];
		}
	}

	Arbol getGenotipo(int pos) {
		return _genotipo[pos];
	}

	void setGenotipo(Arbol genotipo, int pos, std::vector<Mapa> m) {
		this->_genotipo[pos] = genotipo;
		this->evalua(m);
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

	void bloating(int prof, std::vector<Mapa> m) {
		for (std::size_t i = 0; i < 2; ++i) {
			_genotipo[i].bloating(prof, (TipoArbol)i);
		}
		this->evalua(m);
	}

	double evalua(std::vector<Mapa> maps, bool pintar = false) {
		int x, y;
		double media = 0;
		maps[0].toString();
		for (std::size_t i = 0; i < maps.size(); ++i) {
			x = maps[i].getX();
			y = maps[i].getY();
			media += evaluaMapa(maps[i], x, y, pintar);
		}
		this->_adaptacion = media / maps.size();
		return media / maps.size(); //se divide sin restar, ya que size da el total (de 1 a n)
	}

	void eliminaIntrones() {
		for (std::size_t i = 0; i < 2; ++i) {
			_genotipo[i].eliminaIntrones();
		}
	}

	double* getValores(){
		return _valores;
	}

	double* getPesos(){
		return _pesos;
	}

private:

	double evaluaMapa(Mapa m, int posX, int posY, bool dibujar) {
		int maxTurnos = 100;

		std::stack<Nodo*> pila;
		Mapa explorado = m;
		npc jugador(posX, posY, m.getHeight(), m.getWidth());
		npc enemigo(0, 0, m.getHeight(), m.getWidth());
		setPositionInRange(jugador, enemigo, 5, 10, m);
		bool ataque = false;
		Nodo* actual;
		int x = 0, y = 0;
		int incx = 0, incy = 0;
		bool fin = false;
		int cont = 0;

		while (enemigo._turnos < maxTurnos && !ataque) {
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
			/*case ProgN4:
				pila.push(&actual->getHijos()[3]);
				pila.push(&actual->getHijos()[2]);
				pila.push(&actual->getHijos()[1]); //Se introducen en orden inverso que es como se quieren ejecutar.
				pila.push(&actual->getHijos()[0]);
				break;*/
			case SiBloqueado:
				x = 0; y = 0;
				if (!enemigo.getCasillaDelante(x, y)) {
					pila.push(&actual->getHijos()[0]);
				}
				else if (m.estaBloqueado(x, y)) {
					pila.push(&actual->getHijos()[0]);
				}
				else {
					pila.push(&actual->getHijos()[1]);
				}
				break;
			case SiDetectado:
				x = 0; y = 0;
				incx = 0; incy = 0;
				fin = false;
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
						incx--;
						break;
					default:
						break;
					}
					while (!fin && m.coordValidas(x,y)) {
						if (jugador.estaEn(x, y)) {
							fin = true;
							pila.push(&actual->getHijos()[0]);
							explorado.setCasilla(x, y, 5);
						}
						else if (m.estaBloqueado(x, y)) {
							fin = true;
							pila.push(&actual->getHijos()[1]);
						}
						else {
							explorado.setCasilla(x, y, 1);
							x += incx;
							y += incy;
							if (!m.coordValidas(x,y)) {
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
				x = 0; y = 0;
				if (enemigo.getCasillaDelante(x, y)) {
					if (!m.estaBloqueado(x, y)) {
						enemigo.avanza();
					}
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
				std::cerr << "El arbol de patrulla tenia nodos indebidos (" << op << ")" << std::endl;
				exit(-1);
				break;
			}
			mueveJugador(jugador, enemigo, m);
			if (dibujar) imprimeEstado(this->_genotipo[0], m, explorado, enemigo, jugador, ataque);
		}

		enemigo.turnosPatrulla = enemigo._turnos;

		while (enemigo._turnos < maxTurnos && jugador.heridas < 3) {
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
			/*case ProgN4:
				pila.push(&actual->getHijos()[3]);
				pila.push(&actual->getHijos()[2]);
				pila.push(&actual->getHijos()[1]); //Se introducen en orden inverso que es como se quieren ejecutar.
				pila.push(&actual->getHijos()[0]);
				break;*/
			case SiJugador:
				x = 0; y = 0;
				enemigo.getCasillaDelante(x, y);
				if (jugador.estaEn(x, y)) {
					pila.push(&actual->getHijos()[0]);
				}
				else {
					pila.push(&actual->getHijos()[1]);
				}
				break;
			case SiBloqueado:
				x = 0; y = 0;
				if (!enemigo.getCasillaDelante(x, y)) {
					pila.push(&actual->getHijos()[0]);
				}
				else if (m.estaBloqueado(x, y)) {
					pila.push(&actual->getHijos()[0]);
				}
				else {
					pila.push(&actual->getHijos()[1]);
				}
				break;
			case SiRango:
				x = 0; y = 0;
				cont = 0;
				fin = false;
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
					while (cont < enemigo.rango && !fin && m.coordValidas(x,y)) {
						if (jugador.estaEn(x, y)) {
							fin = true;
							pila.push(&actual->getHijos()[0]);
						}
						else if (!m.estaBloqueado(x, y)) {
							fin = true;
							pila.push(&actual->getHijos()[1]);
						}
						else {
							x += incx;
							y += incy;
							if (!m.coordValidas(x,y)) {
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
			case Avanza:
				x = 0; y = 0;
				if (enemigo.getCasillaDelante(x, y)) {
					if (!m.estaBloqueado(x, y)) {
						enemigo.avanza();
					}
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
				enemigo._turnos++;
				break;
			case Atacar:
				x = 0; y = 0;
				enemigo.getCasillaDelante(x, y);
				if (jugador.estaEn(x, y)) {
					enemigo.golpes++;
					if (!jugador.bloqueando) {
						jugador.heridas++;
					}
				}
				enemigo._turnos++;
				break;
			case Retroceder:
				x = 0; y = 0;
				if (enemigo.getCasillaDetras(x, y)) {
					if (!m.estaBloqueado(x, y)) {
						enemigo.retroceder();
					}
				}
				enemigo._turnos++;
				break;
			default:
				std::cerr << "El arbol de ataque tenia nodos indebidos (" << op << ")" << std::endl;
				exit(-1);
				break;
			}
			mueveJugador(jugador, enemigo, m);
			if (dibujar) imprimeEstado(this->_genotipo[1], m, explorado, enemigo, jugador, ataque);
		}
		double evaluacion = 0;
		double dim = m.getHeight() * m.getWidth();
		double optimos[] = {dim, 20.f, 10.f, 3.f };
		int cExpl = casillasExploradas(explorado);
		/*
		_valores[0] = 1 - (abs(casillasExploradas(explorado) - optimos[0]) / optimos[0]);
		if (_valores[0] < 0) _valores[0] = 0;

		_valores[1] = 1 - (abs(enemigo.golpes - optimos[1]) / optimos[1]);
		if (_valores[1] < 0) _valores[1] = 0;

		_valores[2] = 1 - (abs(enemigo.golpesEvitados - optimos[2]) / optimos[2]);
		if (_valores[2] < 0) _valores[2] = 0;

		_valores[3] = 1 - (abs(jugador.heridas - optimos[3]) / optimos[3]);
		if (_valores[3] < 0) _valores[3] = 0;
		*/

		_valores[0] = cExpl;
		_valores[1] = enemigo.golpes;
		_valores[2] = enemigo.golpesEvitados;
		_valores[3] = jugador.heridas;

		for (int i = 0; i < 4; ++i){
			evaluacion += _valores[i] * _pesos[i];
		}
		return evaluacion;
	}

	int casillasExploradas(Mapa m) {
		int ret = 0;
		for (std::size_t i = 0; i < m.getWidth(); ++i) {
			for (std::size_t j = 0; j < m.getHeight(); ++j) {
				if (m.getCasilla(i, j) > 0) {
					ret += m.getCasilla(i, j);
				}
			}
		}
		return ret;
	}

	void mueveJugador(npc &jugador, npc &enemigo, Mapa m) {
		jugador.bloqueando = false;
		int x, y;
		//Operacion op = (Operacion)myRandom::getRandom(Operacion::Avanza, Operacion::Retroceder);
		int intentos = 2;
		Operacion op;
		jugador.getCasillaDelante(x, y);
		if (enemigo.estaEn(x, y)) {
			int n = myRandom::getRandom(0, 3);
			if (n < 2) {
				op = Operacion::Atacar;
			}
			else if(n == 2){
				op = Operacion::BloquearN;
			}
			else{
				op = Operacion::Retroceder;	
				// Le damos la oportunidad de retirarse, sino, se queda siempre delante atacando o bloqueando
			}
		}
		else {
			do{
				op = (Operacion)myRandom::getRandom(Operacion::Avanza, Operacion::Retroceder);
				if (op != Operacion::Atacar) {
					intentos--;
				}
			} while (intentos > 0 && (op == Operacion::GiraDer || op == Operacion::GiraIz || op == Operacion::BloquearN || op == Operacion::Retroceder || op == Operacion::Atacar));
		}
		switch (op) {
		case Avanza:
			x = 0; y = 0;
			if (jugador.getCasillaDelante(x, y)) {
				if (!m.estaBloqueado(x, y)) {
					jugador.avanza();
				}
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
			x = 0; y = 0;
			jugador.getCasillaDelante(x, y);
			if (enemigo.estaEn(x, y)) {
				if (!enemigo.bloqueando) {
					enemigo.heridas++;
				}
				else {
					enemigo.golpesEvitados++;
				}
			}
			break;
		case Retroceder:
			x = 0; y = 0;
			if (jugador.getCasillaDetras(x, y)) {
				if (!m.estaBloqueado(x, y)) {
					jugador.retroceder();
				}
			}
			break;
		default:
			break;
		}
		enemigo.bloqueando = false;
	}
	/*
	npc setRandomPosition(Mapa &m) {
		int x, y;
		do {
			x = myRandom::getRandom(0, m.getWidth() - 1);
			y = myRandom::getRandom(0, m.getHeight() - 1);
		} while (m.getCasilla(x, y) != m.VACIO);
		npc enemigo(x, y, m.getHeight(), m.getWidth());
		return enemigo;
	}*/

	void setPositionInRange(npc jugador, npc &enemigo, int minRan, int maxRan, Mapa m){
		int xJug = jugador._posX;
		int yJug = jugador._posY;
		std::vector<std::pair<int, int>> posibles;
		for (int i = xJug - maxRan; i <= xJug + maxRan; ++i){
			for (int j = yJug - maxRan; j <= yJug + maxRan; ++j){
				if (i < xJug - minRan || i > xJug + minRan){
					if (j < yJug - minRan || j > yJug + minRan){
						if (m.coordValidas(i, j) && !m.estaBloqueado(i,j)){
							posibles.emplace_back(i, j);
						}
					}
				}
			}
		}
		std::random_shuffle(posibles.begin(), posibles.end());
		enemigo._posX = posibles.front().first;
		enemigo._posY = posibles.front().second;
		return;
	}

	

	Arbol _genotipo[2]; //la primera posición es el árbol de patrulla y la segunda el de ataque.
	double _punt;
	double _puntAcum;
	double _adaptacion;
	double _valores[4];
	double _pesos[4]; //casillasExploradas (maximizar), golpes evitados (maximizar), heridasBloqueadas (maximizar), daño (maximizar)
};

#endif