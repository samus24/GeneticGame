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
		_botonPrev(sf::Vector2f(pos.x + size.x - (50 * 2), pos.y + size.y - (50 + 2)),
		sf::Vector2f(50, 50), "PREV", sf::Color(100, 200, 200), 15),
		_botonNext(sf::Vector2f(pos.x + size.x - (50) + 4, pos.y + size.y - (50 + 2)),
		sf::Vector2f(50, 50), "NEXT", sf::Color(100, 200, 200), 15)
	{
		_size = size;
		_font.loadFromFile("arial.ttf");
		if (!_texture.loadFromFile("images/suelo1.png"))
			exit(-1);

	}

	RoomViewer(sf::Vector2f pos, sf::Vector2f size, Cromosoma model) :
		_botonPrev(sf::Vector2f(pos.x + size.x - (50 * 2), pos.y + size.y - (50 + 2)),
		sf::Vector2f(50, 50),"PREV",sf::Color(100, 200, 200),15),
		_botonNext(sf::Vector2f(pos.x + size.x - (50) + 4, pos.y + size.y - (50 + 2)),
		sf::Vector2f(50, 50),"NEXT",sf::Color(100, 200, 200),15)
	{
		RoomViewer(pos, size);
		_currentRoom = 0;
		_model = model;
	}

	void setModel(Cromosoma model){
		_model = model;
		_currentRoom = 0;
		update();
	}

	Cromosoma getModel() const{
		return _model;
	}

	void handleClick(sf::Vector2f point){
		if (_botonPrev.contains(point)){
			if (_currentRoom > 0)
				--_currentRoom;
			update();
		}
		else if (_botonNext.contains(point)){
			if (_currentRoom < _model.getMejorCC().size()-1)
				++_currentRoom;
			update();
		}
	}


private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// apply the tileset texture
		states.texture = &_texture;

		// draw the vertex array
		target.draw(m_vertices, states);

		sf::Text t;
		t.setCharacterSize(20);
		t.setFillColor(sf::Color::Black);
		t.setFont(_font);
		t.setString(std::to_string(_currentRoom));
		t.setPosition(this->getPosition());
		t.move(_size.x - 50, 10);
		
		target.draw(t);
		target.draw(_botonNext);
		target.draw(_botonPrev);
	}

	void update(){
		auto nodos = _model.getMejorCC().getNodos();
		auto it = nodos.begin();
		std::advance(it, _currentRoom);
		Gen sala = it->second;
		m_vertices.clear();
		// resize the vertex array to fit the level size
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(sala.getAncho()*sala.getAlto() * 4);

		// populate the vertex array, with one quad per tile
		for (unsigned int i = 0; i < sala.getAncho(); ++i)
			for (unsigned int j = 0; j < sala.getAlto(); ++j)
			{
				// get the current tile number
				int tileNumber = 0;

				// find its position in the tileset texture
				int tu = tileNumber % (_texture.getSize().x / _TILESIZE);
				int tv = tileNumber / (_texture.getSize().x / _TILESIZE);

				// get a pointer to the current tile's quad
				sf::Vertex* quad = &m_vertices[(i + j * sala.getAncho()) * 4];

				// define its 4 corners
				quad[0].position = sf::Vector2f(i * _TILESIZE, j * _TILESIZE);
				quad[1].position = sf::Vector2f((i + 1) * _TILESIZE, j * _TILESIZE);
				quad[2].position = sf::Vector2f((i + 1) * _TILESIZE, (j + 1) * _TILESIZE);
				quad[3].position = sf::Vector2f(i * _TILESIZE, (j + 1) * _TILESIZE);

				// define its 4 texture coordinates
				quad[0].texCoords = sf::Vector2f(tu * _TILESIZE, tv * _TILESIZE);
				quad[1].texCoords = sf::Vector2f((tu + 1) * _TILESIZE, tv * _TILESIZE);
				quad[2].texCoords = sf::Vector2f((tu + 1) * _TILESIZE, (tv + 1) * _TILESIZE);
				quad[3].texCoords = sf::Vector2f(tu * _TILESIZE, (tv + 1) * _TILESIZE);
			}
		for (size_t i = 0; i < m_vertices.getVertexCount(); ++i){
			m_vertices[i].position += this->getPosition();
		}
	}

	sf::Vector2f _size;
	Cromosoma _model;
	sf::Font _font;
	sf::Texture _texture;
	sf::VertexArray m_vertices;
	TextButton _botonPrev;
	TextButton _botonNext;
	
	unsigned int _currentRoom;

	const unsigned int _TILESIZE = 64;
};

#endif