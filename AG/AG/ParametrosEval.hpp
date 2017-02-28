#ifndef PARAMETROS_EVAL_HPP
#define PARAMETROS_EVAL_HPP


class ParametrosEval {
public:
	unsigned int nodosOptimos;		// Numero de nodos optimos
	double gradoOptimo;				// Media de grado (aristas de cada nodo) optima
	unsigned int anchoOptimo;		// Media de ancho de sala optima
	unsigned int altoOptimo;		// Media de alto de sala optima
	unsigned int ciclosOptimos;		// Numero de ciclos optimo
	unsigned int enemigosOptimos;	// Cantidad de enemigos optima
	unsigned int cofresOptimos;		// Cantidad de cofres optima
};

#endif