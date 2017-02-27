#ifndef VENTANA_HPP
#define VENTANA_HPP

#include <SFML\Graphics.hpp>
#include <thread>
#include "Plotter.hpp"
#include "ProgressBar.hpp"
#include "Logger.hpp"
#include "Button.hpp"
#include "ScrollBar.hpp"
#include "Controlador.hpp"
#include "TabPane.hpp"
#include "GraphViewer.hpp"
#include "RoomViewer.hpp"
#include "Rellenador.hpp"

class Ventana : public IAGObserver{
public:
	Ventana(Controlador& c) :
		_window(sf::VideoMode::getFullscreenModes()[6], "AG"),		// Comentar esta linea y
		//_window(sf::VideoMode(1200,600), "AG"),					// descomentar esta si no se representa bien en pantalla
		_tabPane(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, 25)),
		_plotter(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, _window.getSize().y)),
		_graphViewer(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, _window.getSize().y)),
		_roomViewer(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, _window.getSize().y)),
		_logger(sf::Vector2f(_window.getSize().x * 0.8, 75), sf::Vector2f(_window.getSize().x * 0.19, 400)),
		_botonRun(sf::Vector2f(_window.getSize().x * 0.8 , 10), sf::Vector2f(_window.getSize().x * 0.1, 50), "RUN", sf::Color(100,200,200)),
		_progress(sf::Vector2f(_window.getSize().x *0.8, _window.getSize().y * 0.75), sf::Vector2f(_window.getSize().x * 0.1, 30))
	{
		_font.loadFromFile("arial.ttf");
		_ctrl = &c;
		_generacion = 0;
		_ctrl->addObserver(*(this));
		_tabPane.addTab("Plotter", _plotter);
		_tabPane.addTab("Graph", _graphViewer);
		_tabPane.addTab("Rooms", _roomViewer);
	}

	void run(unsigned int maxIter){
		_progress.setMaxProgress(maxIter);
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

						_valorMediaSel.clear();

						_generacion = 0;
						_logger.clearLog();
						_logger.append("Ejecutando AG\n");
						_progress.clearProgress();
						_window.draw(_progress);
						_window.draw(_logger);
						_window.display();
						_ctrl->run();
					}
					else if (_tabPane.contains(point)){
						_tabPane.handleClick(point);
					}
					else if (_logger.contains(point)){
						_logger.handleClick(point);
					}
					else if (_roomViewer.contains(point)){
						_roomViewer.handleClick(point);
					}
				}
				else if (event.type == sf::Event::MouseButtonReleased){

				}
				else if (event.type == sf::Event::KeyPressed){
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
						_graphViewer.setModel(_graphViewer.getModel());
						_roomViewer.setModel(_roomViewer.getModel(), Rellenador::rellenaMazmorra(_roomViewer.getModel().getMejorCC()));
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
						_plotter.removeAllData();
						_ejeX.clear();
						_valorMedia.clear();
						_valorMejor.clear();
						_valorMejorGen.clear();

						_valorMediaSel.clear();

						_generacion = 0;
						_logger.clearLog();
						_logger.append("Ejecutando AG\n");
						_progress.clearProgress();
						_window.draw(_progress);
						_window.draw(_logger);
						_window.display();
						_ctrl->run();
					}
				}
			}
			_window.clear(sf::Color::White);
			_window.draw(_tabPane);
			_window.draw(_logger);
			_window.draw(_botonRun);
			_window.draw(_progress);
			_window.display();
		}
	}

	void onGeneracionTerminada(double mejor, double mejorGen, double media, double mediaSel){
		_ejeX.push_back(_generacion);
		_valorMejor.push_back(mejor);
		_valorMejorGen.push_back(mejorGen);
		_valorMedia.push_back(media);
		_valorMediaSel.push_back(mediaSel);
		_window.draw(_logger);
		_progress.updateProgress(_generacion);
		_window.draw(_progress);
		_window.display();
		_generacion++;
	}

	void onAGTerminado(Cromosoma mejor, double total, double tmSel, double tmCruce, double tmMut, double tInit, double tmEval){
		_plotter.setEjeX(_ejeX);

		_plotter.pushEjeY(_valorMediaSel, sf::Color::Magenta, "Media Seleccion");

		_plotter.pushEjeY(_valorMedia, sf::Color::Green, "Media Poblacion");
		_plotter.pushEjeY(_valorMejorGen, sf::Color::Red, "Mejor Gen");
		_plotter.pushEjeY(_valorMejor, sf::Color::Blue, "Mejor");
		
		_graphViewer.setModel(mejor);
		_roomViewer.setModel(mejor, Rellenador::rellenaMazmorra(mejor.getMejorCC()));
		
		_logger.append("Valor mejor: " + std::to_string(mejor.getAdaptacion()) + "\n");
		_logger.append("Nodos mejor CC: " + std::to_string(mejor.getMejorCC().size()) + "\n");
		_logger.append("T. ejec.: " + std::to_string(total / 1000) + "s\n");
		_logger.append("T. m. sel.: " + std::to_string(tmSel) + "ms\n");
		_logger.append("T. m. cruce: " + std::to_string(tmCruce) + "ms\n");
		_logger.append("T. m. mut.: " + std::to_string(tmMut) + "ms\n");
		_logger.append("T. init: " + std::to_string(tInit) + "ms\n");
		_logger.append("T. m. eval.: " + std::to_string(tmEval) + "ms\n");
		std::string valoresText[] = {
			"Nodos: ",
			"Media Grado: ",
			"Media Alto: ",
			"Media Ancho: ",
			"Ciclos: ",
			"Enemigos: ",
			"Cofres: "
		};
		_logger.append("-- Notas CC --\n");
		double* valores = mejor.getValores();
		for (size_t i = 0; i < 7; ++i){
			_logger.append(valoresText[i] + std::to_string(valores[i]) + "\n");
		}
		

	}
private:
	sf::RenderWindow _window;
	sf::Font _font;
	ProgressBar _progress;
	TabPane _tabPane;
	Plotter _plotter;
	GraphViewer _graphViewer;
	RoomViewer _roomViewer;
	Logger _logger;
	TextButton _botonRun;
	Controlador* _ctrl;
	unsigned int _generacion;
	std::vector<double> _ejeX;
	std::vector<double> _valorMejor;
	std::vector<double> _valorMejorGen;
	std::vector<double> _valorMedia;
	std::vector<double> _valorMediaSel;
};

#endif