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
	int _turnos; //turnos totales de la simulacion.
	int _posX;
	int _posY;
	facing f;
	int golpes; //golpes que hemos dado al jugador
	int heridas; //heridas que hemos recibido
	int turnosPatrulla; //turnos que hemos invertido hasta cambiar al arbol de ataque
	bool bloqueando;
	int rango = 1;

	npc(int x, int y, int alto, int ancho) {
		this->_posX = x;
		this->_posY = y;
		this->_alto = alto;
		this->_ancho = ancho;
		this->f = (facing)myRandom::getRandom(facing::NORTE, facing::OESTE); //esto funciona?
		this->_turnos = 0;
		this->golpes = 0;
		this->heridas = 0;
		this->turnosPatrulla = 0;
	}

	void avanza() {
		switch (f) {
		case NORTE:
			_posY--;
			if (_posY < 0) _posY++; //si te sales, no haces nada
			break;
		case SUR:
			_posY++;
			if (_posY > this->_alto) _posY--;
			break;
		case ESTE:
			_posX++;
			if (_posX > this->_ancho) _posX--;
			break;
		case OESTE:
			_posX--;
			if (_posX < 0) _posX++;
			break;
		default:
			break;
		}
	}

	void retroceder() {
		switch (f) {
		case SUR:
			_posY--;
			if (_posY < 0) _posY++; //si te sales, no haces nada
			break;
		case NORTE:
			_posY++;
			if (_posY > this->_alto) _posY--;
			break;
		case OESTE:
			_posX++;
			if (_posX > this->_ancho) _posX--;
			break;
		case ESTE:
			_posX--;
			if (_posX < 0) _posX++;
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
		int aux = 0;
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

	void ataca() {
	}

	bool getCasillaDelante(int &x, int &y) {
		x = _posX;
		y = _posY;
		switch (f) {
		case NORTE:
			y--;
			if (y < 0) return false;
			break;
		case SUR:
			y++;
			if (y > this->_alto) return false;
			break;
		case ESTE:
			x++;
			if (x > this->_ancho) return false;
			break;
		case OESTE:
			x--;
			if (x < 0) return false;
			break;
		default:
			break;
		}
		return true;
	}

	bool getCasillaDetras(int &x, int &y) {
		x = _posX;
		y = _posY;
		switch (f) {
		case SUR:
			y--;
			if (y < 0) return false;
			break;
		case NORTE:
			y++;
			if (y > this->_alto) return false;
			break;
		case OESTE:
			x++;
			if (x > this->_ancho) return false;
			break;
		case ESTE:
			x--;
			if (x < 0) return false;
			break;
		default:
			break;
		}
		return true;
	}

private:
	int _ancho;
	int _alto;
};

#endif