#ifndef NPC_HPP
#define NPC_HPP

#include "myrandom.hpp"

enum facing {
	NORTE,
	SUR,
	ESTE,
	OESTE
};

class npc {
public:
	int turnos; //turnos totales de la simulacion.
	int posX;
	int posY;
	facing f;
	int golpes; //golpes que hemos dado al jugador
	int intentos; //golpes efectuados y fallados
	int heridas; //heridas que hemos recibido
	int golpesEvitados;
	int turnosPatrulla; //turnos que hemos invertido hasta cambiar al arbol de ataque
	int turnosGolpeo; //turnos que hemos invertido hasta el primer golpe
	bool bloqueando;
	int rango = 1;

	npc(int x, int y, int alto, int ancho) {
		this->posX = x;
		this->posY = y;
		this->_alto = alto;
		this->_ancho = ancho;
		this->f = (facing)myRandom::getRandom(facing::NORTE, facing::OESTE); //esto funciona?
		this->turnos = 0;
		this->golpes = 0;
		this->intentos = 0;
		this->heridas = 0;
		this->turnosPatrulla = -1;
		this->turnosGolpeo = -1;
		this->golpesEvitados = 0;
		this->bloqueando = false;
	}

	void avanza() {
		switch (f) {
		case NORTE:
			posY--;
			if (posY < 0) posY++; //si te sales, no haces nada
			break;
		case SUR:
			posY++;
			if (posY >= this->_alto) posY--;
			break;
		case ESTE:
			posX++;
			if (posX >= this->_ancho) posX--;
			break;
		case OESTE:
			posX--;
			if (posX < 0) posX++;
			break;
		default:
			break;
		}
	}

	void retroceder() {
		switch (f) {
		case SUR:
			posY--;
			if (posY < 0) posY++; //si te sales, no haces nada
			break;
		case NORTE:
			posY++;
			if (posY >= this->_alto) posY--;
			break;
		case OESTE:
			posX++;
			if (posX >= this->_ancho) posX--;
			break;
		case ESTE:
			posX--;
			if (posX < 0) posX++;
			break;
		default:
			break;
		}
	}

	void izquierda() {
		switch (f) {
		case NORTE:
			f = facing::OESTE;
			break;
		case SUR:
			f = facing::ESTE;
			break;
		case ESTE:
			f = facing::NORTE;
			break;
		case OESTE:
			f = facing::SUR;
			break;
		default:
			break;
		}
	}

	void derecha() {
		switch (f) {
		case NORTE:
			f = facing::ESTE;
			break;
		case SUR:
			f = facing::OESTE;
			break;
		case ESTE:
			f = facing::SUR;
			break;
		case OESTE:
			f = facing::NORTE;
			break;
		default:
			break;
		}
	}

	bool getCasillaDelante(int &x, int &y) {
		x = posX;
		y = posY;
		switch (f) {
		case NORTE:
			(y)--;
			if (y < 0) return false;
			break;
		case SUR:
			(y)++;
			if (y >= this->_alto) return false;
			break;
		case ESTE:
			(x)++;
			if (x >= this->_ancho) return false;
			break;
		case OESTE:
			(x)--;
			if (x < 0) return false;
			break;
		default:
			break;
		}
		return true;
	}

	bool getCasillaDetras(int &x, int &y) {
		x = posX;
		y = posY;
		switch (f) {
		case SUR:
			(y)--;
			if (y < 0) return false;
			break;
		case NORTE:
			(y)++;
			if (y >= this->_alto) return false;
			break;
		case OESTE:
			(x)++;
			if (x >= this->_ancho) return false;
			break;
		case ESTE:
			(x)--;
			if (x < 0) return false;
			break;
		default:
			break;
		}
		return true;
	}

	bool estaEn(int x, int y) {
		return   (posX == x && posY == y);
	}

private:
	int _ancho;
	int _alto;
};

#endif