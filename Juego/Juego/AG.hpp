#ifndef AG_HPP
#define AG_HPP

#include "myRandom.hpp"
#include "Poblacion.hpp"
#include "Cromosoma.hpp"
#include "Parametros.hpp"

class AG {
public:
	AG(Parametros p){
		_param = p;
	}

	void updateParam(Parametros p){
		_param = p;
	}

	Cromosoma ejecuta(){
		bool contractividadLocal = _param.contractividad;
		double mediaActual, mediaAnterior;
		_generacion = 0;
		_genDescartadas = 0;
				
		_pob.generaPoblacionAleatoria(_param.tamPob, _param.minNodos, _param.maxNodos, _param.densidad);
		_pob.evalua();
		_elMejor = _pob.individuos[0];
		
		mediaAnterior = evaluarPoblacion();
		
		while (_generacion < _param.iteraciones){
			_marcados.clear();
			int nElite = (int)(_param.tamPob * 0.02);
			std::vector<Cromosoma> elite;
			if (_param.elitismo){
				_pob.ordenar();
				for (std::size_t i = 0; i < nElite; ++i){
					elite.push_back(_pob.individuos[i]);
				}
			}
			seleccion();
			
			cruce();
						
			mutacion();
			

			if (_param.bloating){
				_pob.bloating(_param.maxNodos, _marcados);
			}

			_pob.evaluaMarcados(_marcados);

			if (_param.elitismo){
				_pob.ordenar();
				for (std::size_t i = 0; i < nElite; ++i){
					_pob.individuos.pop_back();				
				}
				for (std::size_t i = 0; i < nElite; ++i){
					_pob.individuos.push_back(elite[i]);
				}
			}
						
			mediaActual = evaluarPoblacion();
			
			if (contractividadLocal){
				if (mediaAnterior < mediaActual){
					mediaAnterior = mediaActual;
					_generacion++;
				}
				else{
					_genDescartadas++;
				}
				if (_genDescartadas > 0.5 * _param.iteraciones){		// Valorar esta condicion (inicialmente era 10*iter)
					contractividadLocal = false;
				}
			}
			else{
				_generacion++;
			}

		}	// Fin while generaciones
		return _elMejor;
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

		return (sumaAptitud / _param.tamPob);
	}

	void seleccion(){
		_param.seleccion->seleccionar(&_pob);
	}

	void cruce(){
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
			_marcados.insert(seleccionados[i + 1]);
			_param.cruce->cruzar(&_pob.individuos[seleccionados[i]], &_pob.individuos[seleccionados[i + 1]]);
		}

	}

	void mutacion(){
		int* seleccionados = new int[_pob._tam];
		int numSeleMut = 0;
		double prob;
		for (int i = 0; i < _pob._tam; ++i){
			prob = myRandom::getRandom(0.f, 1.f);
			if (prob < _param.probMutacion){
				seleccionados[numSeleMut] = i;
				numSeleMut++;
			}
		}

		for (int i = 0; i < numSeleMut; ++i){
			_marcados.insert(seleccionados[i]);
			_param.mutacion->mutar(&_pob.individuos[seleccionados[i]]);
		}
	}

	unsigned int _generacion;
	unsigned int _genDescartadas;
	Poblacion _pob;
	Parametros _param;
	unsigned int _indexMejor;
	Cromosoma _elMejor;
	std::set<unsigned int> _marcados;
};

#endif