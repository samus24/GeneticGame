#ifndef VENTANA_HPP
#define VENTANA_HPP

#include <SFML\Graphics.hpp>
#include <thread>
#include <fstream>
#include <sstream>
#include "Plotter.hpp"
#include "ProgressBar.hpp"
#include "Logger.hpp"
#include "Button.hpp"
#include "ScrollBar.hpp"
#include "Controlador.hpp"
#include "TabPane.hpp"
#include "SimulationViewer.hpp"

class Ventana : public IAGObserver, public ICromosomaObserver{
public:
	Ventana(Controlador& c) :
		_window(sf::VideoMode::getFullscreenModes()[6], "AG"),		// Comentar esta linea y
		//_window(sf::VideoMode(1200,600), "AG"),					// descomentar esta si no se representa bien en pantalla
		_tabPane(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, 25)),
		_plotter(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, _window.getSize().y)),
		_logger(sf::Vector2f(_window.getSize().x * 0.8, 75), sf::Vector2f(_window.getSize().x * 0.19, 400)),
		_botonRun(sf::Vector2f(_window.getSize().x * 0.8, 10), sf::Vector2f(100, 50), "RUN", sf::Color(100, 200, 200)),
		_botonSim(sf::Vector2f(_window.getSize().x * 0.8 + 110, 10), sf::Vector2f(100, 50), "VER SIM", sf::Color(120, 10, 10)),
		_progress(sf::Vector2f(_window.getSize().x *0.8, 550), sf::Vector2f(_window.getSize().x * 0.1, 30)),
		_simViewer(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, _window.getSize().y))
	{
		_font.loadFromFile("arial.ttf");
		_ctrl = &c;
		_generacion = 0;
		_ctrl->addObserver(*(this));
		_ctrl->addCromosomaObserver(_simViewer);
		_ctrl->addCromosomaObserver(*(this));
		_tabPane.addTab("Plotter", _plotter);
		_tabPane.addTab("SimViewer", _simViewer);
		finalizada = false;

		_labelAux.setFont(_font);
		_labelAux.setFillColor(sf::Color::Black);
		_labelAux.setPosition(sf::Vector2f(200, 0));
		_labelAux.setString("<-- Seleccionar la pestaña SimView para ver TODAS las evaluaciones (con boton VER SIM activado)\nPulsar E al finalizar para ver evaluacion del mejor");
		_labelAux.setCharacterSize(14);
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

						_generacion = 0;
						_logger.clearLog();
						_logger.append("Ejecutando\n");
						_progress.clearProgress();
						_window.draw(_progress);
						_window.draw(_logger);
						_window.display();
						_ctrl->run();
					}
					else if (_botonSim.contains(point)){
						finalizada = !finalizada;
						if (finalizada){
							_botonSim.setColor(sf::Color(10, 120, 10));
						}
						else{
							_botonSim.setColor(sf::Color(120, 10, 10));
						}						
					}
					else if (_tabPane.contains(point)){
						_tabPane.handleClick(point);
					}
					else if (_logger.contains(point)){
						_logger.handleClick(point);
					}
				}
				else if (event.type == sf::Event::MouseButtonReleased){
					
				}
				else if (event.type == sf::Event::KeyPressed){
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
						if (finalizada)
							_mejor.evalua(_ctrl->getMapas());
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
						_plotter.removeAllData();
						_ejeX.clear();
						_valorMedia.clear();
						_valorMejor.clear();
						_valorMejorGen.clear();
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
			_window.draw(_logger);
			_window.draw(_botonRun);
			_window.draw(_botonSim);
			_window.draw(_progress);
			_window.draw(_tabPane);
			_window.draw(_labelAux);
			_window.display();
		}
	}

	void onGeneracionTerminada(double mejor, double mejorGen, double media){
		_ejeX.push_back(_generacion);
		_valorMejor.push_back(mejor);
		_valorMejorGen.push_back(mejorGen);
		_valorMedia.push_back(media);
		_window.draw(_logger);
		_progress.updateProgress(_generacion);
		_window.draw(_progress);
		_window.display();
		_generacion++;
	}

	void onAGTerminado(Cromosoma mejor, double total, double tmSel, double tmCruce, double tmMut, double tInit, double tmEval, poblacion pob){
		_mejor = mejor;
		_plotter.setEjeX(_ejeX);

		_plotter.pushEjeY(_valorMedia, sf::Color::Green, "Media Poblacion");
		_plotter.pushEjeY(_valorMejorGen, sf::Color::Red, "Mejor Gen");
		_plotter.pushEjeY(_valorMejor, sf::Color::Blue, "Mejor");
		/*
		for (std::size_t i = 0; i < pob._tam; ++i) {
			writeToLog("Nuevo individuo: " );
			writeToLog("Árbol de patrulla");
			writeToLog(pob.individuos->getGenotipo(0).toString());
			writeToLog("Árbol de ataque");
			writeToLog(pob.individuos->getGenotipo(1).toString());
			writeToLog("Adaptación del individuo");
			writeToLog(pob.individuos->getAdaptacion());
			writeToLog("");
		}
		*/
		std::string valoresText[] = {
			"Exploradas: ",
			"Andadas: ",
			"Golpes intentados: ",
			"Golpes evitados: ",
			"Daño al jugador: "
		};
		_logger.append("-- Nota mejor --\n");
		const double* valores = mejor.getMediaValores();
		const double* pesos = mejor.getPesos();
		for (size_t i = 0; i < 4; ++i){
			_logger.append(valoresText[i] + std::to_string(valores[i]) + "/" + std::to_string(valores[i]*pesos[i]) + "\n");
		}
		
		finalizada = true;
	}

	void onTurno(const Cromosoma* c, npc jugador, npc enemigo, Mapa m, Mapa explorado, Mapa andado){
		if (finalizada){
			_window.draw(_tabPane);
			_window.display();
		}
	}

	void onSimulacionTerminada(const Cromosoma* c){
	}

	void writeToLog(const std::string &text) {
		std::ofstream log_file("log.txt", std::ios_base::out | std::ios_base::app);
		log_file << text << std::endl;
	}

	void writeToLog(const double &text) {
		std::ofstream log_file("log.txt", std::ios_base::out | std::ios_base::app);
		log_file << text << std::endl;
	}

private:

	sf::RenderWindow _window;
	sf::Font _font;
	ProgressBar _progress;
	TabPane _tabPane;
	Plotter _plotter;
	Logger _logger;
	TextButton _botonRun;
	TextButton _botonSim;
	SimulationViewer _simViewer;
	Cromosoma _mejor;
	Controlador* _ctrl;
	unsigned int _generacion;
	std::vector<double> _ejeX;
	std::vector<double> _valorMejor;
	std::vector<double> _valorMejorGen;
	std::vector<double> _valorMedia;

	sf::Text _labelAux;

	bool finalizada;
};

#endif