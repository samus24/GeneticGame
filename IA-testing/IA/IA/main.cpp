#include <iostream>
#include "AG.hpp"
#include "controlador.hpp"
#include "VistaTerminal.hpp"
#include "MetodoSeleccion.hpp"
#include "MetodoMutacion.hpp"
#include "metodoCruce.hpp"
#include "Ventana.hpp"
#include "VentanaParametros.hpp"
#include "FileLogger.hpp"

int main() {

	Parametros p;
	AG ag(p);
	Controlador c(ag);
	FileLogger fl(c);
	VistaTerminal term(c);
	Ventana v(c);

	v.run(p.iteraciones);

	return 0;
}
