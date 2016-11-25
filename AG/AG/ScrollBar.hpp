#pragma once
#include <string>
#include <SFML\Graphics.hpp>

namespace sf{
	class ScrollBar : public sf::Rect<float>, public sf::Drawable, public sf::Transformable
	{
	public:
		ScrollBar(sf::Vector2f position, sf::Vector2f size, sf::Color bgColor = sf::Color(63, 63, 63), sf::Color cursorColor = sf::Color(127, 127, 127), sf::Color textColor = sf::Color::White);
		~ScrollBar();
	private:
		sf::Vector2f _size;
		std::vector<std::string> _elements;
		int _selectedIndex;
		sf::RectangleShape _box;
		sf::RectangleShape _cursor;
		sf::Font _f;
		sf::Color _bgColor;
		sf::Color _cursorColor;
		sf::Color _textColor;

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
				t.setPosition(pos);
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


