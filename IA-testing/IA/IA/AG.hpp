#ifndef AG_HPP
#define AG_HPP

#include <fstream>
#include "IAGObserver.hpp"
#include "Poblacion.hpp"
#include "Cromosoma.hpp"
#include "Parametros.hpp"
#include "Cronometro.hpp"

class AG {
public:

	AG(Parametros p) {
		_param = p;
		cargarMapas();
		_crono.creaMedida("global");
		_crono.creaMedida("seleccion");
		_crono.creaMedida("cruce");
		_crono.creaMedida("mutacion");

		_crono.creaMedida("init");
		_crono.creaMedida("eval");
	}

	Cromosoma ejecuta(){
		_crono.limpiaMedida("global");
		_crono.limpiaMedida("seleccion");
		_crono.limpiaMedida("cruce");
		_crono.limpiaMedida("mutacion");

		_crono.limpiaMedida("init");
		_crono.limpiaMedida("eval");
		_elMejor = Cromosoma();

		double mediaActual, mediaAnterior;
		_generacion = 0;
		_genDescartadas = 0;
		_crono.iniciaMedida("global", std::chrono::high_resolution_clock::now());

		_crono.iniciaMedida("init", std::chrono::high_resolution_clock::now());
		_pob.generaPoblacionAleatoria(_param.tamPob, _param.minNodos, _param.maxNodos);
		_crono.finalizaMedida("init", std::chrono::high_resolution_clock::now());
		for (size_t i = 0; i < _obsCrom.size(); ++i){
			_pob.addCromosomaObserver(*_obsCrom.at(i));
		}
		_pob.evalua(maps);

		/*
		for (size_t i = 0; i < _pob._tam; ++i){
			std::cout << "I[" << i << "]: " << _pob.individuos[i].getAdaptacion() << std::endl;
		}
		*/
		_crono.iniciaMedida("eval", std::chrono::high_resolution_clock::now());
		mediaAnterior = evaluarPoblacion();
		_crono.finalizaMedida("eval", std::chrono::high_resolution_clock::now());

		while (_generacion < _param.iteraciones){
			int nElite = (int)(_param.tamPob * 0.02);
			std::vector<Cromosoma> elite;
			if (_param.elitismo){
				/*
				_pob.ordenar();
				for (std::size_t i = 0; i < nElite; ++i){
					elite.push_back(_pob.individuos[i]);
				}
				*/
			}
			_crono.iniciaMedida("seleccion", std::chrono::high_resolution_clock::now());
			seleccion();
			_crono.finalizaMedida("seleccion", std::chrono::high_resolution_clock::now());

			_crono.iniciaMedida("cruce", std::chrono::high_resolution_clock::now());
			cruce(TipoArbol::Patrulla);
			cruce(TipoArbol::Ataque);
			_crono.finalizaMedida("cruce", std::chrono::high_resolution_clock::now());

			_crono.iniciaMedida("mutacion", std::chrono::high_resolution_clock::now());
			mutacion(TipoArbol::Patrulla);
			mutacion(TipoArbol::Ataque);
			_crono.finalizaMedida("mutacion", std::chrono::high_resolution_clock::now());

			if (_param.bloating){
				_pob.bloating(_param.maxNodos, maps);
			}
			if (_param.elitismo){
				/*
				_pob.ordenar();
				for (std::size_t i = 0; i < nElite; ++i){
					_pob.individuos.pop_back();
				}
				for (std::size_t i = 0; i < nElite; ++i){
					_pob.individuos.push_back(elite[i]);
				}*/
			}

			_crono.iniciaMedida("eval", std::chrono::high_resolution_clock::now());
			mediaActual = evaluarPoblacion();
			_crono.finalizaMedida("eval", std::chrono::high_resolution_clock::now());

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

		}
		_crono.finalizaMedida("global", std::chrono::high_resolution_clock::now());
		_elMejor.evalua(maps);
		notifyAGTerminado(_elMejor, _crono.getMediaAsMilli("global"), _crono.getMediaAsMilli("seleccion"), _crono.getMediaAsMilli("cruce"), _crono.getMediaAsMilli("mutacion"), _crono.getMediaAsMilli("init"), _crono.getMediaAsMilli("eval"));
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

	void addCromosomaObserver(ICromosomaObserver& o){
		_obsCrom.push_back(&o);
	}

private:

	double evaluarPoblacion(){
		double maximo = -50000;
		double aux = 0;
		double sumaAptitud = 0;
		double puntAcum = 0;
		double media = 0;

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
		_param.seleccion->seleccionar(_pob, true);
	}

	void cruce(TipoArbol tipo){
		int* seleccionados = new int[_pob._tam];
		int numSeleCruce = 0;
		double prob;
		for (int i = 0; i < _pob._tam; ++i){
			prob = myRandom::getRandom(0.f, 1.f);
			if (prob < _param.probCruce){
				seleccionados[numSeleCruce] = i;
				numSeleCruce++;
			}
		}

		if (numSeleCruce % 2 == 1){
			numSeleCruce--;
		}
		for (int i = 0; i < numSeleCruce; i += 2){
			_param.cruce->cruzar(&_pob.individuos[seleccionados[i]], &_pob.individuos[seleccionados[i + 1]], tipo, maps);
		}

	}

	void mutacion(TipoArbol tipo){
		Cromosoma* seleccionados = new Cromosoma[_pob._tam];
		int actualPos = 0;
		double prob;
		for (int i = 0; i < _pob._tam; ++i){
			prob = myRandom::getRandom(0.f, 1.f);
			if (prob < _param.probMutacion){
				seleccionados[actualPos] = _pob.individuos[i];
				actualPos++;
			}
		}

		for (int i = 0; i < actualPos; ++i){
			_param.mutacion->mutar(&seleccionados[i], tipo, maps);
		}
	}

	void notifyGeneracionTerminada(double mejor, double mejorGen, double media){
		for (IAGObserver* o : _obs){
			o->onGeneracionTerminada(mejor, mejorGen, media);
		}
	}

	void notifyAGTerminado(Cromosoma mejor, double total, double tmSel, double tmCruce, double tmMut, double tInit, double tmEval){
		for (IAGObserver* o : _obs){
			o->onAGTerminado(mejor, total, tmSel, tmCruce, tmMut, tInit, tmEval);
		}
	}

	void cargarMapas() {
		std::ifstream file("mapas.txt", std::ifstream::in);
		int nMapas;
		file >> nMapas;
		while (nMapas > 0) {
			int x, y, i, j, celda, origX, origY;
			i = 0;
			j = 0;
			file >> x;
			file >> y;
			file >> origX; //guardamos las coorden
			file >> origY;
			Mapa map(x, y, origX, origY);
			while (j < y) {
				i = 0;
				while (i < x) {
					file >> celda;
					map.setCasilla(i, j, celda);
					i++;
				}
				j++;
			}
			nMapas--;
			maps.push_back(map);
		}
		file.close();
	}

	unsigned int _generacion;
	unsigned int _genDescartadas;
	poblacion _pob;
	Parametros _param;
	unsigned int _indexMejor;
	Cromosoma _elMejor;
	Cronometro _crono;
	std::vector<IAGObserver*> _obs;
	std::vector<ICromosomaObserver*> _obsCrom;
	std::vector<Mapa> maps;
};

#endif