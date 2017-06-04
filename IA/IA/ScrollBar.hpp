#pragma once

#define NOMINMAX

#include <string>
#include <algorithm>
#include <SFML\Graphics.hpp>

namespace sf{

	/**
		La clase ScrollBar se utiliza tanto como metodo de entrada de datos, como una scrollbar de posicion.
		Como otros elementos de la GUI, debe ser dibujable y transformable. Ademas, es tambien un rectangulo, lo que nos permite detectar
		pulsaciones en ella.
		Se debe especificar posicion y tamano. Opcionalmente se puede elegir el color de fondo, del cursor y del texto que represente.
		En caso de querer una ScrollBar de posicion (como en la clase Logger), simplemente se pueden añadir como elementos de la scrollbar
		cadenas vacias, que no seran representadas en pantalla, pero permiten realizar los calculos de posicion de un click dentro de la
		scrollbar.
	*/
	class ScrollBar : public sf::Rect<float>, public sf::Drawable, public sf::Transformable
	{
	public:
		ScrollBar(sf::Vector2f position, sf::Vector2f size, sf::Color bgColor = sf::Color(63, 63, 63), sf::Color cursorColor = sf::Color(127, 127, 127), sf::Color textColor = sf::Color::White);
		~ScrollBar();
	private:
		sf::Vector2f _pos;					// Posicion de la scrollbar (tambien en sf::Rect)
		sf::Vector2f _size;					// Tamano de la scrollbar (tambien en sf::Rect)
		std::vector<std::string> _elements;	// Conjunto de strings a mostrar (las opciones elegibles de la scrollbar)
		unsigned int _selectedIndex;		// Indice dentro de _elements del elemento seleccionado (mostrado)
		sf::RectangleShape _box;			// Caja (fondo) de la scrollbar
		sf::RectangleShape _cursor;			// Elemento movil de la scrollbar que indica la posicion del elemento seleccionado
		sf::Font _f;						// Fuente para los caracteres
		sf::Color _bgColor;					// Color de la caja (fondo)
		sf::Color _cursorColor;				// Color del cursor
		sf::Color _textColor;				// Color del texto

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
			if (_elements.size() > 0){
				sf::Text t(_elements[_selectedIndex], _f);
				sf::Vector2f pos = _cursor.getPosition();
				if (std::max(_size.x, _size.y) == _size.x){
					// Horizontal scrollBar
					pos.y += _cursor.getSize().y;
				}
				else{
					// Vertical scrollBar
					pos.x += _cursor.getSize().x;
				}
				t.setPosition(pos);		// La posicion del texto varia si esta horizontal o vertical
				t.setCharacterSize(15);
				target.draw(t);
			}
			target.draw(_box, states);
			target.draw(_cursor, states);
		}

	public:
		void setElements(std::vector<std::string> e);
		std::vector<std::string> getElements() const;
		void addElement(std::string s);
		void clearElements();
		std::string getSelectedElement();
		int getSelectedIndex();
		int getIndexFromPointer(sf::Vector2f pointer);
		void setSelectedIndex(int index);
	};
}


