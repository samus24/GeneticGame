#ifndef CHECKBUTTON_HPP
#define CHECKBUTTON_HPP
#include "SFML\Graphics.hpp">

#define INSIDE_MARGIN(s) 0.1*s

namespace sf{
	class CheckButton : public sf::Drawable
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
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		bool isPointInside(sf::Vector2i pointer);
		void toogleMarked();
		void setPosition(sf::Vector2f pos);
	};


}

#endif
