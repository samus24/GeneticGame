#ifndef SIMULATIONVIEWER_HPP
#define SIMULATIONVIEWER_HPP

#include <SFML\Graphics.hpp>
#include <queue>
#include "Cromosoma.hpp"
#include "Button.hpp"
#include "ICromosomaObserver.hpp"
#include "TreeViewer.hpp"

class Leyenda : public sf::Drawable, public sf::Transformable{
public:
	Leyenda(sf::Vector2f pos){
		_font.loadFromFile("arial.ttf");
		_pos = pos;
		int realTileSize = 20;
		int space = 5;
		std::vector<sf::Color> colores = {
			sf::Color(127, 127, 127),	// Vacio
			sf::Color::Cyan,			// Muro
			sf::Color::Yellow,			// Cofre
			sf::Color::Red,				// Enemigo (IA)
			sf::Color::Green,			// Jugador
			sf::Color(0, 0, 255, 127),	// Explorado
			sf::Color(255, 0, 0, 127),	// Andado (en patrulla)
			sf::Color(255, 0, 255, 127), // Explorado y andado
			sf::Color(255, 128, 0, 127), // Andado ataque
			sf::Color(174, 108, 72, 127) //Explorado, andado y andado
		};
		std::vector<std::string> cadenas = {
			"Vacio",
			"Muro",
			"Cofre",
			"Enemigo (IA)",
			"Jugador",
			"Explorado",
			"Andado (patrulla)",
			"Expl. y andado",
			"Andado (ataque)",
			"Expl, andado y andado"
		};
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(colores.size() * 4);
		
		for (size_t i = 0; i < colores.size(); ++i){
			sf::Text texto;
			sf::Vertex* quad = &m_vertices[i * 4];

			quad[0].position = sf::Vector2f(pos.x, pos.y + (i*(realTileSize + space)));
			quad[1].position = sf::Vector2f(pos.x + realTileSize, pos.y + (i*(realTileSize + space)));
			quad[2].position = sf::Vector2f(pos.x + realTileSize, pos.y + (i*(realTileSize + space) + realTileSize));
			quad[3].position = sf::Vector2f(pos.x, pos.y + (i*(realTileSize + space) + realTileSize));

			// define its 4 texture coordinates
			quad[0].color = colores[i];
			quad[1].color = colores[i];
			quad[2].color = colores[i];
			quad[3].color = colores[i];

			texto.setFont(_font);
			texto.setString(cadenas[i]);
			texto.setPosition(pos.x, pos.y + (i*(realTileSize + space)));
			texto.setFillColor(sf::Color::Black);
			texto.setCharacterSize(13);
			float hueco = texto.getLocalBounds().width + space;
			texto.move(-hueco, 0);
			_textos.push_back(texto);
		}
	}

	sf::VertexArray m_vertices;
	sf::Font _font;
	std::vector<sf::Text> _textos;
	sf::Vector2f _pos;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		sf::RectangleShape r;
		r.setPosition(_pos);
		sf::Vector2f size;
		size.x = 20;
		size.y = 25*8;
		r.setSize(size);
		r.setFillColor(sf::Color::White);
		target.draw(r, states);
		target.draw(m_vertices, states);
		for (auto it = _textos.begin(); it != _textos.end(); ++it)
			target.draw(*it, states);
	}
};

class SimulationViewer : public sf::Rect<float>, public sf::Drawable, public sf::Transformable, public ICromosomaObserver{
public:
	SimulationViewer(sf::Vector2f pos, sf::Vector2f size) :
		sf::Rect<float>(pos, size),
		_leyenda(sf::Vector2f(pos.x + size.x + 10, pos.y + (0.6 * size.y)))
	{
		_size = size;
		_font.loadFromFile("arial.ttf");
		_simulationInfo.setCharacterSize(12);
		_simulationInfo.setFillColor(sf::Color::Black);
		_simulationInfo.setFont(_font);
		_simulationInfo.setPosition(this->getPosition());
		_simulationInfo.move(10, size.y-100);

		_lastFitness.setFont(_font);
		_lastFitness.setCharacterSize(15);
		_lastFitness.setFillColor(sf::Color::Red);
		_lastFitness.setPosition(this->getPosition());
		_lastFitness.move(10, size.y-120);

		_actualFitness.setFont(_font);
		_actualFitness.setCharacterSize(15);
		_actualFitness.setFillColor(sf::Color::Red);
		_actualFitness.setPosition(this->getPosition());
		_actualFitness.move(10, size.y - 140);

		_playerFacing.setPrimitiveType(sf::Triangles);
		_playerFacing.resize(3);
		_playerFacing[0].color = sf::Color(0, 200, 0);
		_playerFacing[1].color = sf::Color(0, 200, 0);
		_playerFacing[2].color = sf::Color(0, 200, 0);

		_playerHealth.setFont(_font);
		_playerHealth.setCharacterSize(14);
		_playerHealth.setFillColor(sf::Color::Black);
		_playerHealth.setStyle(sf::Text::Bold);

		_enemyFacing.setPrimitiveType(sf::Triangles);
		_enemyFacing.resize(3);
		_enemyFacing[0].color = sf::Color(200, 0, 0);
		_enemyFacing[1].color = sf::Color(200, 0, 0);
		_enemyFacing[2].color = sf::Color(200, 0, 0);

		_enemyHealth.setFont(_font);
		_enemyHealth.setCharacterSize(14);
		_enemyHealth.setFillColor(sf::Color::Black);
		_enemyHealth.setStyle(sf::Text::Bold);

		_enemyState.setFillColor(sf::Color::Transparent);
		_enemyState.setOutlineThickness(3);

		_selected = false;
	}

