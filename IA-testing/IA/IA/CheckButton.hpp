#ifndef CHECKBUTTON_HPP
#define CHECKBUTTON_HPP
#include "SFML\Graphics.hpp">

#define INSIDE_MARGIN(s) 0.1*s

namespace sf{
	class CheckButton
	{
	public:
		CheckButton(sf::Vector2f position, unsigned int size);
		~CheckButton();
	private:
		unsigned int size;
		bool marked;
		sf::Vector2f position;
		sf::RectangleShape box;
		sf::RectangleShape insideBox;
	public:
		bool isMarked();
		void draw(sf::RenderTarget* target);
		bool isPointInside(sf::Vector2i pointer);
		void toogleMarked();
	};


}

#endif
