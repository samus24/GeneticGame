#include <iostream>
#include "AG.hpp"
#include "controlador.hpp"
#include "VistaTerminal.hpp"
#include "MetodoSeleccion.hpp"
#include "MetodoMutacion.hpp"
#include "metodoCruce.hpp"
#include "Ventana.hpp"
#include "FileLogger.hpp"

int main() {

	Parametros p;
	p.tamPob = 30;
	p.iteraciones = 100;// 30;
	p.minNodos = 1;
	p.maxNodos = 5;// 7; Cambio para debug
	p.elitismo = false;
	p.bloating = true;
	p.contractividad = false;
	p.probCruce = 0.6;
	p.probMutacion = 0.2;
	p.seleccion = new seleccionTorneo();
	p.cruce = new cruceSimple();
	p.mutacion = new mutacionTerminal();
	AG ag(p);
	Controlador c(ag);
	FileLogger fl(c);
	VistaTerminal term(c);
	Ventana v(c);

	v.run(p.iteraciones);

	return 0;
}
