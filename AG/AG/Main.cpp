#include <iostream>

#include <SFML\Graphics.hpp>
#include "AG.hpp"
#include "Cromosoma.hpp"
#include "Plotter.hpp"

int main(){
	sf::RenderWindow window(sf::VideoMode(1000, 800), "Graph Generator");

	Plotter p(sf::Vector2f(0, 0), sf::Vector2f(1000, 800));

	sf::Font f;
	f.loadFromFile("arial.ttf");
	sf::Text t;
	t.setCharacterSize(15);
	t.setPosition(620, 0);
	t.setFillColor(sf::Color::White);
	std::vector<double> data;
	std::vector<double> dataY;

	for (size_t i = 0; i < 50; ++i){
		data.push_back(i);
		dataY.push_back(i);
	}

	p.setEjeX(data);
	p.pushEjeY(dataY, sf::Color::Red, "X=Y");
	p.setNombreEjeX("Generacion");
	p.setNombreEjeY("Adaptacion");

	while (window.isOpen())
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonPressed){

			}
			else if (event.type == sf::Event::MouseButtonReleased){

			}
		}


		// draw the map
		window.clear(sf::Color::White);
		window.draw(p);
		window.display();
	}
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