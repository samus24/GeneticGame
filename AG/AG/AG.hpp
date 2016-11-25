#ifndef AG_HPP
#define AG_HPP

#include "IAGObserver.hpp"
#include "Poblacion.hpp"
#include "Cromosoma.hpp"
#include "Parametros.hpp"
#include "Cronometro.hpp"

class AG {
public:
	AG(Parametros p){
		_param = p;
		_crono.creaMedida("global");
		_crono.creaMedida("seleccion");
		_crono.creaMedida("cruce");
		_crono.creaMedida("mutacion");
	}

	Cromosoma ejecuta(){
		double mediaActual, mediaAnterior;
		_generacion = 0;
		_genDescartadas = 0;
		_crono.iniciaMedida("global", std::chrono::high_resolution_clock::now());
		_pob.generaPoblacionAleatoria(_param.tamPob, _param.minNodos, _param.maxNodos, _param.densidad);
		mediaAnterior = evaluarPoblacion();
		while (_generacion < _param.iteraciones){
			int nElite = (int)(_param.tamPob * 0.02);
			std::vector<Cromosoma> elite;
			if (_param.elitismo){
				_pob.ordenar();
				for (std::size_t i = 0; i < nElite; ++i){
					elite.push_back(_pob.individuos[i]);
				}
			}
			_crono.iniciaMedida("seleccion", std::chrono::high_resolution_clock::now());
			seleccion();
			_crono.finalizaMedida("seleccion", std::chrono::high_resolution_clock::now());

			_crono.iniciaMedida("cruce", std::chrono::high_resolution_clock::now());
			cruce();
			_crono.finalizaMedida("cruce", std::chrono::high_resolution_clock::now());

			_crono.iniciaMedida("mutacion", std::chrono::high_resolution_clock::now());
			mutacion();
			_crono.finalizaMedida("mutacion", std::chrono::high_resolution_clock::now());

			if (_param.bloating){
				_pob.bloating(_param.maxNodos);
			}
			if (_param.elitismo){
				_pob.ordenar();
				for (std::size_t i = 0; i < nElite; ++i){
					_pob.individuos.pop_back();				// Asegurarse de que esto funciona bien !!!
				}
				for (std::size_t i = 0; i < nElite; ++i){
					_pob.individuos.push_back(elite[i]);
				}
			}

			mediaActual = evaluarPoblacion();
			if (_param.contractividad){
				if (mediaAnterior < mediaActual){
					_generacion++;
					notifyGeneracionTerminada(_elMejor.getAdaptacion(), _pob.individuos[_indexMejor].getAdaptacion(), mediaActual);
				}
				else{
					_genDescartadas++;
				}
				if (_genDescartadas > 10 * _param.iteraciones){
					_param.contractividad = false;
				}
			}
			else{
				_generacion++;
				notifyGeneracionTerminada(_elMejor.getAdaptacion(), _pob.individuos[_indexMejor].getAdaptacion(), mediaActual);
			}

		}	// Fin while generaciones
		//_elMejor.evalua();
		_crono.finalizaMedida("global", std::chrono::high_resolution_clock::now());
		notifyAGTerminado(_elMejor, _crono.getMediaAsMilli("global"), _crono.getMediaAsMilli("seleccion"), _crono.getMediaAsMilli("cruce"), _crono.getMediaAsMilli("mutacion"));
		return _elMejor;
	}

	std::string getDatos(){
		std::string ret = "";
		ret += "Datos de ejecucion:\n";
		ret += "\tTiempo total de ejecucion: " + std::to_string(_crono.getMediaAsMilli("global")) + "ms\n";
		ret += "\tTiempo medio de Seleccion: " + std::to_string(_crono.getMediaAsMilli("seleccion")) + "ms\n";
		ret += "\tTiempo medio de Cruce: " + std::to_string(_crono.getMediaAsMilli("cruce")) + "ms\n";
		ret += "\tTiempo medio de Mutacion: " + std::to_string(_crono.getMediaAsMilli("mutacion")) + "ms\n";
		return ret;
	}

	void addObserver(IAGObserver& o){
		_obs.push_back(&o);
	}

	/*
	void removeObserver(IAGObserver o){
		auto it = std::find(_obs.begin(), _obs.end(), &o);
		if (it != _obs.end())
			_obs.erase(it);
	}*/

private:

	double evaluarPoblacion(){
		double maximo = -50000;
		double minimo = +50000;
		double aux = 0;
		double sumaAptitud = 0;
		double puntAcum = 0;
		double media = 0;
		_pob.evalua();	// Esto asegura que la adaptacion esta actualizada

		// Busqueda del mejor
		for (std::size_t i = 0; i < _param.tamPob; ++i){
			aux = _pob.individuos[i].getAdaptacion();
			sumaAptitud += aux;
			if (maximo < aux){
				maximo = aux;
				_indexMejor = i;
			}
		}

		// Ajuste de las puntuaciones (normal y acumulada)
		for (std::size_t i = 0; i < _param.tamPob; ++i){
			_pob.individuos[i].setPunt(_pob.individuos[i].getAdaptacion() / sumaAptitud);
			_pob.individuos[i].setPuntAcum(_pob.individuos[i].getPunt() + puntAcum);
			puntAcum += _pob.individuos[i].getPunt();
		}

		// Se determina si hay alguno mejor que el que ya se tiene
		double evElMejor = _elMejor.getAdaptacion();
		double evMejorActual = _pob.individuos[_indexMejor].getAdaptacion();
		if (evMejorActual > evElMejor){
			_elMejor = _pob.individuos[_indexMejor];
		}

		// Se calcula la media de aptitud de esta generacion
		for (std::size_t i = 0; i < _param.tamPob; ++i){
			media += _pob.individuos[i].getAdaptacion();
		}

		return (media / _param.tamPob);
	}

	void seleccion(){
		for (size_t i = 0; i < 800000; ++i){
			__nop();
		}
	}

	void cruce(){
		for (size_t i = 0; i < 1600000; ++i){
			__nop();
		}
	}

	void mutacion(){
		for (size_t i = 0; i < 3200000; ++i){
			__nop();
		}
	}

	void notifyGeneracionTerminada(double mejor, double mejorGen, double media){
		for (IAGObserver* o : _obs){
			o->onGeneracionTerminada(mejor, mejorGen, media);
		}
	}

	void notifyAGTerminado(Cromosoma mejor, double total, double tmSel, double tmCruce, double tmMut){
		for (IAGObserver* o : _obs){
			o->onAGTerminado(mejor, total, tmSel, tmCruce, tmMut);
		}
	}

	unsigned int _generacion;
	unsigned int _genDescartadas;
	Poblacion _pob;
	Parametros _param;
	unsigned int _indexMejor;
	Cromosoma _elMejor;
	Cronometro _crono;

	std::vector<IAGObserver*> _obs;
};

#endif