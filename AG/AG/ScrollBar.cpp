#include "ScrollBar.hpp"


namespace sf{
	ScrollBar::ScrollBar(sf::Vector2f position, sf::Vector2f size, sf::Color bgColor, sf::Color cursorColor, sf::Color textColor) :
		sf::Rect<float>(position, size),
		_pos(position),
		_size(size),
		_cursor(sf::Vector2f(std::min(size.x,size.y), std::min(size.x,size.y))),
		_box(size)
	{
		_bgColor = bgColor;
		_cursorColor = cursorColor;
		_textColor = textColor;
		_box.setPosition(position);
		_box.setFillColor(_bgColor);
		_box.setOutlineColor(sf::Color::Black);
		_box.setOutlineThickness(2);
		_cursor.setPosition(position);
		_cursor.setFillColor(_cursorColor);
		_selectedIndex = 0;
		_f.loadFromFile("arial.ttf");
	}


	ScrollBar::~ScrollBar()
	{
	}

	void ScrollBar::setElements(std::vector<std::string> e){
		_elements = e;
	}

	std::vector<std::string> ScrollBar::getElements() const{
		return _elements;
	}
	void ScrollBar::addElement(std::string s){
		_elements.push_back(s);
	}
	void ScrollBar::clearElements(){
		_elements.clear();
	}


	std::string ScrollBar::getSelectedElement()
	{
		return _elements[_selectedIndex];
	}


	int ScrollBar::getSelectedIndex()
	{
		return _selectedIndex;
	}


	int ScrollBar::getIndexFromPointer(sf::Vector2f pointer)
	{
		if (!this->contains(pointer)){
			return -1;
		}
		else{
			float percentage;
			if (std::max(_size.x, _size.y) == _size.x){
				// Horizontal scrollBar
				percentage = (pointer.x - _pos.x) / _size.x;
				
			}
			else{
				// Vertical scrollBar
				percentage = (pointer.y - _pos.y) / _size.y;
			}
			return  percentage * _elements.size();
		}
	}

	void ScrollBar::setSelectedIndex(int index){
		_selectedIndex = index;
		sf::Vector2f pos;
		if (_elements.size() > 0){
			if (std::max(_size.x, _size.y) == _size.x){
				// Horizontal scrollBar
				pos = sf::Vector2f(std::min(_pos.x + _size.x*((float)_selectedIndex / _elements.size()), _pos.x + _size.x - _cursor.getSize().x), _pos.y);

			}
			else{
				// Vertical scrollBar
				pos = sf::Vector2f(_pos.x, std::min(_pos.y + _size.y*((float)_selectedIndex / _elements.size()), _pos.y + _size.y - _cursor.getSize().y));
			}
			_cursor.setPosition(pos);
		}
		else{
			_cursor.setPosition(_pos);
		}
		
	}
}
