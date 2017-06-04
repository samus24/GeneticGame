#ifndef TEXTFIELD_HPP
#define TEXTFIELD_HPP

#include <SFML/Graphics.hpp>

#include "Button.hpp"

class TextField : public Button{
public:

	TextField(sf::Vector2f pos, sf::Vector2f size, std::string text = "") :
		Button(pos, size),
		_selected(false),
		_margin(1)
	{
		_rect.setPosition(pos);
		_rect.setSize(size);
		_rect.setFillColor(sf::Color::White);
		_rect.setOutlineThickness(1);
		_rect.setOutlineColor(sf::Color::Black);

		_charSize = size.x / 4.f;
		if (_charSize > 15) _charSize = 15;
		_font.loadFromFile("arial.ttf");
		_text.setFont(_font);
		_text.setCharacterSize(_charSize);
		_text.setFillColor(sf::Color::Black);
		_text.setPosition(pos);
		_text.move(_margin, _margin);
		_text.setString(text);
	}

	void setPosition(sf::Vector2f pos){
		Button::setPos(pos);
		_rect.setPosition(pos);
		_text.setPosition(pos);
		_text.move(_margin, _margin);
	}

	void setSize(sf::Vector2f size){
		Button::setSize(size);
		_rect.setSize(size);
		_charSize = size.x / 4.f;
		if (_charSize > 15) _charSize = 15;
		_text.setCharacterSize(_charSize);
	}

	void setText(std::string s){
		_text.setString(s);
	}

	std::string getText() const{
		return _text.getString();
	}

	void setSelected(bool sel){
		_selected = sel;
		if (_selected){
			_rect.setOutlineColor(sf::Color::Red);
		}
		else{
			_rect.setOutlineColor(sf::Color::Black);
		}
	}

	bool isSelected() const{
		return _selected;
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		target.draw(_rect, states);
		target.draw(_text, states);
	}


	sf::Font _font;
	sf::Text _text;
	sf::RectangleShape _rect;
	unsigned int _charSize;

	unsigned int _margin;
	bool _selected;
};

#endif