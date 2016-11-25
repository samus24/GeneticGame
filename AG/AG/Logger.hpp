#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <SFML\Graphics.hpp>
#include "ScrollBar.hpp"

class Logger : public sf::Rect<float>, public sf::Drawable, public sf::Transformable{
public:
	Logger(sf::Vector2f pos, sf::Vector2f size, std::string title = "Log", int charSize = 15) :
		_rect(pos, size)
	{
		unsigned int titleHeight = charSize * 1.3;
		_pos = pos;
		_size = size;
		_title = title;
		_drawableRect.setPosition(pos.x, pos.y + titleHeight);
		_drawableRect.setSize(sf::Vector2f(size.x, size.y - titleHeight));
		_drawableRect.setFillColor(sf::Color::White);
		_drawableRect.setOutlineColor(sf::Color(127, 127, 127));
		_drawableRect.setOutlineThickness(2);
		_margin = 2;
		_font.loadFromFile("arial.ttf");
		_charsPerLine = (size.x - 2 * _margin) / (charSize/2);
		_linesToShow = ((size.y - titleHeight) - 2 * _margin) / (charSize * 1.1);
		_charSize = charSize;
		clearLog();
	}

	void clearLog(){
		_log.clear();
		_actualCharsInLine = 0;
		_actualLines = 0;
	}

	void append(std::string s){
		size_t actualPos = 0;
		auto it = s.begin();
		while ((_actualLines < _linesToShow) && it != s.cend()){
			if (*it == '\n'){
				_actualCharsInLine = 0;
				_actualLines++;
			}
			else{
				_actualCharsInLine++;
			}
			if (_actualCharsInLine >= _charsPerLine){
				it = s.insert(it, '\n');
				_actualCharsInLine = 0;
				_actualLines++;
			}
			actualPos++;
			it++;
		}
		if (_actualLines < _linesToShow)
			_log.append(s);
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		unsigned int titleHeight = _charSize * 1.3;
		sf::Text drawableLog;
		drawableLog.setFont(_font);
		drawableLog.setFillColor(sf::Color::Black);
		drawableLog.setCharacterSize(_charSize);
		drawableLog.setPosition(_pos.x + _margin, _pos.y + _margin + titleHeight);
		drawableLog.setString(_log);

		sf::Text title;
		title.setFont(_font);
		title.setFillColor(sf::Color::Black);
		title.setCharacterSize(_charSize);
		title.setPosition(_pos.x, _pos.y);
		title.setString(_title);

		target.draw(title);
		target.draw(_drawableRect);
		target.draw(drawableLog);
		
	}

	std::string _log;
	std::string _title;
	sf::Font _font;
	unsigned int _margin;
	unsigned int _charSize;
	unsigned int _charsPerLine;
	unsigned int _actualCharsInLine;
	unsigned int _linesToShow;
	unsigned int _actualLines;
	sf::Vector2f _pos;
	sf::Vector2f _size;
	sf::Rect<float> _rect;
	sf::RectangleShape _drawableRect;
};

#endif
