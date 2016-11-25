#ifndef VENTANA_HPP
#define VENTANA_HPP

#include <SFML\Graphics.hpp>
#include "Plotter.hpp"
#include "Logger.hpp"
#include "Button.hpp"
#include "ScrollBar.hpp"
#include "Controlador.hpp"
#include "TabPane.hpp"

class Ventana : public IAGObserver{
public:
	Ventana(Controlador& c) :
		//_window(sf::VideoMode::getFullscreenModes()[6], "AG"),
		_window(sf::VideoMode(1000,600), "AG"),
		_tabPane(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, _window.getSize().y)),
		_plotter(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, _window.getSize().y)),
		_logger(sf::Vector2f(_window.getSize().x * 0.8, 75), sf::Vector2f(_window.getSize().x * 0.15, 400)),
		_botonRun(sf::Vector2f(_window.getSize().x * 0.8 , 10), sf::Vector2f(_window.getSize().x * 0.1, 50), "RUN", sf::Color(100,200,200))
	{
		_font.loadFromFile("arial.ttf");
		_ctrl = &c;
		_generacion = 0;
		_ctrl->addObserver(*(this));
		_tabPane.addTab("Plotter", _plotter);
	}

	void run(){
		srand(time(NULL));
		for (size_t i = 0; i < 10; ++i){
			_ejeX.push_back(i);
		}

		_plotter.setNombreEjeX("Generación");
		_plotter.setNombreEjeY("Adaptación");
		_plotter.setEjeX(_ejeX);

		while (_window.isOpen())
		{
			// handle events
			sf::Event event;
			while (_window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					_window.close();
				else if (event.type == sf::Event::MouseButtonPressed){
					sf::Vector2f point = sf::Vector2f(sf::Mouse::getPosition(_window));
					if (_botonRun.contains(point)){
						_plotter.removeAllData();
						_ejeX.clear();
						_valorMedia.clear();
						_valorMejor.clear();
						_valorMejorGen.clear();
						_generacion = 0;
						_ctrl->run();
					}
					else if (_tabPane.contains(point)){
						_tabPane.handleClick(point);
					}
				}
				else if (event.type == sf::Event::MouseButtonReleased){

				}
			}
			_window.clear(sf::Color::White);
			_window.draw(_tabPane);
			_window.draw(_logger);
			_window.draw(_botonRun);
			_window.display();
		}
	}

	void onGeneracionTerminada(double mejor, double mejorGen, double media){
		_ejeX.push_back(_generacion);
		_valorMejor.push_back(mejor);
		_valorMejorGen.push_back(mejorGen);
		_valorMedia.push_back(media);
		_generacion++;
	}

	void onAGTerminado(){
		_plotter.setEjeX(_ejeX);
		_plotter.pushEjeY(_valorMejor, sf::Color::Blue, "Mejor");
		_plotter.pushEjeY(_valorMejorGen, sf::Color::Red, "Mejor Gen");
		_plotter.pushEjeY(_valorMedia, sf::Color::Green, "Media");
		
		_logger.clearLog();
		_logger.append("Valor mejor: " + std::to_string(_valorMejor.back()) + "\n");
	}
private:
	

	sf::RenderWindow _window;
	sf::Font _font;
	TabPane _tabPane;
	Plotter _plotter;
	Logger _logger;
	TextButton _botonRun;
	Controlador* _ctrl;
	unsigned int _generacion;
	std::vector<double> _ejeX;
	std::vector<double> _valorMejor;
	std::vector<double> _valorMejorGen;
	std::vector<double> _valorMedia;
};

#endif