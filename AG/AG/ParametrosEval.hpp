#ifndef PARAMETROS_EVAL_HPP
#define PARAMETROS_EVAL_HPP


class ParametrosEval {
public:
	unsigned int nodosOptimos;		// Numero de nodos optimos
	double gradoOptimo;				// Media de grado (aristas de cada nodo) optima
	unsigned int anchoOptimo;		// Media de ancho de sala optima
	unsigned int altoOptimo;		// Media de alto de sala optima
	unsigned int ciclosOptimos;		// Numero de ciclos optimo
	double enemigosOptimos;			// Densidad de enemigos (en funcion del numero de salas)
	double cofresOptimos;			// Densidad de cofres (en funcion del numero de salas)
};

#endif