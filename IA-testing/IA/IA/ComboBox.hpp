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
		_markedIndex(0),
		_margin(1)
	{
		_rect.setPosition(pos);
		_rect.setSize(size);
		_rect.setFillColor(sf::Color::White);
		_rect.setOutlineThickness(0.5);
		_rect.setOutlineColor(sf::Color::Black);

		_triangle.setPosition(pos);
		_triangle.move(size.x-size.y-_margin ,_margin);
		_triangle.setFillColor(sf::Color(127, 127, 127));
		_triangle.rotate(180);

		_charSize = size.x / 4.f;
		if (_charSize > 15) _charSize = 15;
		_font.loadFromFile("arial.ttf");
		_text.setFont(_font);
		_text.setCharacterSize(_charSize);
		_text.setFillColor(sf::Color::Black);
		_text.setPosition(pos);
		_text.move(_margin, _margin);
	}

	void setPosition(sf::Vector2f pos){
		Button::setPos(pos);
		_rect.setPosition(pos);
		_text.setPosition(pos);
		_text.move(_margin, _margin);
		_triangle.setPosition(pos);
		sf::Vector2f size = Button::getSize();
		_triangle.move(size.x - size.y - _margin, _margin);
	}

	void setSize(sf::Vector2f size){
		Button::setSize(size);
		_rect.setSize(size);
		_charSize = size.x / 4.f;
		if (_charSize > 15) _charSize = 15;
		_text.setCharacterSize(_charSize);
		sf::Vector2f pos = Button::getPosition();
		_triangle.setPosition(pos);
		_triangle.move(size.x - size.y - _margin, _margin);
	}

	void setSelectedIndex(unsigned int i){
		_selectedIndex = i;
		if (_elements.size() > i){
			_text.setString(_elements[i]);
		}
	}

	unsigned int getSelectedIndex() const{
		return _selectedIndex;
	}

	std::string getSelectedElement(unsigned int i) const{
		if (_elements.size() > i)
			return _elements[_selectedIndex];
		else
			return "";
	}

	void setMarkedIndex(unsigned int i){
		_markedIndex = i;
	}

	unsigned int getMarkedIndex() const{
		return _markedIndex;
	}

	void setSelected(bool sel){
		_selected = sel;
	}

	bool isSelected() const{
		return _selected;
	}

	void addElement(std::string e){
		_elements.push_back(e);
	}

	void removeElement(unsigned int i){
		if (_elements.size() > i){
			_elements.erase(_elements.begin() + i);
		}
	}

	unsigned int getSize() const{
		return _elements.size();
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		
		target.draw(_rect, states);
		target.draw(_text, states);
		target.draw(_triangle, states);

		if (_selected){
			sf::RectangleShape rect;
			sf::Vector2f rectSize(_rect.getSize().x, _rect.getSize().y * _elements.size());
			rect.setSize(rectSize);
			rect.setPosition(_rect.getPosition());
			rect.move(0, _rect.getSize().y);
			rect.setOutlineThickness(0.5);
			rect.setOutlineColor(sf::Color::Black);
			rect.setFillColor(sf::Color::White);
			std::vector<sf::Text> texts;
			sf::Vector2f textPos = rect.getPosition();
			textPos.x += _margin;
			for (size_t i = 0; i < _elements.size(); ++i){
				textPos.y += _margin;
				sf::Text t;
				t.setFont(_font);
				t.setCharacterSize(_charSize);
				if(_markedIndex == i) t.setFillColor(sf::Color::Red);
				else				  t.setFillColor(sf::Color::Black);
				t.setString(_elements[i]);
				t.setPosition(textPos);
				textPos.y += _rect.getSize().y;
				texts.push_back(t);
			}

			target.draw(rect);
			for (sf::Text t : texts){
				target.draw(t, states);
			}
		}
	}

	std::vector<std::string> _elements;
	unsigned int _selectedIndex;
	unsigned int _markedIndex;

	sf::Font _font;
	sf::Text _text;
	sf::RectangleShape _rect;
	sf::CircleShape _triangle;
	unsigned int _charSize;

	unsigned int _margin;
	bool _selected;

};

#endif