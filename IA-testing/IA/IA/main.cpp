#include <iostream>
#include "Arbol.hpp"

int main() {

	Nodo n(Operacion::ProgN3, nullptr, 3, 0);
	Arbol a = Arbol(n);

	a.insertaNodo(0, Operacion::Avanza, 0);
	a.insertaNodo(0, Operacion::GiraDer, 1);
	a.insertaNodo(0, Operacion::SiJugador, 2);
	a.insertaNodo(3, Operacion::GiraIz, 0);
	a.insertaNodo(3, Operacion::Avanza, 1);
	std::cout << "Hola";
	return 0;
}
