#ifndef SALA_HPP
#define SALA_HPP

#include <memory>

class Sala {
public:
	static const int VACIO = -1;
	static const int COFRE = -2;
	static const int MURO = -3;
	static const int ENEMIGO = -4;		// Potencialmente esto NO va aquí, pero es para pruebas

	// Podemos considerar los numeros negativos como elementos de las salas,
	// y comenzar a codificar las puertas con los numeros de sala con la que estan conectados

public:
	Sala(unsigned int id, unsigned int ancho, unsigned int alto){
		_id = id;
		_ancho = ancho;
		_alto = alto;
		_celdas.resize(ancho);
		for (size_t i = 0; i < _ancho; ++i){
			for (size_t j = 0; j < alto; ++j){
				_celdas[i].push_back(VACIO);
			}
		}
	}

	void setCeldas(std::vector<std::vector<int>> celdas){
		_celdas = celdas;
	}

	void setCelda(unsigned int x, unsigned int y, int valor){
		_celdas[x][y] = valor;
	}

	int getCelda(unsigned int i, unsigned int j){
		return _celdas[i][j];
	}

	std::vector<int> operator[](unsigned int i){
		return _celdas[i];
	}

private:
	unsigned int _id;
	unsigned int _ancho;
	unsigned int _alto;
	std::vector<std::vector<int>> _celdas;
};

#endif