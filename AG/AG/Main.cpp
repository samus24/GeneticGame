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

	/*
	Grafo<tNode> g;
	tNode n[10];
	for (int i = 0; i < 10; ++i){
		n[i].size = i;
		g.anadeNodo(n[i]);
	}
	/*
	for (int i = 0; i < 1; ++i){
		g.anadeArista(i, 4-i);
	}*
	g.anadeArista(0, 1);
	g.anadeArista(0, 2);
	g.anadeArista(1, 3);
	g.anadeArista(0, 4);
	g.anadeArista(3, 4);

	std::cout << g.getGradoNodo(0) << std::endl;

	//std::vector<Grafo<tNode>> subs = g.divideGrafo(2);

	//Grafo<tNode> h = g.unirGrafo(subs[0], subs[1]);

	std::vector<Grafo<tNode>> subs = g.divideEnGrafos(3);
	Parametros p;
	p.tamPob = 50;
	p.iteraciones = 100;
	p.minNodos = 10;
	p.maxNodos = 50;
	p.densidad = 0.03;
	p.elitismo = false;
	p.bloating = false;
	p.contractividad = false;
	p.probCruce = 0.6;
	p.probMutacion = 0.02;

	AG ag(p);
	Cromosoma mejor = ag.ejecuta();

	std::cout << ag.getDatos();

	return 0;*/