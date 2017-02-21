#include <iostream>

#include <SFML\Graphics.hpp>
#include "AG.hpp"
#include "Cromosoma.hpp"
#include "Plotter.hpp"
#include "Ventana.hpp"
#include "MetodoSeleccion.hpp"
#include "Rellenador.hpp"

int main(){
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
	p.seleccion = new SeleccionRuleta();
	p.cruce = new CruceMonopunto();

	ParametrosEval pEval;
	pEval.nodosOptimos = 25;
	pEval.gradoOptimo = 2.5;
	pEval.anchoOptimo = 30;
	pEval.altoOptimo = 20;
	pEval.ciclosOptimos = 1;
	pEval.enemigosOptimos = 35;
	pEval.cofresOptimos = 10;

	AG ag(p,pEval);
	Controlador c(ag);
	Ventana v(c);
	v.run();
	return 0;
}