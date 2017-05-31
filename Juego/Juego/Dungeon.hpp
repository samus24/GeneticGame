#ifndef DUNGEON_HPP
#define DUNGEON_HPP

#include <memory>
#include "Cromosoma.hpp"
#include "myRandom.hpp"

class Dungeon{
public:
	class Matrix{
	public:
		size_t width;
		size_t height;
		int** cells;

		Matrix(size_t width, size_t height){
			this->width = width;
			this->height = height;
			cells = new int*[width];
			for (size_t i = 0; i < width; ++i){
				cells[i] = new int[height];
			}
			for (size_t i = 0; i < width; ++i){
				for (size_t j = 0; j < height; ++j){
					cells[i][j] = Dungeon::EMPTY;
				}
			}
		}
		int** getCells(){
			return cells;
		}

		void setCell(unsigned int x, unsigned int y, int value){
			if (x < width && y < height){
				cells[x][y] = value;
			}
		}

		int* operator[](unsigned int i) const{
			return cells[i];
		}
	};

public:
	Dungeon(){

	}

	void generateRooms(Cromosoma c){
		Grafo<Gen> g = c.getMejorCC();
		auto nodes = g.getNodos();
		auto adj = g.getAdyacencia();
		auto itNodes = nodes.begin();
		auto itAdj = adj.begin();
		RolSala rol = RolSala::RolCount;
		std::vector<unsigned int> rolesSala = c.getRolesSala();
		for (size_t i = 0; i < g.size(); ++i, itNodes++, itAdj++){
			rol = RolSala::RolCount;
			for (size_t j = 0; j < rolesSala.size(); ++j){
				if (rolesSala[j] == itNodes->first){
					rol = RolSala(j);
					break;
				}
			}
			_rooms.push_back(fillRoom(itNodes->second, itAdj->second, rol));
		}
		_selectedRoom = rolesSala[RolSala::Inicio];
	}
	
	void setSelectedRoom(size_t i){
		_selectedRoom = i;
	}

	Matrix getRoom(size_t i){
		return _rooms[i];
	}

	size_t getSelectedRoom() const{
		return _selectedRoom;
	}

private:
	Matrix fillRoom(Gen g, std::set<unsigned int> neighbours, RolSala rol){
		Matrix ret(g.getAncho(), g.getAlto());
		std::vector<std::pair<int, int>> portals = portalPlaces(g.getAncho(), g.getAlto(), neighbours.size());
		auto it = neighbours.begin();
		for (size_t i = 0; i < neighbours.size(); ++i, it++){
			ret.setCell(portals[i].first, portals[i].second, *it);
		}
		generateWalls(ret);
		return ret;
	}

	void generateWalls(Matrix &m){
		unsigned int nWalls = (m.height*m.width) / 200.f;
		unsigned int wall;
		unsigned int posInWall;
		unsigned int wallLength;
		double probContinue = 0.6;
		for (size_t i = 0; i < nWalls; ++i){
			probContinue = 0.2;
			int x, y, incX, incY;
			wall = myRandom::getRandom(0, 3);
			do{
				posInWall = myRandom::getRandom(1u, (wall % 2 == 0) ? m.width - 2 : m.height - 2);
				if (wall == 0){
					x = posInWall; y = 0; incX = 0; incY = 1;
				}
				else if (wall == 1){
					x = 0; y = posInWall; incX = 1; incY = 0;
				}
				else if (wall == 2){
					x = posInWall; y = m.height - 1; incX = 0; incY = -1;
				}
				else if (wall == 3){
					x = m.width - 1; y = posInWall; incX = -1; incY = 0;
				}
			} while (m[x][y] >= 0 || m[x][y] == Dungeon::WALL);
			wallLength = myRandom::getRandom(2, (wall % 2 == 0) ? int(m.height*0.8) : int(m.width*0.8));
			bool cont = false;
			do{
				cont = false;
				createWall(x, y, incX, incY, wallLength, m);
				if (probContinue > myRandom::getRandom(0.f, 1.f)){
					probContinue /= 3;
					wallLength = myRandom::getRandom(1, int(std::ceil(wallLength*0.8)));
					cont = true;
					std::swap(incX, incY);
					if (myRandom::getRandom(0u, 1u)){
						incX = -incX;
						incY = -incY;
					}

				}
			} while (cont);
		}
	}

	void createWall(int &x, int &y, int incX, int incY, unsigned int length, Matrix &m){
		for (size_t j = 0; j < length; ++j){
			if (j == 0 && wallsBeside(x, y, m) > 4){
				break;
			}
			if (j > 0 && wallsBeside(x, y, m) > 1){
				break;
			}
			m.setCell(x, y, Dungeon::WALL);
			x += incX;
			y += incY;
		}
	}

	int wallsBeside(int x, int y,const Matrix &m){
		int ret = 0;
		x -= 1;
		y -= 1;
		for (size_t i = 0; i < 3; ++i){
			for (size_t j = 0; j < 3; ++j){
				if (i != 1 && j != 1){
					if (x+i < 0 || x+i >= m.width || y+j < 0 || y+j >= m.height){
						ret++;
					}
					else if (m[x + i][y + j] == Dungeon::WALL || m[x + i][y + j] == Dungeon::CHEST){
						ret++;
					}
				}
			}
		}
		return ret;
	}

	static std::vector<std::pair<int, int>> portalPlaces(unsigned int width, unsigned int height, unsigned int unions){
		std::vector<std::pair<int, int>> ret;

		int portalsPerSide[4];	

		portalsPerSide[UP] = unions / 4;
		portalsPerSide[LEFT] = unions / 4;
		portalsPerSide[DOWN] = unions / 4;
		portalsPerSide[RIGHT] = unions / 4;

		int rest = unions % 4;
		while (rest > 0){
			portalsPerSide[myRandom::getRandom(0u, 3u)]++;
			rest--;
		}

		unsigned int step;
		if (portalsPerSide[UP] != 0)	step = std::ceil((double)width / (portalsPerSide[UP] + 1));
		for (size_t i = 0; i < portalsPerSide[UP]; ++i)
			ret.push_back(std::pair<int, int>((step*(i + 1)) % width, 0));
		if (portalsPerSide[DOWN] != 0)	step = std::ceil((double)width / (portalsPerSide[DOWN] + 1));
		for (size_t i = 0; i < portalsPerSide[DOWN]; ++i)
			ret.push_back(std::pair<int, int>((step*(i + 1)) % width, height - 1));
		if (portalsPerSide[LEFT] != 0)	step = std::ceil((double)height / (portalsPerSide[LEFT] + 1));
		for (size_t i = 0; i < portalsPerSide[LEFT]; ++i)
			ret.push_back(std::pair<int, int>(0, (step*(i + 1) % height)));
		if (portalsPerSide[RIGHT] != 0)	step = std::ceil((double)height / (portalsPerSide[RIGHT] + 1));
		for (size_t i = 0; i < portalsPerSide[RIGHT]; ++i)
			ret.push_back(std::pair<int, int>(width - 1, (step*(i + 1) % height)));

		return ret;
	}

private:
	static const int UP = 0;
	static const int LEFT = 1;
	static const int DOWN = 2;
	static const int RIGHT = 3;

	static const int EMPTY = -1;
	static const int CHEST = -2;
	static const int WALL = -3;
	static const int ENEMY = -4;

	std::vector<Matrix> _rooms;
	size_t _selectedRoom;
};

#endif