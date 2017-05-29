#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>

const sf::Vector2f TILESIZE(16,16);

class TEXTURE_HOLDER{
	TEXTURE_HOLDER(){
		if (!t.loadFromFile("Textura.png")){
			exit(-1);
		}
	}



	sf::Texture t;
};

#endif