#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <SFML\Graphics.hpp>
#include "ScrollBar.hpp"

class Logger : public sf::Rect<float>, public sf::Drawable, public sf::Transformable{
public:
	Logger(sf::Vector2f pos, sf::Vector2f size, std::string title = "Log", int charSize = 15) :
		sf::Rect<float>(pos, size),
		_scrollH(sf::Vector2f(pos.x, pos.y + size.y - _SCROLLBARTHICKNESS), sf::Vector2f(size.x, _SCROLLBARTHICKNESS)),
		_scrollV(sf::Vector2f(pos.x + size.x - _SCROLLBARTHICKNESS, pos.y), sf::Vector2f(_SCROLLBARTHICKNESS, size.y))
	{
		unsigned int titleHeight = charSize * 1.3;
		_scrollV = sf::ScrollBar(sf::Vector2f(pos.x + size.x - _SCROLLBARTHICKNESS, pos.y + titleHeight), sf::Vector2f(_SCROLLBARTHICKNESS, size.y - titleHeight));
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
		_showScrollBarH = false;
		_showScrollBarV = false;
		_scrollH.clearElements();
		_scrollV.clearElements();
		_indexOfShowingH = 0;
		_indexOfShowingV = 0;
	}

	void append(std::string s){
		_log.push_back(s);
		_scrollV.addElement("");
		if (s.size() > _charsPerLine){
			_showScrollBarH = true;
		}
		if (_log.size() > _linesToShow){
			_showScrollBarV = true;
			_indexOfShowingV = _log.size() - _linesToShow;
		}
	}

	void handleClick(sf::Vector2f point){
		int i;
		if (_showScrollBarV){
			i = _scrollV.getIndexFromPointer(point);
			if (i != -1){
				_scrollV.setSelectedIndex(i);
				_indexOfShowingV = (i * _log.size()) / 100;
			}
		}
		if (_showScrollBarH){
			i = _scrollH.getIndexFromPointer(point);
			if (i != -1){
				_scrollH.setSelectedIndex(i);
				//_indexOfShowingH = (i * _log.size()) / 100;
			}
		}
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		unsigned int titleHeight = _charSize * 1.3;
		std::string virtualLog = "";
		sf::Text drawableLog;
		drawableLog.setFont(_font);
		drawableLog.setFillColor(sf::Color::Black);
		drawableLog.setCharacterSize(_charSize);
		drawableLog.setPosition(_pos.x + _margin, _pos.y + _margin + titleHeight);
		for (size_t i = 0; i < _log.size(); ++i){
			if (i >= _indexOfShowingV && i < _indexOfShowingV + _linesToShow){
				virtualLog += _log[i].substr(0, _charsPerLine);
				if (virtualLog.back() != '\n') virtualLog += '\n';
			}
		}
		drawableLog.setString(virtualLog);

		sf::Text title;
		title.setFont(_font);
		title.setFillColor(sf::Color::Black);
		title.setCharacterSize(_charSize);
		title.setPosition(_pos.x, _pos.y);
		title.setString(_title);

		target.draw(title);
		target.draw(_drawableRect);
		target.draw(drawableLog);
		if (_showScrollBarV){
			target.draw(_scrollV);
		}
		
	}

	std::vector<std::string> _log;
	std::string _title;
	sf::Font _font;
	bool _showScrollBarH;
	bool _showScrollBarV;
	sf::ScrollBar _scrollH;
	sf::ScrollBar _scrollV;
	unsigned int _margin;
	unsigned int _charSize;
	unsigned int _charsPerLine;
	//unsigned int _actualCharsInLine;
	unsigned int _linesToShow;
	//unsigned int _actualLines;
	unsigned int _indexOfShowingV;
	unsigned int _indexOfShowingH;
	sf::Vector2f _pos;
	sf::Vector2f _size;
	//sf::Rect<float> _rect;
	sf::RectangleShape _drawableRect;

	const unsigned int _SCROLLBARTHICKNESS = 20;
};

#endif
