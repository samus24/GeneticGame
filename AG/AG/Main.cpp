#include <iostream>

#include "Grafo.hpp"

typedef struct {
	int size;
} tNode;


int main(){
	Grafo<tNode> g;
	tNode n[10];
	for (int i = 0; i < 10; ++i){
		n[i].size = i;
		g.anadeNodo(n[i]);
	}
	/*
	for (int i = 0; i < 1; ++i){
		g.anadeArista(i, 4-i);
	}*/
	g.anadeArista(0, 1);
	g.anadeArista(0, 2);
	g.anadeArista(1, 3);
	g.anadeArista(0, 4);
	g.anadeArista(3, 4);

	std::cout << g.getGradoNodo(0) << std::endl;

	//std::vector<Grafo<tNode>> subs = g.divideGrafo(2);

	//Grafo<tNode> h = g.unirGrafo(subs[0], subs[1]);

	std::vector<Grafo<tNode>> subs = g.divideEnGrafos(3);

	return 0;
}