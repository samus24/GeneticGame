#ifndef RANDOMGEN_HPP
#define RANDOMGEN_HPP

#include <random>

class RandomGen {
public:
	static int getRandom(int from, int to){
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(from, to);
		return dis(gen);
	}

	static unsigned int getRandom(unsigned int from, unsigned int to){
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(from, to);
		return dis(gen);
	}

	static double getRandom(double from, double to){
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(from, to);
		return dis(gen);
	}
};

#endif