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
	p.tamPob = 30;							// Tamano de la poblacion
	p.iteraciones = 30;						// Numero maximo de generaciones
	p.minNodos = 10;						// Numero minimo de nodos iniciales
	p.maxNodos = 40;						// Numero maximo de nodos iniciales
	p.densidad = 0.03;						// Densidad de aristas inciales
	p.elitismo = false;						// Elitismo (guarda a los mejores)
	p.bloating = false;						// Bloating	(evita grafos demasiado grandes)
	p.contractividad = false;				// Contractividad (descarta generaciones malas)
	p.probCruce = 0.6;						// Probabilidad de cruce
	p.probMutacion = 0.02;					// Probabilidad de mutacion
	p.seleccion = new SeleccionTorneo();	// Metodo de seleccion (Ver "MetodoSeleccion.hpp")
	p.cruce = new CruceMonopunto();			// Metodo de cruce (Ver "MetodoCruce.hpp")
	p.mutacion = new MutacionNodo();		// Metodo de mutacion (Ver "MetodoMutacion.hpp")

	ParametrosEval pEval;
	pEval.nodosOptimos = 25;
	pEval.gradoOptimo = 2;
	pEval.anchoOptimo = 30;
	pEval.altoOptimo = 20;
	pEval.ciclosOptimos = 4;
	pEval.enemigosOptimos = 1.4;			// Cada sala deberia tener 1.4 enemigos
	pEval.cofresOptimos = 0.4;				// Cada sala deberia tener 0.4 cofres

	AG ag(p,pEval);
	Controlador c(ag);
	Ventana v(c);
	v.run(p.iteraciones);
	return 0;
}