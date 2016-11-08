#include <iostream>

#include "Grafo.hpp"

typedef struct {
	int size;
} tNode;


int main(){
	Grafo<tNode> g;
	tNode a, b;
	a.size = 5;
	b.size = 3;
	g.anadeNodo(a);
	g.anadeNodo(b);
	if (!g.anadeArista(1, 2))
		std::cout << "Eso no vale" << std::endl;
	g.anadeArista(0, 1);

	g.borraNodo(0);

	return 0;
}