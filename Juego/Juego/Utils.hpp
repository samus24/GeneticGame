#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const sf::Vector2f WINDOW_RES(WINDOW_WIDTH, WINDOW_HEIGHT);

const sf::Vector2u TILESIZE(16,16);

const sf::Vector2f BUTTONSIZE(280, 60);
const sf::IntRect  BUTTONSELECTED1RECT(15, 125, BUTTONSIZE.x, BUTTONSIZE.y);
const sf::IntRect  BUTTONNORMAL1RECT(15, 203, BUTTONSIZE.x, BUTTONSIZE.y);
const sf::IntRect  BUTTONSELECTED2RECT(15, 281, BUTTONSIZE.x, BUTTONSIZE.y);
const sf::IntRect  BUTTON2NORMALRECT(15, 359, BUTTONSIZE.x, BUTTONSIZE.y);


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