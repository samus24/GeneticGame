#ifndef TABPANE_HPP
#define TABPANE_HPP

#include <SFML\Graphics.hpp>
#include "Button.hpp"

class Ventana;

class TabPane : public sf::Rect<float>, public sf::Drawable, public sf::Transformable{
public:
	TabPane(sf::Vector2f pos, sf::Vector2f size) :
		sf::Rect<float>(pos, size)
	{
		_font.loadFromFile("arial.ttf");
		_xForNextButton = pos.x;
		_tabToShow = 0;
	}

	void addTab(std::string tabName, sf::Drawable &item){
		sf::Color c = _BUTTONCOLORUNSELECTED;
		if (_tabButtons.size() == 0) c = _BUTTONCOLORSELECTED;
		TextButton b(sf::Vector2f(_xForNextButton,this->getPosition().y), _BUTTONSIZE, tabName, c, _CHARSIZE);
		_xForNextButton += _BUTTONSIZE.x;
		_tabButtons.push_back(b);
		_tabs.push_back(&item);
	}

	bool removeTab(std::string tabName){
		for (size_t i = 0; i < _tabButtons.size(); ++i){
			if (_tabButtons[i].getString() == tabName){
				_tabButtons.erase(_tabButtons.begin() + i);
				_tabs.erase(_tabs.begin() + i);
				return true;
			}
		}
		return false;
	}

	bool removeTab(unsigned int index){
		if (index >= _tabButtons.size()){
			return false;
		}
		_tabButtons.erase(_tabButtons.begin() + index);
		_tabs.erase(_tabs.begin() + index);
		return true;
	}

	void handleClick(sf::Vector2f point){
		if (_tabButtons.size() > 0){
			for (size_t i = 0; i < _tabButtons.size(); ++i){
				if (_tabButtons[i].contains(point)){
					_tabButtons[_tabToShow].setColor(_BUTTONCOLORUNSELECTED);
					_tabToShow = i;
					_tabButtons[i].setColor(_BUTTONCOLORSELECTED);
				}
			}
		}
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (_tabButtons.size() > 0){
			target.draw(*(_tabs[_tabToShow]), states);
			for (size_t i = 0; i < _tabButtons.size(); ++i){
				target.draw(_tabButtons[i], states);
			}
			target.draw(_tabButtons[_tabToShow], states);	// Redraw selected button to be in front
<<<<<<< HEAD:IA-testing/IA/IA/TabPane.hpp
=======
			target.draw(*(_tabs[_tabToShow]), states);
>>>>>>> refs/remotes/origin/master:AG/AG/TabPane.hpp
		}
	}

	sf::Font _font;
	unsigned int _xForNextButton;	// Horizontal position (x) for the next button
	unsigned int _tabToShow;
	std::vector<TextButton> _tabButtons;
	std::vector<sf::Drawable*> _tabs;

	const sf::Vector2f _BUTTONSIZE = sf::Vector2f(75, 25);
	const sf::Color _BUTTONCOLORSELECTED = sf::Color(100,200,200);
	const sf::Color _BUTTONCOLORUNSELECTED = sf::Color(200, 200, 100);
	const unsigned int _CHARSIZE = 15;
};

#endif