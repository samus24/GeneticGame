#ifndef MAPA_HPP
#define MAPA_HPP

#include <vector>

class Mapa {
public:

	static const int VACIO = -1;
	static const int COFRE = -2;
	static const int MURO = -3;
	static const int ENEMIGO = -4;
	static const int JUGADOR = -5;

	Mapa() {}

	Mapa(int width, int height, int origX, int origY) {
		this->_width = width;
		this->_height = height;
		this->_origX = origX;
		this->_origY = origY;
		for (std::size_t i = 0; i < _height; ++i) { //se rellena a un mapa vacio
			for (std::size_t j = 0; j < _width; ++j) {
				_mapa[i].push_back(VACIO);
			}
		}
	}

	int getWidth() {
		return _width;
	}

	void setWidth(int width) {
		this->_width = width;
	}

	int getHeight() {
		return _height;
	}

	void setHeight(int height) {
		this->_height = height;
	}

	int getX() {
		return _origX;
	}

	void setX(int x) {
		this->_origX = x;
	}

	int getY() {
		return _origY;
	}

	void setY(int y) {
		this->_origY = y;
	}

	void cargarMapa(std::vector<std::vector<int>> casillas) {
		_mapa = casillas;
	}

	std::vector<int> operator[](unsigned int i){
		return _mapa[i];
	}

private:
	int _width;
	int _height;
	int _origX;
	int _origY;
	std::vector<std::vector<int>> _mapa;
};


#endif