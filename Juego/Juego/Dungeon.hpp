#ifndef DUNGEON_HPP
#define DUNGEON_HPP

#include <memory>

class Dungeon{
public:
	Dungeon(){

	}

private:
	struct Matrix{
		size_t width;
		size_t height;
		std::unique_ptr<int> cells;

		Matrix(size_t width, size_t height){
			this->width = width;
			this->height = height;
			cells = std::make_unique<int>(width*height);
		}
	};

private:
	std::vector<Matrix> _rooms;
};

#endif