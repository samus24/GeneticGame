#ifndef VENTANA_HPP
#define VENTANA_HPP

#include <SFML\Graphics.hpp>
#include <thread>
#include <fstream>
#include <sstream>
#include <direct.h>
#include "Plotter.hpp"
#include "ProgressBar.hpp"
#include "Logger.hpp"
#include "Button.hpp"
#include "ScrollBar.hpp"
#include "Controlador.hpp"
#include "TabPane.hpp"
#include "SimulationViewer.hpp"
#include "VentanaParametros.hpp"

class Ventana : public IAGObserver, public ICromosomaObserver{
public:
	Ventana(Controlador& c) :
		_window(sf::VideoMode::getFullscreenModes()[6], "AG"),		// Comentar esta linea y
		//_window(sf::VideoMode(1200,600), "AG"),					// descomentar esta si no se representa bien en pantalla
		_windowTrees(sf::VideoMode(800, 800), "Arboles"),
		_windowParam(sf::VideoMode(500, 500), "Parametros"),
		_ventParam(sf::Vector2f(500, 500)),
		_tabPane(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, 25)),
		_plotter(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, _window.getSize().y)),
		_logger(sf::Vector2f(_window.getSize().x * 0.8, 75), sf::Vector2f(_window.getSize().x * 0.19, 400)),
		_botonRun(sf::Vector2f(_window.getSize().x * 0.8, 10), sf::Vector2f(_window.getSize().x * 0.07, 50), "RUN", sf::Color(100, 200, 200)),
		_botonSim(sf::Vector2f(_window.getSize().x * 0.88, 10), sf::Vector2f(_window.getSize().x * 0.07, 50), "VER SIM", sf::Color(120, 10, 10)),
		_botonSave(sf::Vector2f(_window.getSize().x * 0.8, 500), sf::Vector2f(_window.getSize().x * 0.07, 50), "SAVE", sf::Color(120, 10, 10)),
		_progress(sf::Vector2f(_window.getSize().x *0.8, _window.getSize().y *0.8), sf::Vector2f(_window.getSize().x * 0.1, 30)),
		_simViewer(sf::Vector2f(0, 0), sf::Vector2f(_window.getSize().x * 0.75, _window.getSize().y)),
		_visorPatrulla(sf::Vector2f(20, 20), sf::Vector2f(800-40, 400)),
		_visorAtaque(sf::Vector2f(20, 420), sf::Vector2f(800-40, 400)),
		_hiloArboles(&Ventana::ventanaArboles, this),
		_hiloAG(&Ventana::ejecutaAG, this),
		_hiloParam(&Ventana::ventanaParam, this)
	{
		save = false;
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

		tx.create(_windowTrees.getSize().x, _windowTrees.getSize().y);

		_windowParam.setActive(false);
		_hiloParam.launch();

		_nuevo = true;
		_windowTrees.setVisible(false);
		_windowTrees.setActive(false);

		running = false;
	}

	void run(unsigned int maxIter){
		_progress.setMaxProgress(maxIter);
		for (size_t i = 0; i < 10; ++i){
			_ejeX.push_back(i);
		}

		_plotter.setNombreEjeX("Generación");
		_plotter.setNombreEjeY("Adaptación");
		_plotter.setEjeX(_ejeX);

		_hiloArboles.launch();
		_window.setVerticalSyncEnabled(true);
		_window.setFramerateLimit(60);
		sf::Thread hiloMejor(&Ventana::evaluaMejor, this);

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
							_botonSave.setColor(sf::Color(120, 10, 10));
							_plotter.removeAllData();
							_ejeX.clear();
							_valorMedia.clear();
							_valorMejor.clear();
							_valorMejorGen.clear();
							_generacion = 0;
							_logger.clearLog();
							_logger.append("Ejecutando AG\n");
							_progress.clearProgress();
							Parametros p = _ventParam.getParametros();
							_ventParam.setLocked(true);
							_ctrl->setParametros(p);
							_hiloAG.launch();
							_nuevo = true;
						}
					}
					else if (_botonSave.contains(point)){
						if (_botonSave.getColor() != sf::Color(120, 10, 10)){
							save = true;
							_logger.append("Resultados guardados\n");
						}
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
						hiloMejor.launch();
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
						if (!running){
							_botonSave.setColor(sf::Color(120, 10, 10));
							_plotter.removeAllData();
							_ejeX.clear();
							_valorMedia.clear();
							_valorMejor.clear();
							_valorMejorGen.clear();
							_generacion = 0;
							_logger.clearLog();
							_logger.append("Ejecutando AG\n");
							_progress.clearProgress();
							Parametros p = _ventParam.getParametros();
							_ventParam.setLocked(true);
							_ctrl->setParametros(p);
							_hiloAG.launch();
							_nuevo = true;
						}
					}
				}
			}
			sf::Lock lock(_mutex);
			_window.clear(sf::Color::White);
			_window.draw(_logger);
			_window.draw(_botonRun);
			_window.draw(_botonSim);
			_window.draw(_botonSave);
			_window.draw(_progress);
			_window.draw(_tabPane);
			_window.draw(_labelAux);
			_window.display();
		}
	}

	void onGeneracionTerminada(double mejor, double mejorGen, double media){
		sf::Lock lock(_mutex);
		_ejeX.push_back(_generacion);
		_valorMejor.push_back(mejor);
		_valorMejorGen.push_back(mejorGen);
		_valorMedia.push_back(media);
		_progress.updateProgress(_generacion);
		_plotter.removeAllData();
		_plotter.setEjeX(_ejeX);
		_plotter.pushEjeY(_valorMedia, sf::Color::Green, "Media Poblacion");
		_plotter.pushEjeY(_valorMejorGen, sf::Color::Red, "Mejor Gen");
		_plotter.pushEjeY(_valorMejor, sf::Color::Blue, "Mejor");
		_generacion++;
	}

	void onAGTerminado(Cromosoma mejor, double total, double tmSel, double tmCruce, double tmMut, double tInit, double tmEval, poblacion pob){
		sf::Lock lock(_mutex);
		Arbol arbPatrulla = mejor.getGenotipo(0);
		Arbol arbAtaque = mejor.getGenotipo(1);
		_visorPatrulla.update(arbPatrulla, TipoArbol::Patrulla);
		_visorAtaque.update(arbAtaque, TipoArbol::Ataque);

		_pobFinal = pob;

		_mejor = mejor;
		_plotter.setEjeX(_ejeX);

		_plotter.pushEjeY(_valorMedia, sf::Color::Green, "Media Poblacion");
		_plotter.pushEjeY(_valorMejorGen, sf::Color::Red, "Mejor Gen");
		_plotter.pushEjeY(_valorMejor, sf::Color::Blue, "Mejor");
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

		_ventParam.setLocked(false);
	}

	void onSimulacionIniciada(const Cromosoma* c){
		sf::Lock lock(_mutex);
		//_nuevo = true;
		Arbol arbPatrulla = c->getGenotipo(0);
		Arbol arbAtaque = c->getGenotipo(1);
		_visorPatrulla.update(arbPatrulla, TipoArbol::Patrulla);
		_visorAtaque.update(arbAtaque, TipoArbol::Ataque);
	}

	void onTurno(const Cromosoma* c, npc jugador, npc enemigo, Mapa m, Mapa explorado, Mapa andado, Mapa andadoAtaque){
		
	}

	void onMapaTerminado(double fitness, double factorPatrulla, int cExpl, int cAndadas, int turnosQueValen, double factorAtaque, int cAndadasAtaque, int golpesEvitados, int golpes, int encontradoAtaque, int turnosAtaque, int intentos, double distancia, int turnosGolpeo) {

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

	void evaluaMejor(){
		if (finalizada){
			std::vector<ICromosomaObserver*> obs;
			obs.push_back(this);
			_mejor.evalua(_ctrl->getMapas(), false, obs);
		}
			
	}

	void ventanaArboles(){
		_windowTrees.setActive(true);
		_windowTrees.setFramerateLimit(30);
		_windowTrees.setVerticalSyncEnabled(true);
		_windowTrees.clear(sf::Color(127,127,127));
		while (_window.isOpen())
		{
			_windowTrees.setActive(!finalizada);
			_windowTrees.setVisible(finalizada);
			sf::Event event;
			while (_windowTrees.pollEvent(event));

			sf::Lock lock(_mutex);
			if (save){
				time_t t = time(0);   // get time now
				struct tm now;
				localtime_s(&now, &t);

				std::string date = "[" + std::to_string(now.tm_year + 1900) + "-" + std::to_string(now.tm_mon + 1) + "-" + std::to_string(now.tm_mday) + "][" + std::to_string(now.tm_hour) + "-" + std::to_string(now.tm_min) + "-" + std::to_string(now.tm_sec) + "]";
				std::string pathPlotter = "Resultados/" + date + "grafica.png";
				std::string pathArbol = "Resultados/" + date + "arbol.png";
				_windowTrees.clear(sf::Color::White);
				_windowTrees.draw(_plotter);
				tx.update(_windowTrees);
				sf::Image imPlotter = tx.copyToImage();
				imPlotter.saveToFile(pathPlotter);

				_windowTrees.clear(sf::Color::White);
				_windowTrees.draw(_visorPatrulla);
				_windowTrees.draw(_visorAtaque);
				tx.update(_windowTrees);
				sf::Image imArbol = tx.copyToImage();
				imArbol.saveToFile(pathArbol);

				sf::Image im;
				std::string path;
				for (size_t i = 0; i < _pobFinal._tam; ++i){
					_windowTrees.clear(sf::Color::White);
					_visorPatrulla.update(_pobFinal.individuos[i].getGenotipo(0), TipoArbol::Patrulla);
					_visorAtaque.update(_pobFinal.individuos[i].getGenotipo(1), TipoArbol::Ataque);
					_windowTrees.draw(_visorPatrulla);
					_windowTrees.draw(_visorAtaque);
					tx.update(_windowTrees);
					im = tx.copyToImage();
					path = "Resultados/Poblacion" + date;
					_mkdir(path.c_str());
					path += "/individuo-" + ((i < 10) ? "0" + std::to_string(i) : std::to_string(i)) + ".png";
					im.saveToFile(path);
				}

				_windowTrees.setActive(false);
				save = false;
			}
			_windowTrees.clear(sf::Color(127, 127, 127));
			_windowTrees.draw(_visorPatrulla);
			_windowTrees.draw(_visorAtaque);
			_windowTrees.display();
		}
	}

	void ventanaParam(){
		while (_window.isOpen())
		{
			sf::Event event;
			while (_windowParam.pollEvent(event)){
				if (event.type == sf::Event::MouseButtonPressed){
					sf::Vector2f point = sf::Vector2f(sf::Mouse::getPosition(_windowParam));
					_ventParam.handleClick(point);
				}
				else if (event.type == sf::Event::KeyReleased){
					_ventParam.handleKeyboardEvent(event);
				}
			}

			sf::Lock lock(_mutex);
			_windowParam.clear(sf::Color(200, 200, 200));
			_windowParam.draw(_ventParam);
			_windowParam.display();
		}
	}

	void ejecutaAG(){
		if (!running){
			running = true;
			_ctrl->run();
			running = false;
			_botonSave.setColor(sf::Color(100, 200, 200));
		}
	}

	sf::RenderWindow _window;
	sf::RenderWindow _windowTrees;
	sf::RenderWindow _windowParam;
	sf::Font _font;
	ProgressBar _progress;
	TabPane _tabPane;
	Plotter _plotter;
	Logger _logger;
	TextButton _botonRun;
	TextButton _botonSim;
	TextButton _botonSave;
	SimulationViewer _simViewer;
	Cromosoma _mejor;
	poblacion _pobFinal;
	Controlador* _ctrl;
	unsigned int _generacion;
	std::vector<double> _ejeX;
	std::vector<double> _valorMejor;
	std::vector<double> _valorMejorGen;
	std::vector<double> _valorMedia;
	VentanaParametros _ventParam;

	TreeViewer _visorPatrulla;
	TreeViewer _visorAtaque;

	sf::Texture tx;

	sf::Text _labelAux;

	sf::Thread _hiloParam;
	sf::Thread _hiloArboles;
	sf::Thread _hiloAG;

	sf::Mutex _mutex;
	bool _nuevo;
	bool running;

	bool finalizada;
	bool save;
};

#endif