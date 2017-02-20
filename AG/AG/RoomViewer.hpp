#ifndef ROOMVIEWER_HPP
#define ROOMVIEWER_HPP

#include <SFML\Graphics.hpp>
#include <queue>
#include "Cromosoma.hpp"
#include "Button.hpp"

class RoomViewer : public sf::Rect<float>, public sf::Drawable, public sf::Transformable{
public:
	RoomViewer(sf::Vector2f pos, sf::Vector2f size) :
		sf::Rect<float>(pos, size),
		_botonPrev(sf::Vector2f(pos.x + size.x - (50 * 2), pos.y + size.y - 50),
		sf::Vector2f(50,50),
		"PREV"),
		_botonNext(sf::Vector2f(pos.x + size.x - (50 * 2), pos.y + size.y - 50),
		sf::Vector2f(50,50),
		"NEXT")
	{
		_size = size;
		_font.loadFromFile("arial.ttf");

	}

	RoomViewer(sf::Vector2f pos, sf::Vector2f size, Cromosoma model) :
		_botonPrev(sf::Vector2f(pos.x + size.x - (50 * 2), pos.y + size.y - 50),
		sf::Vector2f(50,50),
		"PREV"),
		_botonNext(sf::Vector2f(pos.x + size.x - (50 * 2), pos.y + size.y - 50),
		sf::Vector2f(50,50),
		"NEXT")
	{
		RoomViewer(pos, size);
		update();
	}

	void setModel(Cromosoma model){
		_model = model;
		update();
	}

	Cromosoma getModel() const{
		return _model;
	}


private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		//target.draw(t, states);
		target.draw(_botonNext);
		target.draw(_botonPrev);
	}

	void update(){
		Grafo<Gen> g = _model.getGenotipo();
		
	}



	sf::Vector2f _size;
	Cromosoma _model;
	sf::Font _font;
	TextButton _botonPrev;
	TextButton _botonNext;
};

#endif