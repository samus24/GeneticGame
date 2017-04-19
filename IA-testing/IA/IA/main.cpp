#include <iostream>
#include "AG.hpp"
#include "controlador.hpp"
#include "VistaTerminal.hpp"
#include "MetodoSeleccion.hpp"
#include "MetodoMutacion.hpp"
#include "metodoCruce.hpp"

int main() {

	Parametros p;
	p.tamPob = 30;
	p.iteraciones = 30;
	p.minNodos = 1;
	p.maxNodos = 2;// 7; Cambio para debug
	p.elitismo = false;
	p.bloating = true;
	p.contractividad = false;
	p.probCruce = 0.6;
	p.probMutacion = 0;
	p.seleccion = new seleccionRuleta();
	p.cruce = new cruceSimple();
	p.mutacion = new mutacionArbol();
	AG ag(p);
	Controlador c(ag);
	VistaTerminal v(c);
	v.ejecuta();

	return 0;
}