	void handleClick(sf::Vector2f point){
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();
		/*
		sf::RectangleShape r;
		r.setPosition(sf::Vector2f(0, 30));
		sf::Vector2f size;
		size.x = _size.x + 10;
		size.y = _size.y;
		r.setSize(size);
		r.setFillColor(sf::Color::White);
		target.draw(r, states);

		sf::RectangleShape r2;
		r2.setPosition(sf::Vector2f(0, _size.y - 110));
		size.x = _size.x*2;
		size.y = 110;
		r2.setSize(size);
		r2.setFillColor(sf::Color::White);
		target.draw(r2, states);*/

		// draw the vertex array
		target.draw(m_vertices, states);
		target.draw(_leyenda, states);

		target.draw(_playerFacing, states);
		target.draw(_enemyFacing, states);

		target.draw(_playerHealth, states);
		target.draw(_enemyHealth, states);

		target.draw(_enemyState, states);
		
		target.draw(_simulationInfo);
		target.draw(_lastFitness);
		target.draw(_actualFitness);
	}
	void onSimulacionIniciada(const Cromosoma* c){
		
	}

	void onTurno(const Cromosoma* c, npc jugador, npc enemigo, Mapa m, Mapa explorado, Mapa andado, Mapa andadoAtaque){
		sf::Vector2f origin(10, 30);
		unsigned int realTileSize = std::min(_size.x, _size.y) / 40;
		Arbol arbPatrulla = c->getGenotipo(0);
		Arbol arbAtaque = c->getGenotipo(1);
		_playerHealth.setString(std::to_string(3 - jugador.heridas));
		_enemyHealth.setString(std::to_string(3 - enemigo.heridas));
		if ((m.getWidth()*m.getHeight() * 4) != m_vertices.getVertexCount()){
			m_vertices.clear();
			m_vertices.setPrimitiveType(sf::Quads);
			m_vertices.resize(m.getWidth()*m.getHeight() * 4);
		}
		for (unsigned int i = 0; i < m.getWidth(); ++i)
			for (unsigned int j = 0; j < m.getHeight(); ++j)
			{
				// get the current tile number
				sf::Color c;
				switch (m.getCasilla(i,j)){
				case Mapa::VACIO:
					c = sf::Color(127, 127, 127);
					break;
				case Mapa::MURO:
					c = sf::Color::Cyan;
					break;
				case Mapa::COFRE:
					c = sf::Color::Yellow;
					break;
				default:
					break;
				}

				if (enemigo.estaEn(i, j)){
					c = sf::Color::Red;
					switch (enemigo.f){
					case NORTE:
						_enemyFacing[0].position = origin + sf::Vector2f((i + 0.5) * realTileSize, j * realTileSize);
						_enemyFacing[1].position = origin + sf::Vector2f(i * realTileSize, (j + 1) * realTileSize);
						_enemyFacing[2].position = origin + sf::Vector2f((i + 1) * realTileSize, (j + 1) * realTileSize);
						break;
					case SUR:
						_enemyFacing[0].position = origin + sf::Vector2f((i + 0.5) * realTileSize, (j + 1) * realTileSize);
						_enemyFacing[1].position = origin + sf::Vector2f(i * realTileSize, j * realTileSize);
						_enemyFacing[2].position = origin + sf::Vector2f((i + 1) * realTileSize, j * realTileSize);
						break;
					case ESTE:
						_enemyFacing[0].position = origin + sf::Vector2f((i + 1) * realTileSize, (j + 0.5) * realTileSize);
						_enemyFacing[1].position = origin + sf::Vector2f(i * realTileSize, j * realTileSize);
						_enemyFacing[2].position = origin + sf::Vector2f(i * realTileSize, (j + 1) * realTileSize);
						break;
					case OESTE:
						_enemyFacing[0].position = origin + sf::Vector2f(i * realTileSize, (j + 0.5) * realTileSize);
						_enemyFacing[1].position = origin + sf::Vector2f((i + 1) * realTileSize, j * realTileSize);
						_enemyFacing[2].position = origin + sf::Vector2f((i + 1) * realTileSize, (j + 1) * realTileSize);
						break;
					}
					_enemyState.setSize(sf::Vector2f(realTileSize, realTileSize));
					_enemyState.setPosition(origin + sf::Vector2f(i * realTileSize, j * realTileSize));
					if (enemigo.turnosPatrulla != -1){
						_enemyState.setOutlineColor(sf::Color(200, 0, 0));
					}
					else{
						_enemyState.setOutlineColor(sf::Color(0, 0, 250));
					}
					_enemyHealth.setPosition(origin + sf::Vector2f((i + 0.4)*realTileSize, (j - 1)*realTileSize));
				}
				else if (jugador.estaEn(i, j)){
					c = sf::Color::Green;
					switch (jugador.f){
					case NORTE:
						_playerFacing[0].position = origin + sf::Vector2f((i + 0.5) * realTileSize, j * realTileSize);
						_playerFacing[1].position = origin + sf::Vector2f(i * realTileSize, (j + 1) * realTileSize);
						_playerFacing[2].position = origin + sf::Vector2f((i + 1) * realTileSize, (j + 1) * realTileSize);
						break;
					case SUR:
						_playerFacing[0].position = origin + sf::Vector2f((i + 0.5) * realTileSize, (j + 1) * realTileSize);
						_playerFacing[1].position = origin + sf::Vector2f(i * realTileSize, j * realTileSize);
						_playerFacing[2].position = origin + sf::Vector2f((i + 1) * realTileSize, j * realTileSize);
						break;
					case ESTE:
						_playerFacing[0].position = origin + sf::Vector2f((i + 1) * realTileSize, (j + 0.5) * realTileSize);
						_playerFacing[1].position = origin + sf::Vector2f(i * realTileSize, j * realTileSize);
						_playerFacing[2].position = origin + sf::Vector2f(i * realTileSize, (j + 1) * realTileSize);
						break;
					case OESTE:
						_playerFacing[0].position = origin + sf::Vector2f(i * realTileSize, (j + 0.5) * realTileSize);
						_playerFacing[1].position = origin + sf::Vector2f((i + 1) * realTileSize, j * realTileSize);
						_playerFacing[2].position = origin + sf::Vector2f((i + 1) * realTileSize, (j + 1) * realTileSize);
						break;
					}
					_playerHealth.setPosition(origin + sf::Vector2f((i + 0.4)*realTileSize, (j - 1)*realTileSize));
				}
				else if ((explorado.getCasilla(i, j) > 0) && (andado.getCasilla(i, j) > 0) && (andadoAtaque.getCasilla(i, j) > 0)){
					c = sf::Color(174, 108, 72);
					c.a = 127;
				}
				else if ((explorado.getCasilla(i, j) > 0) && (andado.getCasilla(i, j) > 0)){
					c = sf::Color::Magenta;
					c.a = 127;
				}
				else if (explorado.getCasilla(i, j) > 0){
					c = sf::Color::Blue;
					c.a = 127;
				}
				else if (andado.getCasilla(i, j) > 0){
					c = sf::Color::Red;
					c.a = 127;
				}
				else if (andadoAtaque.getCasilla(i, j) > 0){
					c = sf::Color(255,128,0);
					c.a = 127;
				}

				// get a pointer to the current tile's quad
				sf::Vertex* quad = &m_vertices[(i + j * m.getWidth()) * 4];

				// define its 4 corners
				quad[0].position = origin + sf::Vector2f(i * realTileSize, j * realTileSize);
				quad[1].position = origin + sf::Vector2f((i + 1) * realTileSize, j * realTileSize);
				quad[2].position = origin + sf::Vector2f((i + 1) * realTileSize, (j + 1) * realTileSize);
				quad[3].position = origin + sf::Vector2f(i * realTileSize, (j + 1) * realTileSize);

				// define its 4 texture coordinates
				quad[0].color = c;
				quad[1].color = c;
				quad[2].color = c;
				quad[3].color = c;
			}		
		std::string info = "Estado: ";
		if (enemigo.turnosPatrulla > 0){
			info += "Ataque";
		}
		else{
			info += "Patrulla";
		}
		info += "\n";
		info += "Arbol patrulla:\n";
		info += arbPatrulla.toString();
		info += "\nArbol ataque:\n";
		info += arbAtaque.toString();
		info += "\n<Enemigo> Tur: " + std::to_string(enemigo.turnos) + " G: " + std::to_string(enemigo.golpes) + " GE: " + std::to_string(enemigo.golpesEvitados) + " H: " + std::to_string(enemigo.heridas) + "\n";
		info += "<Jugador> H: " + std::to_string(jugador.heridas) + " B: " + std::to_string(jugador.bloqueando) + "\n";

		_simulationInfo.setString(info);
	}

	void onMapaTerminado(double fitness){
		std::string s = "Fitness en el ultimo mapa: " + std::to_string(fitness);
		_actualFitness.setString(s);
	}

	void onSimulacionTerminada(const Cromosoma* c){
		std::string s = "Fitness del ultimo individuo: " + std::to_string(c->getAdaptacion());
		_lastFitness.setString(s);
	}

	void update(){
		
	}

	sf::Vector2f _size;
	sf::Font _font;
	sf::VertexArray m_vertices;
	sf::VertexArray _playerFacing;
	sf::Text _playerHealth;
	sf::VertexArray _enemyFacing;
	sf::Text _enemyHealth;
	sf::Text _simulationInfo;
	sf::Text _lastFitness;
	sf::Text _actualFitness;
	Leyenda _leyenda;
	sf::RectangleShape _enemyState;
	

	bool _selected;

	const unsigned int _TILESIZE = 64;
};

#endif