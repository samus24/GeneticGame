#ifndef MAPA_HPP
#define MAPA_HPP

#include <vector>

class Mapa {
public:

	Mapa(int width, int height, int origX, int origY) {
		this->_width = width;
		this->_height = height;
		this->_origX = origX;
		this->_origY = origY;
		this->_mapa[height][width];
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

	void cargarMapa(std::vector<int> casillas) {
		for (std::size_t i = 0; i < _height; ++i) {
			for (std::size_t j = 0; j < _width; ++j) {
				//rellenar el mapa, ver si pasarselo como un array
				//o mejor un array de arrays y directamente copiarlo.
			}
		}
	}

private:
	int _width;
	int _height;
	int _origX;
	int _origY;
	std::vector<std::vector<int>> _mapa;
};


#endif