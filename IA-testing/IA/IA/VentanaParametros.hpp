#ifndef VENTANAPARAMETROS_HPP
#define VENTANAPARAMETROS_HPP

#include <SFML/Graphics.hpp>

#include "parametros.hpp"
#include "TextField.hpp"
#include "CheckButton.hpp"
#include "ComboBox.hpp"

class VentanaParametros : public sf::Drawable{
public:
	VentanaParametros(sf::Vector2f size) :
		_size(size),
		_locked(false)
	{
		_param.tamPob = 10;
		_param.iteraciones = 100;
		_param.minNodos = 2;
		_param.maxNodos = 5;
		_param.elitismo = false;
		_param.bloating = true;
		_param.contractividad = false;
		_param.probCruce = 0.65;
		_param.probMutacion = 0.25;
		_param.seleccion = new seleccionTorneo();
		_param.cruce = new cruceSimple();
		_param.mutacion = new mutacionCombinada();
	}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		
	}

	sf::Vector2f _size;
	Parametros _param;
	bool _locked;


};

#endif