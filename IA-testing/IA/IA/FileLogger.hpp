#ifndef FILELOGGER_HPP
#define FILELOGGER_HPP

#include <fstream>
#include <unordered_map>
#include "iagobserver.hpp"
#include "ICromosomaObserver.hpp"

class FileLogger : public IAGObserver, public ICromosomaObserver{
public:
	FileLogger(Controlador& c):
		_regAG("registroAG.txt", std::ios_base::out | std::ios_base::app),
		_regPob("registroPoblacion.txt", std::ios_base::out | std::ios_base::app)
	{
		_ctrl = &c;

		_ctrl->addObserver(*this);
		_ctrl->addCromosomaObserver(*this);

		generacion = 0;
		individuo = 0;

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
		_regAG << "Punt. exploracion: " + std::to_string(mejor.getMediaValores()[0]) + "(" + std::to_string(mejor.getMediaValores()[0] * mejor.getPesos()[0]) + " sobre el total)" << std::endl;
		_regAG << "Punt. andar: " + std::to_string(mejor.getMediaValores()[1]) + "(" + std::to_string(mejor.getMediaValores()[1] * mejor.getPesos()[1]) + " sobre el total)" << std::endl;
		_regAG << "Punt. golpes intentados: " + std::to_string(mejor.getMediaValores()[2]) + "(" + std::to_string(mejor.getMediaValores()[2] * mejor.getPesos()[2]) + " sobre el total)" << std::endl;
		_regAG << "Punt. golpes evitados: " + std::to_string(mejor.getMediaValores()[3]) + "(" + std::to_string(mejor.getMediaValores()[3] * mejor.getPesos()[3]) + " sobre el total)" << std::endl;
		_regAG << "Punt. daño al jugador: " + std::to_string(mejor.getMediaValores()[4]) + "(" + std::to_string(mejor.getMediaValores()[4] * mejor.getPesos()[4]) + " sobre el total)\n" << std::endl;
		_regAG << "Tiempos de ejecucion" << std::endl;
		_regAG << "Tiempo total de ej: " + std::to_string((total / 1000)) + " seg" << std::endl;
		_regAG << "Tiempo inicializacion: " + std::to_string(tInit) + " ms" << std::endl;
		_regAG << "Tiempo medio seleccion: " + std::to_string(tmSel) + " ms" << std::endl;
		_regAG << "Tiempo medio cruce: " + std::to_string(tmCruce) + " ms" << std::endl;
		_regAG << "Tiempo medio mutacion: " + std::to_string(tmMut) + " ms" << std::endl;
		_regAG << "Tiempo medio evaluacion: " + std::to_string((tmEval / 1000)) + " seg\n\n" << std::endl;
		_regAG << "Datos de Poblacion" << std::endl;
		for (size_t i = 0; i < pob._tam; ++i){
			_regAG << "Individuo " + std::to_string(i) + ": " + std::to_string(pob.individuos[i].getAdaptacion()) << std::endl;
			_regAG << "Arbol patrulla: " + pob.individuos[i].getGenotipo(0).toString() << std::endl;
			_regAG << "Arbol ataque: " + pob.individuos[i].getGenotipo(1).toString() + "\n" << std::endl;
		}

		_regAG.close();
		_regPob.close();
	}

	void onTurno(const Cromosoma* c, npc jugador, npc enemigo, Mapa m, Mapa explorado, Mapa andado){
	}

	void onSimulacionTerminada(const Cromosoma* c){
		_regPob << "Fitness Individuo: " + std::to_string(c->getAdaptacion()) << ((c->getDescartado()) ? "(descartado)" : "") << std::endl;
		_regPob << "Arbol patrulla: " + c->getGenotipo(0).toString() << std::endl;
		_regPob << "Arbol ataque: " + c->getGenotipo(1).toString() << std::endl;
		_regPob << "Notas" << std::endl;
		_regPob << "Punt. exploracion: " + std::to_string(c->getMediaValores()[0]) + "(" + std::to_string(c->getMediaValores()[0] * c->getPesos()[0]) + " sobre el total)" << std::endl;
		_regPob << "Punt. andar: " + std::to_string(c->getMediaValores()[1]) + "(" + std::to_string(c->getMediaValores()[1] * c->getPesos()[1]) + " sobre el total)" << std::endl;
		_regPob << "Punt. golpes intentados: " + std::to_string(c->getMediaValores()[2]) + "(" + std::to_string(c->getMediaValores()[2] * c->getPesos()[2]) + " sobre el total)" << std::endl;
		_regPob << "Punt. golpes evitados: " + std::to_string(c->getMediaValores()[3]) + "(" + std::to_string(c->getMediaValores()[3] * c->getPesos()[3]) + " sobre el total)" << std::endl;
		_regPob << "Punt. daño al jugador: " + std::to_string(c->getMediaValores()[4]) + "(" + std::to_string(c->getMediaValores()[4] * c->getPesos()[4]) + " sobre el total)\n" << std::endl;

		_regPob.close();	// Forzar escritura
		_regPob.open("registroPoblacion.txt", std::ios_base::out | std::ios_base::app);
	}


private:
	unsigned int generacion;
	unsigned int individuo;

	std::ofstream _regAG;
	std::ofstream _regPob;
	Controlador* _ctrl;
};
#endif