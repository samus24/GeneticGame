#ifndef COMBOBOX_HPP
#define COMBOBOX_HPP

#include "Button.hpp"

class ComboBox : public Button{
public:
	ComboBox(sf::Vector2f pos, sf::Vector2f size) :
		Button(pos, size),
		_triangle(size.y, 3),
		_selected(false),
		_selectedIndex(0),
		_margin(1)
	{
		_rect.setPosition(pos);
		_rect.setSize(size);
		_rect.setFillColor(sf::Color::White);

		_triangle.setPosition(pos);
		_triangle.move(size.x-size.y-_margin ,_margin);
		_triangle.setFillColor(sf::Color(127, 127, 127));
		_triangle.rotate(180);
		

		_charSize = size.x / 4.f;
		_font.loadFromFile("arial.ttf");
		_text.setFont(_font);
		_text.setCharacterSize(_charSize);
		_text.setFillColor(sf::Color::Black);
		_text.setPosition(pos);
		_text.move(_margin, _margin);
	}

	void setSelectedIndex(unsigned int i){
		_selectedIndex = i;
	}

	unsigned int getSelectedIndex() const{
		return _selectedIndex;
	}

	std::string getSelectedElemnt() const{
		return _elements[_selectedIndex];
	}
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		
		target.draw(_rect, states);
		target.draw(_text, states);
		target.draw(_triangle, states);

		if (_selected){

		}
	}

	std::vector<std::string> _elements;
	unsigned int _selectedIndex;

	sf::Font _font;
	sf::Text _text;
	sf::RectangleShape _rect;
	sf::CircleShape _triangle;
	unsigned int _charSize;

	unsigned int _margin;
	bool _selected;

};

#endif