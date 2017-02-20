#ifndef SALA_HPP
#define SALA_HPP

#include <memory>

class Sala {
public:
	static const int VACIO = -1;
	static const int COFRE = -2;
	static const int MURO = -3;

	// Podemos considerar los numeros negativos como elementos de las salas,
	// y comenzar a codificar las puertas con los numeros de sala con la que estan conectados

public:
	Sala(unsigned int id, unsigned int ancho, unsigned int alto){
		_id = id;
		_ancho = ancho;
		_alto = alto;
		for (size_t i = 0; i < _ancho; ++i){
			for (size_t j = 0; j < alto; ++j){
				_celdas[i][j] = VACIO;
			}
		}
	}

	void setCeldas(std::vector<std::vector<int>> celdas){
		_celdas = celdas;
	}

	void setCelda(unsigned int x, unsigned int y, int valor){
		_celdas[x][y] = valor;
	}

private:
	unsigned int _id;
	unsigned int _ancho;
	unsigned int _alto;
	std::vector<std::vector<int>> _celdas;
	std::unordered_map<int, int> _puertas;	// Este array indica para una puerta (id puerta), a qué sala (id sala) conduce

	

	



};

#endif