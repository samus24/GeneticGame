#ifndef PROGRESSBAR_HPP
#define PROGRESSBAR_HPP

#include <SFML\Graphics.hpp>

class ProgressBar : public sf::Rect<float>, public sf::Drawable, public sf::Transformable{
public:
	ProgressBar(sf::Vector2f pos, sf::Vector2f size, double maxProgress = 100, int charSize = 15):
		sf::Rect<float>(pos, size)
	{
		_progress = 0;
		_maxProgress = maxProgress-1;
		_pos = pos;
		_size = size;
		unsigned int percentageSize = size.x * 0.25;
		_drawableRect.setPosition(pos.x, pos.y);
		_drawableRect.setSize(sf::Vector2f(size.x-percentageSize, size.y));
		_drawableRect.setFillColor(sf::Color::White);
		_drawableRect.setOutlineColor(sf::Color(127, 127, 127));
		_drawableRect.setOutlineThickness(2);
		_drawableProgress.setPosition(pos.x, pos.y);
		_drawableProgress.setSize(sf::Vector2f(0, size.y));
		_drawableProgress.setFillColor(sf::Color::Green);
		_drawableProgress.setOutlineThickness(0);
		_font.loadFromFile("arial.ttf");
		_text.setFont(_font);
		_text.setCharacterSize(charSize);
		_text.setFillColor(sf::Color::Black);
		_text.setPosition(pos.x + size.x - percentageSize + 2, pos.y);
		_text.setString("--/" + std::to_string((int)_maxProgress));
	}

	void setMaxProgress(double max){
		_maxProgress = max-1;
	}

	void clearProgress(){
		_progress = 0;
	}

	void updateProgress(double p){
		if (p <= _maxProgress){
			_progress = p;
			double maxWidth = _size.x * 0.75;
			_drawableProgress.setSize(sf::Vector2f((p / _maxProgress)*maxWidth, _size.y));
			_text.setString(std::to_string((int)_progress) + "/" + std::to_string((int)_maxProgress));
		}
	}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::RectangleShape r;
		r.setPosition(_pos.x, _pos.y);
		r.setSize(sf::Vector2f(_size.x*1.1, _size.y*1.1));
		r.setFillColor(sf::Color::White);

		target.draw(r);
		target.draw(_drawableRect);
		target.draw(_drawableProgress);
		target.draw(_text);
		
	}

	double _progress;
	double _maxProgress;
	sf::Font _font;
	sf::Text _text;
	sf::Vector2f _pos;
	sf::Vector2f _size;
	sf::RectangleShape _drawableRect;
	sf::RectangleShape _drawableProgress;
};
#endif