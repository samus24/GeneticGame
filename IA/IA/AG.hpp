#ifndef AG_HPP
#define AG_HPP

#include <fstream>
#include <iostream>
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

		_crono.creaMedida("evalua");

		_crono.creaMedida("init");
	}

	Cromosoma ejecuta(){
		_crono.limpiaMedida("global");
		_crono.limpiaMedida("seleccion");
		_crono.limpiaMedida("cruce");
		_crono.limpiaMedida("mutacion");
		_crono.limpiaMedida("evalua");

		_crono.limpiaMedida("init");
		_crono.limpiaMedida("eval");
		_elMejor = Cromosoma();

		double mediaActual, mediaAnterior;
		_generacion = 0;
		_genDescartadas = 0;
		_crono.iniciaMedida("global", std::chrono::high_resolution_clock::now());

		_crono.iniciaMedida("init", std::chrono::high_resolution_clock::now());
		_pob.generaPoblacionAleatoria(_param.tamPob, _param.minNodos, _param.maxNodos, _param.opValidas);
		_crono.finalizaMedida("init", std::chrono::high_resolution_clock::now());

		_crono.iniciaMedida("evalua", std::chrono::high_resolution_clock::now());
		_pob.evalua(maps, _param.paralelizar, _obsCrom);
		_crono.finalizaMedida("evalua", std::chrono::high_resolution_clock::now());
		_pob.individuos[0].getCopia(_elMejor);

		mediaAnterior = evaluarPoblacion();

		std::cout << _pob._tam << " individuos evaluados en " << (_crono.getMediaAsMilli("evalua") / 1000.f) << "seg, comienza el AG" << std::endl;
		_crono.limpiaMedida("evalua");

		while (_generacion < _param.iteraciones){
			_marcados.clear();
			int nElite = (int)(_param.tamPob * 0.02);
			std::vector<Cromosoma> elite;
			if (_param.elitismo){
				elite.resize(nElite);
				_pob.ordenar();
				for (std::size_t i = 0; i < nElite; ++i){
					_pob.individuos[i].getCopia(elite[i]);
				}
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
				_pob.bloating(_param.maxNodos, _marcados, _param.opValidas);
			}

			_pob.eliminaIntrones(_marcados, _param.opValidas);

			std::cout << "Hay " << _marcados.size() << " marcados en la generacion " << _generacion << std::endl;

			//factorPatrulla*factorAtaque*((cExpl + cAndadas + turnosQueValen) + atacado*(enemigo.golpesEvitados / 20 + enemigo.golpes + turnosAtaque)) - enemigo.intentos - abs(distancia - enemigo.rango) - enemigo.turnosGolpeo;
			//std::cout << "FactorPatrulla - FactorAtaque - SumaPatrulla - Atacado - SumaAtaque - Intentos - Distancia - TurnosGolpeo => Fitness" << std::endl;

			_crono.iniciaMedida("evalua", std::chrono::high_resolution_clock::now());
			_pob.evaluaMarcados(maps, _marcados, _param.paralelizar, _obsCrom);
			_crono.finalizaMedida("evalua", std::chrono::high_resolution_clock::now());

			std::cout << "La media de evaluacion de pob es de " << (_crono.getMediaAsMilli("evalua") / 1000.f) << "seg" << std::endl;
			_crono.limpiaMedida("evalua");

			if (_param.elitismo){
				_pob.ordenar();
				for (std::size_t i = 0; i < nElite; ++i){
					elite[i].getCopia(_pob.individuos[_param.tamPob - nElite + i]);
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

		}
		_crono.finalizaMedida("global", std::chrono::high_resolution_clock::now());
		//_elMejor.evalua(maps);
		notifyAGTerminado(_elMejor, _crono.getMediaAsMilli("global"), _crono.getMediaAsMilli("seleccion"), _crono.getMediaAsMilli("cruce"), _crono.getMediaAsMilli("mutacion"), _crono.getMediaAsMilli("init"), _crono.getMediaAsMilli("evalua"), _pob);
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

	std::vector<Mapa> getMapas() const{
		return maps;
	}

	void addObserver(IAGObserver& o){
		_obs.push_back(&o);
	}

	void addCromosomaObserver(ICromosomaObserver& o){
		_obsCrom.push_back(&o);
	}

	void setParametros(Parametros p){
		_param = p;
	}

private:

	double evaluarPoblacion(){
		double maximo = -50000;
		double aux = 0;
		double sumaAptitud = 0;
		double puntAcum = 0;
		double media = 0;

		// Se reajustan las adaptaciones para que sean todas >0
		double minimo = 5000000;
		for (std::size_t i = 0; i < _param.tamPob; ++i){
			if (minimo > _pob.individuos[i].getAdaptacion()){
				minimo = _pob.individuos[i].getAdaptacion();
			}
		}

		if (minimo < 0){
			minimo = -minimo;
			for (std::size_t i = 0; i < _param.tamPob; ++i){
				_pob.individuos[i].setAdaptacion(_pob.individuos[i].getAdaptacion() + minimo);
			}
		}

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
		/*
		// Se calcula la media de aptitud de esta generacion
		for (std::size_t i = 0; i < _param.tamPob; ++i){
			media += _pob.individuos[i].getAdaptacion();
		}
		*/
		return (sumaAptitud / _param.tamPob);
	}

	void seleccion(){
		_param.seleccion->seleccionar(&_pob, true);
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
			_marcados.insert(seleccionados[i]);
			_marcados.insert(seleccionados[i+1]);
			_param.cruce->cruzar(&_pob.individuos[seleccionados[i]], &_pob.individuos[seleccionados[i + 1]], tipo);
		}

	}

	void mutacion(TipoArbol tipo){
		//Cromosoma* seleccionados = new Cromosoma[_pob._tam];
		int* seleccionados = new int[_pob._tam];
		int actualPos = 0;
		double prob;
		for (int i = 0; i < _pob._tam; ++i){
			prob = myRandom::getRandom(0.f, 1.f);
			if (prob < _param.probMutacion){
				seleccionados[actualPos] = i;
				actualPos++;
			}
		}

		for (int i = 0; i < actualPos; ++i){
			_marcados.insert(seleccionados[i]);
			_param.mutacion->mutar(&_pob.individuos[seleccionados[i]], tipo, _param.opValidas);
		}
	}

	void notifyGeneracionTerminada(double mejor, double mejorGen, double media){
		for (IAGObserver* o : _obs){
			o->onGeneracionTerminada(mejor, mejorGen, media);
		}
	}

	void notifyAGTerminado(Cromosoma mejor, double total, double tmSel, double tmCruce, double tmMut, double tInit, double tmEval, poblacion pob){
		for (IAGObserver* o : _obs){
			o->onAGTerminado(mejor, total, tmSel, tmCruce, tmMut, tInit, tmEval, pob);
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
	std::set<unsigned int> _marcados;
};

#endif