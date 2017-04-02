#include <iostream>
#include "AG.hpp"
#include "controlador.hpp"
#include "MetodoSeleccion.hpp"
#include "MetodoMutacion.hpp"
#include "metodoCruce.hpp"
#include "parametros.hpp"

int main() {

	Parametros p;
	p.tamPob = 30;
	p.iteraciones = 30;
	p.minNodos = 10;
	p.maxNodos = 40;
	p.densidad = 0.03;
	p.elitismo = false;
	p.bloating = true;
	p.contractividad = false;
	p.probCruce = 0.6;
	p.probMutacion = 0.02;
	p.seleccion = new seleccionRuleta();
	p.cruce = new cruceSimple();
	p.mutacion = new mutacionArbol();

	AG ag(p);
	Controlador c(ag);

	//Faltaria la ventana para sacar datos

	return 0;
}
