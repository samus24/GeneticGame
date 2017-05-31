#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const sf::Vector2f WINDOW_RES(WINDOW_WIDTH, WINDOW_HEIGHT);

const sf::Vector2f TILESIZE(16,16);

class TEXTURE_HOLDER{
	TEXTURE_HOLDER(){
		if (!t.loadFromFile("Textura.png")){
			exit(-1);
		}
	}



	sf::Texture t;
};

void centerOrigin(sf::Sprite& sprite);

void centerOrigin(sf::Text& text);

#endif