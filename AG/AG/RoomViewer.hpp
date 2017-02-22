#ifndef ROOMVIEWER_HPP
#define ROOMVIEWER_HPP

#include <SFML\Graphics.hpp>
#include <queue>

#include "RandomGen.hpp"
#include "Cromosoma.hpp"
#include "Button.hpp"
#include "Mazmorra.hpp"

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
		_roomInfo.setCharacterSize(15);
		_roomInfo.setFillColor(sf::Color::Black);
		_roomInfo.setFont(_font);
		_roomInfo.setString(std::to_string(_currentRoom));
		_roomInfo.setPosition(this->getPosition());
		_roomInfo.move(_size.x - 75, 10);
		if (!_texture.loadFromFile("images/textures.png"))
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

	void setModel(Cromosoma model, Mazmorra mazmorra){
		_model = model;
		_mazmorra = mazmorra;
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
		
		target.draw(_roomInfo);
		target.draw(_botonNext);
		target.draw(_botonPrev);
	}

	void update(){
		unsigned int realTileSize = std::min(_size.x, _size.y) / 40;	// 40 = maximum tiles for a room
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
				switch (_mazmorra[_currentRoom][i][j]){
				case Sala::VACIO:
					tileNumber = RandomGen::getRandom(0, 1);
					break;
				case Sala::MURO:
					tileNumber = RandomGen::getRandom(3, 6);
					break;
				case Sala::COFRE:
					tileNumber = RandomGen::getRandom(7, 18);
					break;
				case Sala::ENEMIGO:
					tileNumber = 19;
					break;
				default:
					tileNumber = 2;
				}

				// find its position in the tileset texture
				int tu = tileNumber % (_texture.getSize().x / _TILESIZE);
				int tv = tileNumber / (_texture.getSize().x / _TILESIZE);

				// get a pointer to the current tile's quad
				sf::Vertex* quad = &m_vertices[(i + j * sala.getAncho()) * 4];

				// define its 4 corners
				quad[0].position = sf::Vector2f(i * realTileSize, j * realTileSize);
				quad[1].position = sf::Vector2f((i + 1) * realTileSize, j * realTileSize);
				quad[2].position = sf::Vector2f((i + 1) * realTileSize, (j + 1) * realTileSize);
				quad[3].position = sf::Vector2f(i * realTileSize, (j + 1) * realTileSize);

				// define its 4 texture coordinates
				quad[0].texCoords = sf::Vector2f(tu * _TILESIZE, tv * _TILESIZE);
				quad[1].texCoords = sf::Vector2f((tu + 1) * _TILESIZE, tv * _TILESIZE);
				quad[2].texCoords = sf::Vector2f((tu + 1) * _TILESIZE, (tv + 1) * _TILESIZE);
				quad[3].texCoords = sf::Vector2f(tu * _TILESIZE, (tv + 1) * _TILESIZE);
			}
		// Move all tiles from (0,0) to (10,30) in order to avoid the tabPane
		for (size_t i = 0; i < m_vertices.getVertexCount(); ++i){
			m_vertices[i].position += sf::Vector2f(10,30);
		}
		std::string info = "Sala " + std::to_string(_currentRoom) + " (id " + std::to_string(it->first) + ")\n";
		info += "Dim: (" + std::to_string(sala.getAncho()) + "x" + std::to_string(sala.getAlto()) + ")\n";
		info += "Enemigos: " + std::to_string(sala.getEnemigos()) + "\n";
		info += "Cofres: " + std::to_string(sala.getCofres()) + "\n";
		_roomInfo.setString(info);
	}

	sf::Vector2f _size;
	Cromosoma _model;
	Mazmorra _mazmorra;
	sf::Font _font;
	sf::Texture _texture;
	sf::VertexArray m_vertices;
	sf::Text _roomInfo;
	TextButton _botonPrev;
	TextButton _botonNext;
	
	unsigned int _currentRoom;

	const unsigned int _TILESIZE = 64;
};

#endif