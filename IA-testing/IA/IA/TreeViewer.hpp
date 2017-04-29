#ifndef TREEVIEWER_HPP
#define TREEVIEWER_HPP

#include <SFML\Graphics.hpp>
#include <queue>
#include "Arbol.hpp"

class TreeViewer : public sf::Rect<float>, public sf::Drawable, public sf::Transformable{
public:
	TreeViewer(sf::Vector2f pos, sf::Vector2f size) :
		sf::Rect<float>(pos, size)
	{
			_pos = pos;
			_size = size;
			_font.loadFromFile("arial.ttf");
	}

	void handleClick(sf::Vector2f point){
	}

	void update(Arbol a, TipoArbol t){
		_uniones.clear();
		_uniones.setPrimitiveType(sf::Lines);
		_nodos.clear();
		_textos.clear();
		sf::Vector2f p(_pos.x + (_size.x/2), _pos.y + 20);
		dibujaNodo(a.getRaiz(), p, _size.x, t);
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();
		target.draw(_uniones, states);
		for (auto it = _nodos.begin(); it != _nodos.end(); ++it)
			target.draw(*it, states);
		for (auto it = _textos.begin(); it != _textos.end(); ++it)
			target.draw(*it, states);
	}

	void dibujaNodo(Nodo* n, sf::Vector2f pos, int espacio, TipoArbol tip){
		if (_uniones.getVertexCount() > 0){
			_uniones.append(sf::Vertex(pos, sf::Color::Green));
		}
		int radio = 20;
		sf::CircleShape c = creaCirculo(radio, tip);
		c.setPosition(pos);
		sf::Text t = creaTexto(opToString(n->getElem()), tip);
		t.setPosition(pos);
		float ancho = t.getLocalBounds().width / 2;
		float alto = t.getLocalBounds().height / 2;
		t.move(-ancho, -alto);
		_nodos.push_back(c);
		_textos.push_back(t);
		if (n->getNhijos() > 0){
			/*
			Reparto equitativo del espacio de los hijos

			int subespacio = espacio / n->getNhijos();
			int dist = 2.5 * radio;
			sf::Vector2f hijoPos(pos.x - (espacio / 2), pos.y + dist);
			hijoPos += sf::Vector2f(subespacio / 2, 0);
			for (size_t i = 0; i < n->getNhijos(); ++i){
				_uniones.append(sf::Vertex(pos, sf::Color::Green));
				dibujaNodo(&n->getHijos()[i], hijoPos, subespacio, tip);
				hijoPos += sf::Vector2f(subespacio, 0);
			}
			*/

			// Reparto en funcion del espacio que necesitan los hijos
			float total = n->getNumNodos() - 1;
			int dist = 2.5 * radio;
			sf::Vector2f hijoPos(pos.x - (espacio / 2), pos.y + dist);
			for (size_t i = 0; i < n->getNhijos(); ++i){
				int subespacio = espacio * (n->getHijos()[i].getNumNodos() / total);
				hijoPos += sf::Vector2f(subespacio/2, 0);
				_uniones.append(sf::Vertex(pos, sf::Color::Green));
				dibujaNodo(&n->getHijos()[i], hijoPos, subespacio, tip);
				hijoPos += sf::Vector2f(subespacio / 2, 0);
			}
		}
	}

	sf::CircleShape creaCirculo(int radio, TipoArbol t){
		sf::CircleShape yo(radio);
		yo.setOrigin(yo.getOrigin().x + radio, yo.getOrigin().y + radio);
		if (t == TipoArbol::Patrulla){
			yo.setFillColor(sf::Color(0, 0, 200, 127));
		}
		else{
			yo.setFillColor(sf::Color(200, 0, 0, 127));
		}
		yo.setOutlineColor(sf::Color(127,127,127));
		yo.setOutlineThickness(2);
		return yo;
	}
	sf::Text creaTexto(std::string texto, TipoArbol t){
		sf::Text ret;
		ret.setFont(_font);
		if (t == TipoArbol::Patrulla){
			ret.setFillColor(sf::Color::Yellow);
		}
		else{
			ret.setFillColor(sf::Color::Black);
		}
		ret.setCharacterSize(12);
		ret.setString(texto);
		return ret;
	}

	sf::Vector2f _pos;
	sf::Vector2f _size;
	sf::Font _font;
	sf::VertexArray _uniones;
	std::vector<sf::CircleShape> _nodos;
	std::vector<sf::Text> _textos;

};

#endif