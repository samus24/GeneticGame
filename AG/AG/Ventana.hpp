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
#include "IObserverCruce.hpp"

class Ventana : public IAGObserver, public IObserverCruce{
public:
	Ventana(Controlador& c) :
		_window(sf::VideoMode::getFullscreenModes()[6], "AG"),		// Comentar esta linea y
		//_window(sf::VideoMode(1200,600), "AG"),					// descomentar esta si no se representa bien en pantalla
		_windowCruce(sf::VideoMode(1500,1000), "Cruce"),
		_tabPane(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, 25)),
		_plotter(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, _window.getSize().y)),
		_graphViewer(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, _window.getSize().y)),
		_roomViewer(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, _window.getSize().y)),
		_logger(sf::Vector2f(_window.getSize().x * 0.8, 75), sf::Vector2f(_window.getSize().x * 0.19, 400)),
		_botonRun(sf::Vector2f(_window.getSize().x * 0.8 , 10), sf::Vector2f(100, 50), "RUN", sf::Color(100,200,200)),
		_progress(sf::Vector2f(_window.getSize().x *0.8, _window.getSize().y -50), sf::Vector2f(_window.getSize().x * 0.1, 30)),
		_hiloAG(&Ventana::ejecutaAG, this),
		_hiloCruces(&Ventana::ventanaCruces, this),
		_botonCruces(sf::Vector2f(_window.getSize().x * 0.8 + 110, 10), sf::Vector2f(100, 50), "Ver Cruces", sf::Color(100, 200, 200)),
		_gwPadreA(sf::Vector2f(0, 0), sf::Vector2f(_windowCruce.getSize().x / 2, _windowCruce.getSize().y / 2)),
		_gwPadreB(sf::Vector2f(0, _windowCruce.getSize().y / 2), sf::Vector2f(_windowCruce.getSize().x / 2, _windowCruce.getSize().y / 2)),
		_gwHijoA(sf::Vector2f(_windowCruce.getSize().x / 2, 0), sf::Vector2f(_windowCruce.getSize().x / 2, _windowCruce.getSize().y / 2)),
		_gwHijoB(sf::Vector2f(_windowCruce.getSize().x / 2, _windowCruce.getSize().y / 2), sf::Vector2f(_windowCruce.getSize().x / 2, _windowCruce.getSize().y / 2))
	{
		_font.loadFromFile("arial.ttf");
		_ctrl = &c;
		_generacion = 0;
		_ctrl->addObserver(*(this));
		_ctrl->addObserverCruce(*(this));
		_tabPane.addTab("Plotter", _plotter);
		_tabPane.addTab("Graph", _graphViewer);
		_tabPane.addTab("Rooms", _roomViewer);

		running = false;

		_windowCruce.setActive(false);
		_windowCruce.setVisible(false);
		verCruces = false;
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
						if (!running){
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
							_hiloAG.launch();
							_hiloCruces.launch();
						}
					}
					else if (_botonCruces.contains(point)){
						verCruces = !verCruces;
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
						if (!running){
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
							_hiloAG.launch();
							_hiloCruces.launch();
						}
					}
				}
			}
			sf::Lock lock(_mutex);
			_window.clear(sf::Color::White);
			_window.draw(_tabPane);
			_window.draw(_logger);
			_window.draw(_botonRun);
			_window.draw(_botonCruces);
			_window.draw(_progress);
			_window.display();
		}
	}

	void onCruceIniciado(const Cromosoma& a, const Cromosoma& b, unsigned int corteA, unsigned int corteB){
		sf::Lock lock(_mutex);
		if (!verCruces) return;
		_gwPadreA.setModel(a);
		_gwPadreB.setModel(b);
	}

	void onCruceTerminado(const Cromosoma& a, const Cromosoma& b, unsigned int corteA, unsigned int corteB){
		sf::Lock lock(_mutex);
		if (!verCruces) return;
		_gwHijoA.setModel(a);
		_gwHijoB.setModel(b);
	}

	void onGeneracionTerminada(Cromosoma elMejor, double mejor, double mejorGen, double media, double mediaSel){
		sf::Lock lock(_mutex);
		_ejeX.push_back(_generacion);
		_graphViewer.setModel(elMejor);
		_valorMejor.push_back(mejor);
		_valorMejorGen.push_back(mejorGen);
		_valorMedia.push_back(media);
		_valorMediaSel.push_back(mediaSel);
		_progress.updateProgress(_generacion);


		_plotter.removeAllData();
		_plotter.setEjeX(_ejeX);

		_plotter.pushEjeY(_valorMediaSel, sf::Color::Magenta, "Media Seleccion");

		_plotter.pushEjeY(_valorMedia, sf::Color::Green, "Media Poblacion");
		_plotter.pushEjeY(_valorMejorGen, sf::Color::Red, "Mejor Gen");
		_plotter.pushEjeY(_valorMejor, sf::Color::Blue, "Mejor");

		_generacion++;
	}

	void onAGTerminado(Cromosoma mejor, double total, double tmSel, double tmCruce, double tmMut, double tInit, double tmEval){
		sf::Lock lock(_mutex);
		_plotter.removeAllData();
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
		std::vector<std::string> valoresText = mejor.getTagValores();
		_logger.append("-- Notas CC --\n");
		std::vector<double> valores = mejor.getValores();
		for (size_t i = 0; i < valores.size(); ++i){
			_logger.append(valoresText[i] + ": " + std::to_string(valores[i]) + "\n");
		}
	}


private:

	void ejecutaAG(){
		if (!running){
			running = true;
			_ctrl->run();
			running = false;
		}
	}

	void ventanaCruces(){
		_windowCruce.setActive(true);
		
		while (_window.isOpen() && running)
		{
			_windowCruce.setVisible(verCruces);
			// handle events
			sf::Event event;
			while (_windowCruce.pollEvent(event));

			sf::Lock lock(_mutex);
			if (verCruces){
				_windowCruce.clear(sf::Color::White);
				_windowCruce.draw(_gwPadreA);
				_windowCruce.draw(_gwPadreB);
				_windowCruce.draw(_gwHijoA);
				_windowCruce.draw(_gwHijoB);
				_windowCruce.display();
			}
		}
		_windowCruce.setActive(false);

	}

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

	sf::RenderWindow _windowCruce;
	GraphViewer _gwPadreA;
	GraphViewer _gwPadreB;
	GraphViewer _gwHijoA;
	GraphViewer _gwHijoB;

	bool running;
	sf::Thread _hiloAG;
	sf::Thread _hiloCruces;
	sf::Mutex _mutex;
	bool verCruces;
	TextButton _botonCruces;
};

#endif