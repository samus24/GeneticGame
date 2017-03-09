#ifndef TESTBENCH_HPP
#define TESTBENCH_HPP

#include <fstream>
#include <iomanip>

#include "IAGObserver.hpp"
#include "Parametros.hpp"
#include "ParametrosEval.hpp"
#include "AG.hpp"

class Testbench : public IAGObserver{
public:
	Testbench(Controlador& c){
		_ctrl = &c;
		_ctrl->addObserver(*(this));
		_pEval.nodosOptimos = 25;
		_pEval.gradoOptimo = 2;
		_pEval.anchoOptimo = 30;
		_pEval.altoOptimo = 20;
		_pEval.ciclosOptimos = 4;
		_pEval.enemigosOptimos = 35;
		_pEval.cofresOptimos = 10;
	}

	bool openFile(std::string path){
		_outfile.open(path);
		return _outfile.is_open();
	}

	void launchTestbench(){
		if (!_outfile.is_open())
			return;
		
		_ejec = 0;
		/// Estos parametros no van a variar
		_param.minNodos = 10;
		_param.maxNodos = 40;
		_param.densidad = 0.03;
		///

		_param.tamPob = 30;							
		_param.iteraciones = 30;								
		_param.elitismo = false;					
		_param.bloating = false;						
		_param.contractividad = false;				
		_param.probCruce = 0.6;						
		_param.probMutacion = 0.02;					
		_param.seleccion = new SeleccionRuleta();	
		_param.cruce = new CruceMonopunto();		
		_param.mutacion = new MutacionNodo();

		// Es necesario reducir las posibles combinaciones de parametros.
		// Originalmente habia planteado muchos mas casos, pero, estimando que cada
		// AG se ejecuta en 1.5 minutos (una estimacion baja, creo yo), el total de 
		// pruebas tardaria 14.7 años, claramente inviable.
		// He reducido las combinaciones (y las repeticiones) hasta alcanzar un tiempo
		// total de simulacion de aproximadamente 1 dia y medio (seguramente sean mas de 2 dias en realidad)

		std::vector<unsigned int> tamanos = {30, 50, 100, 150};			// Posibles tamanos de poblacion
		std::vector<unsigned int> generaciones = { 30, 50, 100, 200};	// Posibles numeros de generaciones
		std::vector<bool> elitismos = { false, true };
		std::vector<bool> bloatings = { false, true };
		std::vector<bool> contracts = { false, true };
		std::vector<double> probCruces = {0.6};
		std::vector<double> probMutac = {0.02};
		std::vector<MetodoSeleccion* > selecciones = {new SeleccionTorneo()};
		std::vector<MetodoCruce* > cruces = { new CruceMonopunto(), new CruceMultipunto() };
		std::vector<MetodoMutacion* > mutaciones = { new MutacionArista(), new MutacionNodo() };

		_repeticiones = 3;

		double combinaciones = tamanos.size() * generaciones.size() * 8 * probCruces.size() * probMutac.size() * 2 * 2;
		double minutos = combinaciones * _repeticiones * 1.5;
		double dias = (minutos / 60) / 24;

		std::cout << "Combinaciones : " << combinaciones << std::endl;
		std::cout << "Minutos : " << minutos << std::endl;
		std::cout << "Dias : " << dias << std::endl;

		for (auto i : tamanos){
			_param.tamPob = i;
			for (auto j : generaciones){
				_param.iteraciones = j;
				for (auto k : elitismos){
					_param.elitismo = k;
					for (auto l : bloatings){
						_param.bloating = l;
						for (auto m : contracts){
							_param.contractividad = m;
							for (auto n : probCruces){
								_param.probCruce = n;
								for (auto o : probMutac){
									_param.probMutacion = o;
									for (auto p : selecciones){
										_param.seleccion = p;
										for (auto q : cruces){
											_param.cruce = q;
											for (auto r : mutaciones){
												_param.mutacion = r;
												if ((_ejec / _repeticiones) >= 247){
													actualizaParametros();
													for (size_t s = 0; s < _repeticiones; ++s)
														lanzaAG();
												}
												else{
													_ejec += _repeticiones;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		

		_outfile.close();
		return;
	}

	void onGeneracionTerminada(double mejor, double mejorGen, double media, double mediaSel){
		_adapMejor.push_back(mejor);
		_adapMejorGen.push_back(mejorGen);
		_mediaPob.push_back(media);
		_mediaSel.push_back(mediaSel);
		++_iteracion;

		std::cout << "\r" << std::setprecision(3) << (_iteracion / (float)_param.iteraciones) * 100 << "%     ";
	}

	void onAGTerminado(Cromosoma mejor, double total, double tmSel, double tmCruce, double tmMut, double tInit, double tmEval){
		_outfile << "==== Resultados de ejecucion " <<_ejec / _repeticiones << "/" << _ejec % _repeticiones << " ====" << std::endl;
		_outfile << "== Adap mejor: " << mejor.getAdaptacion() << std::endl;
		_outfile << "== Nodos mejor CC: " << mejor.getMejorCC().size() << std::endl;
		_outfile << "== T. ejec.: " << std::to_string(total / 1000) << "s" << std::endl;
		_outfile << "== T. m. sel.: " << std::to_string(tmSel) << "ms" << std::endl;
		_outfile << "== T. m. cruce: " << std::to_string(tmCruce) << "ms" << std::endl;
		_outfile << "== T. m. mut.: " << std::to_string(tmMut) << "ms" << std::endl;
		_outfile << "== T. init: " << std::to_string(tInit) << "ms" << std::endl;
		_outfile << "== T. m. eval.: " << std::to_string(tmEval) << "ms" << std::endl;
		std::string valoresText[] = {
			"== Nodos: ",
			"== Media Grado: ",
			"== Media Alto: ",
			"== Media Ancho: ",
			"== Ciclos: ",
			"== Enemigos: ",
			"== Cofres: "
		};
		_outfile << "== -- Notas CC --" << std::endl;
		double* valores = mejor.getValores();
		double* pesos = mejor.getPesos();
		for (size_t i = 0; i < 7; ++i){
			_outfile << valoresText[i] << std::to_string(valores[i]) << " / " + std::to_string(valores[i] * pesos[i]) << std::endl;
		}
		_outfile << "== Datos de poblacion durante generaciones ==" << std::endl;

		_outfile << "== Adaptacion Mejor" << std::endl;
		_outfile << "\t[";
		for (size_t i = 0; i < _adapMejor.size(); ++i){
			if (i == _adapMejor.size() - 1){
				_outfile << _adapMejor[i] << "]" << std::endl;
			}
			else{
				_outfile << _adapMejor[i] << ", ";
			}
		}

		_outfile << "== Adaptacion Mejor Generacion" << std::endl;
		_outfile << "\t[";
		for (size_t i = 0; i < _adapMejorGen.size(); ++i){
			if (i == _adapMejor.size() - 1){
				_outfile << _adapMejorGen[i] << "]" << std::endl;
			}
			else{
				_outfile << _adapMejorGen[i] << ", ";
			}
		}

		_outfile << "== Media Poblacion" << std::endl;
		_outfile << "\t[";
		for (size_t i = 0; i < _mediaPob.size(); ++i){
			if (i == _mediaPob.size() - 1){
				_outfile << _mediaPob[i] << "]" << std::endl;
			}
			else{
				_outfile << _mediaPob[i] << ", ";
			}
		}

		_outfile << "== Media Seleccion" << std::endl;
		_outfile << "\t[";
		for (size_t i = 0; i < _mediaSel.size(); ++i){
			if (i == _mediaSel.size() - 1){
				_outfile << _mediaSel[i] << "]" << std::endl;
			}
			else{
				_outfile << _mediaSel[i] << ", ";
			}
		}

		++_ejec;

		std::cout << std::endl;
	}

private:
	void actualizaParametros(){
		_ctrl->updateParam(_param, _pEval);
		_outfile << "#### Nuevos Parametros #### " << _ejec / _repeticiones << std::endl;
		imprimeParam();
	}

	void lanzaAG(){
		_adapMejor.clear();
		_adapMejorGen.clear();
		_mediaPob.clear();
		_mediaSel.clear();
		_iteracion = 0;
		time_t t = time(0);   // get time now
		struct tm now;
		localtime_s(&now, &t);
		_outfile << "[" << now.tm_hour << ':' << now.tm_min << ':' << now.tm_sec << "] ";
		_outfile << "--< Ejecucion " << _ejec / _repeticiones << "/" << _ejec % _repeticiones << " >--" << std::endl;
		std::cout << "[" << now.tm_hour << ':' << now.tm_min << ':' << now.tm_sec << "] ";
		std::cout << "--< Ejecucion " << _ejec / _repeticiones << "/" << _ejec % _repeticiones << " >--" << std::endl;
		_ctrl->run();
	}

	void imprimeParam(){
		_outfile << "···· Parametros ····" << std::endl;
		_outfile << "·· Poblacion: " << _param.tamPob << std::endl;
		_outfile << "·· Iteraciones: " << _param.iteraciones << std::endl;
		_outfile << "·· Elitismo: " << _param.elitismo << std::endl;
		_outfile << "·· Bloating: " << _param.bloating << std::endl;
		_outfile << "·· Contractividad: " << _param.contractividad << std::endl;
		_outfile << "·· Prob. Cruce: " << _param.probCruce << std::endl;
		_outfile << "·· Prob. Mutacion: " << _param.probMutacion << std::endl;
		_outfile << "·· Metodo Seleccion: " << _param.seleccion->toString() << std::endl;
		_outfile << "·· Metodo Cruce: " << _param.cruce->toString() << std::endl;
		_outfile << "·· Metodo Mutacion: " << _param.mutacion->toString() << std::endl;
		_outfile << "····················" << std::endl;
	}

	Controlador* _ctrl;

	std::ofstream _outfile;
	ParametrosEval _pEval;
	Parametros _param;
	unsigned int _ejec = 0;
	unsigned int _iteracion = 0;
	unsigned int _repeticiones = 20;
	std::vector<double> _adapMejor;
	std::vector<double> _adapMejorGen;
	std::vector<double> _mediaPob;
	std::vector<double> _mediaSel;
	


};

#endif