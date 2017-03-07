#ifndef NPC_HPP
#define NPC_HPP

enum facing {
	NORTE,
	SUR,
	ESTE,
	OESTE
};

class pnc {
public:
	int _posX;
	int _posY;
	facing f;

	pnc(int x, int y, int alto, int ancho) {
		_posX = x;
		_posY = y;
		_alto = alto;
		_ancho = ancho;
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