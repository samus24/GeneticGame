#ifndef MAPA_HPP
#define MAPA_HPP

#include <vector>
#include <cmath>
#include <unordered_map>

class Mapa {
public:

	class Coord{
	public:
		unsigned int x;
		unsigned int y;

		Coord(){
			x = 0;
			y = 0;
		}

		Coord(unsigned int x, unsigned int y){
			this->x = x;
			this->y = y;
		}

		bool operator== (const Coord& other) const{
			return (this->x == other.x && this->y == other.y);
		}
		bool operator!= (const Coord& other) const{
			return !(*this == other);
		}

		double distancia(Coord dest){
			return std::sqrt((x - dest.x)*(x - dest.x) + (y - dest.y)*(y - dest.y));
		}
	};

	class CoordHash{
	public:
		size_t operator()(const Coord & key) const // <-- don't forget const
		{
			size_t hashVal = 0;
			hashVal += (key.x * 1000);
			hashVal += key.y;
			return hashVal;
		}
	};

	class Casilla{
	public:
		double dist;
		Coord pos;

		Casilla(double dist, Coord pos) :
			pos(pos)
		{
			this->dist = dist;
		}
	};

	class Compare{
	public:
		bool operator()(Casilla a, Casilla b) const{
			return a.dist > b.dist;
		}
	};

	static const int VACIO = -1;
	static const int COFRE = -2;
	static const int MURO = -3;

	Mapa() {}

	Mapa(int width, int height, int origX, int origY) {
		this->_width = width;
		this->_height = height;
		this->_origX = origX;
		this->_origY = origY;

		for (std::size_t i = 0; i < _height; ++i) { //se rellena a un mapa vacio
			_mapa.push_back(std::vector<int>());
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

	void setCasilla(int x, int y, int valor){
		_mapa[y][x] = valor;
	}

	int getCasilla(int x, int y) const{
		return _mapa[y][x];
	}

	bool estaBloqueado(int x, int y) const{
		return _mapa[y][x] != VACIO;
	}

	bool estaBloqueado(Coord c) const{
		return _mapa[c.y][c.x] != VACIO;
	}

	bool coordValidas(int x, int y) const{
		return (x >= 0 && x < _width && y >= 0 && y < _height);
	}

	bool coordValidas(Coord c) const{
		return (c.x >= 0 && c.x < _width && c.y >= 0 && c.y < _height);
	}

	std::vector<Coord> adyacentes(Coord c) const{
		std::vector<Coord> ady;
		Coord up(c.x, c.y-1);
		if(coordValidas(up) && !estaBloqueado(up)) ady.push_back(up);
		Coord down(c.x, c.y+1);
		if (coordValidas(down) && !estaBloqueado(down)) ady.push_back(down);
		Coord left(c.x - 1, c.y);
		if (coordValidas(left) && !estaBloqueado(left)) ady.push_back(left);
		Coord rigth(c.x + 1, c.y);
		if (coordValidas(rigth) && !estaBloqueado(rigth)) ady.push_back(rigth);

		return ady;
	}

	std::string toString() const{
		std::unordered_map<int, char> symbols = {
			{ VACIO, '-' },
			{ MURO, '#' },
			{ COFRE, '*' },

		};
		std::string ret = "";
		for (size_t j = 0; j < _height; ++j){
			for (size_t i = 0; i < _width; ++i){
				ret += symbols[_mapa[j][i]];
			}
			ret += '\n'; 
		}
		ret += '\n';
		return ret;
	}

	std::vector<Coord> astar(Coord orig, Coord dest) const{
		std::unordered_map<Coord, Coord, CoordHash> parents;
		std::priority_queue<Casilla, std::vector<Casilla>, Compare> queue;
		Casilla actual(0, orig);
		parents[actual.pos] = actual.pos;
		queue.push(actual);
		bool encontrado = false;
		while (!queue.empty() && !encontrado){
			actual = queue.top();
			queue.pop();
			if (actual.pos == dest){
				encontrado = true;
				break;
			}
			std::vector<Coord> ady = adyacentes(actual.pos);
			for (Coord c : ady){
				if (parents.find(c) == parents.end()){
					Casilla n(c.distancia(dest), c);
					queue.push(n);
					parents[c] = actual.pos;
				}
			}
		}
		std::vector<Coord> path;
		if (encontrado){
			Coord a = dest;
			path.push_back(a);
			while (a != orig){
				a = parents[a];
				path.push_back(a);
			}
			std::reverse(path.begin(), path.end());
		}
		return path;
	}
private:
	int _width;
	int _height;
	int _origX;
	int _origY;
	std::vector<std::vector<int>> _mapa;
};


#endif