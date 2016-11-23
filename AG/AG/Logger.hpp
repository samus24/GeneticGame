#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <SFML\Graphics.hpp>

class Logger{
public:
	Logger(sf::Vector2f pos, sf::Vector2f size, ){
		_pos = pos;
		_size = size;
		_log = "";
	}

private:
	std::string _log;
	sf::Vector2f _pos;
	sf::Vector2f _size;
};

#endif
