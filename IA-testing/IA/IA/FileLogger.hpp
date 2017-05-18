#ifndef FILELOGGER_HPP
#define FILELOGGER_HPP

#include <fstream>
#include <unordered_map>
#include "iagobserver.hpp"
#include "ICromosomaObserver.hpp"

class FileLogger : public IAGObserver, public ICromosomaObserver{
public:
	FileLogger(Controlador& c)
	{
		_ctrl = &c;

		_ctrl->addObserver(*this);
		_ctrl->addCromosomaObserver(*this);

		generacion = 0;
		individuo = 0;

		time_t t = time(0);   // get time now
		struct tm now;
		localtime_s(&now, &t);

		_date = "[" + std::to_string(now.tm_year+1900) + "-" + std::to_string(now.tm_mon + 1) + "-" + std::to_string(now.tm_mday) + "][" + std::to_string(now.tm_hour) + "-" + std::to_string(now.tm_min) + "-" + std::to_string(now.tm_sec) + "]";
		std::string pathAG = "Registros/" + _date + "registroAG.txt";
		std::string pathPob = "Registros/" + _date + "registroPob.txt";

		_regAG.open(pathAG, std::ios_base::out | std::ios_base::app);
		_regPob.open(pathPob, std::ios_base::out | std::ios_base::app);

		_regAG << "\n\nNUEVA EJECUCION" << std::endl;
		_regPob << "\n\nNUEVA EJECUCION" << std::endl;
	}

	void onGeneracionTerminada(double mejor, double mejorGen, double media){
		_regAG << "Gen " + std::to_string(generacion) + ":" << std::endl;
		_regAG << "Fitness mejor: " + std::to_string(mejor) + "; Fitness medio: " + std::to_string(media) << std::endl;

		_regPob << std::endl << "Nueva Generacion " << generacion << std::endl;
		generacion++;
	}

	void onAGTerminado(Cromosoma mejor, double total, double tmSel, double tmCruce, double tmMut, double tInit, double tmEval, poblacion pob){
		_regAG << " -- AG Finalizado --" << std::endl;
		_regAG << "Mejor individuo: " + std::to_string(mejor.getAdaptacion()) << std::endl;
		_regAG << "Arbol patrulla: " + mejor.getGenotipo(0).toString() << std::endl;
		_regAG << "Arbol ataque: " + mejor.getGenotipo(1).toString() + "\n" << std::endl;
		_regAG << "Notas" << std::endl;
		_regAG << "Punt. exploracion: " + std::to_string(mejor.getMediaValores()[0]) << std::endl;
		_regAG << "Punt. Ataque: " + std::to_string(mejor.getMediaValores()[1]) << std::endl;
		_regAG << "Punt. Distancia: " + std::to_string(mejor.getMediaValores()[2]) << std::endl;
		_regAG << "Tiempos de ejecucion" << std::endl;
		_regAG << "Tiempo total de ej: " + std::to_string((total / 1000)) + " seg" << std::endl;
		_regAG << "Tiempo inicializacion: " + std::to_string(tInit) + " ms" << std::endl;
		_regAG << "Tiempo medio seleccion: " + std::to_string(tmSel) + " ms" << std::endl;
		_regAG << "Tiempo medio cruce: " + std::to_string(tmCruce) + " ms" << std::endl;
		_regAG << "Tiempo medio mutacion: " + std::to_string(tmMut) + " ms" << std::endl;
		_regAG << "Tiempo medio evaluacion: " + std::to_string((tmEval / 1000)) + " seg\n\n" << std::endl;
		_regAG << "Datos de Poblacion" << std::endl;
		for (size_t i = 0; i < pob._tam; ++i){
			_regAG << "Individuo " + std::to_string(i) + ": " + std::to_string(pob.individuos[i].getAdaptacion()) << ((pob.individuos[i].getDescartado()) ? "(descartado)" : "") << std::endl;
			_regAG << "Arbol patrulla: " + pob.individuos[i].getGenotipo(0).toString() << std::endl;
			_regAG << "Arbol ataque: " + pob.individuos[i].getGenotipo(1).toString() + "\n" << std::endl;
		}

		_regAG.close();
		_regPob.close();
	}
	void onSimulacionIniciada(const Cromosoma* c){
		
	}

	void onTurno(const Cromosoma* c, npc jugador, npc enemigo, Mapa m, Mapa explorado, Mapa andado, Mapa andadoAtaque){
	}

	void onMapaTerminado(double fitness){

	}

	void onSimulacionTerminada(const Cromosoma* c){
		_regPob << "Fitness Individuo: " + std::to_string(c->getAdaptacion()) << ((c->getDescartado()) ? "(descartado)" : "") << std::endl;
		_regPob << "Arbol patrulla: " + c->getGenotipo(0).toString() << std::endl;
		_regPob << "Arbol ataque: " + c->getGenotipo(1).toString() << std::endl;
		_regPob << "Notas" << std::endl;
		_regPob << "Punt. exploracion: " + std::to_string(c->getMediaValores()[0]) << std::endl;
		_regPob << "Punt. Ataque: " + std::to_string(c->getMediaValores()[1]) << std::endl;
		_regPob << "Punt. distancia: " + std::to_string(c->getMediaValores()[2]) << std::endl;
	}


private:
	unsigned int generacion;
	unsigned int individuo;

	std::ofstream _regAG;
	std::ofstream _regPob;
	Controlador* _ctrl;

	std::string _date;
};
#endif