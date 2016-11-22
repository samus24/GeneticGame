#ifndef VENTANA_HPP
#define VENTANA_HPP

#include <SFML\Graphics.hpp>
#include "Plotter.hpp"
#include "Button.hpp"
#include "ScrollBar.hpp"

class Ventana {
public:
	Ventana() :
		_window(sf::VideoMode::getFullscreenModes()[6], "AG"),
		_plotter(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, _window.getSize().y)),
		_botonRun(sf::Vector2f(_window.getSize().x * 0.8 , 0), sf::Vector2f(_window.getSize().x * 0.1, 50), "RUN")
	{
		_font.loadFromFile("arial.ttf");
	}

	void run(){
		

		while (_window.isOpen())
		{
			// handle events
			sf::Event event;
			while (_window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					_window.close();
				else if (event.type == sf::Event::MouseButtonPressed){

				}
				else if (event.type == sf::Event::MouseButtonReleased){

				}
			}
			_window.clear(sf::Color::White);
			_window.draw(_plotter);
			_window.draw(_botonRun);
			_window.display();
		}
	}
private:
	sf::RenderWindow _window;
	sf::Font _font;
	Plotter _plotter;
	TextButton _botonRun;
};

#endif