#ifndef NPC_HPP
#define NPC_HPP

enum facing {
	NORTE,
	SUR,
	ESTE,
	OESTE
};

class npc {
public:
	int _posX;
	int _posY;
	facing f;

	npc(int x, int y, int alto, int ancho) {
		this->_posX = x;
		this->_posY = y;
		this->_alto = alto;
		this->_ancho = ancho;
		this->f = facing::ESTE;
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
			aux = _posY - 1;
			if (aux >= 0) aux=0; //si te sales, no haces nada. Si es la casilla, habrá que codificarla de alguna forma
			break;
		case SUR:
			aux = _posY++;
			if (_posY <= this->_alto) aux = 0;
			break;
		case ESTE:
			aux = _posX++;
			if (_posX <= this->_ancho) aux = 0;
			break;
		case OESTE:
			aux = _posX--;
			if (_posX >= 0) aux = 0;
			break;
		default:
			break;
		}
	}

	void ataca() {
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

private:
	int _ancho;
	int _alto;
};

#endif