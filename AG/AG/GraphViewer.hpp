#ifndef GRAPHVIEWER_HPP
#define GRAPHVIEWER_HPP

#include <SFML\Graphics.hpp>
#include <queue>
#include "Cromosoma.hpp"

class GraphViewer : public sf::Rect<float>, public sf::Drawable, public sf::Transformable{
public:
	GraphViewer(sf::Vector2f pos, sf::Vector2f size) :
		sf::Rect<float>(pos, size)
	{
		_pos = pos;
		_size = size;
		_font.loadFromFile("arial.ttf");

		_gridSize = 50;
		_cols = (int)_size.x / _gridSize;
		_rows = (int)_size.y / _gridSize;
		_virtualGrid = new bool*[_cols];
		for (size_t i = 0; i < _cols; ++i){
			_virtualGrid[i] = new bool[_rows];
		}

		std::vector<sf::Color> colors = { sf::Color::Green, sf::Color::Red, sf::Color(255, 128, 0) };
		for (size_t i = 0; i < colors.size(); ++i){
			ROL_COLORS.push_back(colors[i]);
		}

		_drawableLinks.setPrimitiveType(sf::Lines);
		clearGrid();
	}

	GraphViewer(sf::Vector2f pos, sf::Vector2f size, Cromosoma model)
	{
		GraphViewer(pos, size);
		update();
	}

	void setModel(Cromosoma model){
		_model = model;
		_model.setIndexElMejor(model.getIndexElMejor());	// This attribute doesn't copy on asignment
		_mejorCC = model.getMejorCC().getNodos();
		update();
	}

	Cromosoma getModel() const{
		return _model;
	}


private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::VertexArray grid;
		grid.setPrimitiveType(sf::Lines);
		for (size_t i = 0; i <= _cols; ++i){
			grid.append(sf::Vertex(_pos+sf::Vector2f(_gridSize*i, 25), sf::Color(200, 200, 200)));
			grid.append(sf::Vertex(_pos + sf::Vector2f(_gridSize*i, this->getPosition().y + _gridSize*_rows), sf::Color(200, 200, 200)));
		}
		for (size_t i = 0; i < _rows; ++i){
			grid.append(sf::Vertex(_pos + sf::Vector2f(this->getPosition().x, 25 + _gridSize*i), sf::Color(200, 200, 200)));
			grid.append(sf::Vertex(_pos + sf::Vector2f(this->getPosition().x + _gridSize*_cols, 25 + _gridSize*i), sf::Color(200, 200, 200)));
		}
		target.draw(grid, states);

		for (sf::RectangleShape r : _drawableNodes){
			target.draw(r, states);
		}
		target.draw(_drawableLinks, states);
		for (sf::Text t : _drawableIDs){
			target.draw(t, states);
		}	
		for (sf::Text t : _drawableSizes){
			target.draw(t, states);
		}
		for (sf::Text t : _drawableEnemies){
			target.draw(t, states);
		}
		for (sf::Text t : _drawableLoots){
			target.draw(t, states);
		}
	}

	void update(){
		clearGrid();
		Grafo<Gen> g = _model.getGenotipo();
		_drawableLinks.clear();
		_drawableNodes.clear();
		_drawableIDs.clear();
		_drawableSizes.clear();
		_drawableEnemies.clear();
		_drawableLoots.clear();

		std::set<unsigned int> drawn;
		auto nodos = g.getNodos();
		auto it = nodos.begin();
		while (it != nodos.cend()){
			if (drawn.find(it->first) == drawn.cend()){
				// the node hasn't been drawn yet
				// drawNode(g, it->first, drawn, findFreePos());
				drawNode(g, it->first, drawn, findNearestPositionTo(sf::Vector2i(_cols / 2, _rows / 2)), _model.getRolesSala());
			}
			it++;
		}
		for (size_t i = 0; i < _drawableNodes.size(); ++i){
			_drawableNodes[i].move(0, 25);
			_drawableNodes[i].setOutlineThickness(2);
			_drawableNodes[i].move(_pos);
		}
		for (size_t i = 0; i < _drawableIDs.size(); ++i){
			_drawableIDs[i].move(0, 25);
			_drawableIDs[i].move(_pos);
			_drawableEnemies[i].move(_gridSize - 12, 0);
			_drawableEnemies[i].move(_pos);
			_drawableLoots[i].move(_gridSize - 5, 0);
			_drawableLoots[i].move(_pos);
		}
		for (size_t i = 0; i < _drawableLinks.getVertexCount(); ++i){
			_drawableLinks[i].position.y += _pos.y + 25;
			_drawableLinks[i].position.x += _pos.x;
		}
		for (size_t i = 0; i < _drawableSizes.size(); ++i){
			_drawableSizes[i].move(_pos);
		}

	}

	void drawNode(Grafo<Gen> g, unsigned int idNodo, std::set<unsigned int> &drawn, sf::Vector2i parentCoord, std::vector<unsigned int> roles){
		drawn.insert(idNodo);
		Gen nodo = g.getNodos()[idNodo];
		sf::RectangleShape n;
		sf::Vector2i myCoords = findNearestPositionTo(parentCoord);
		if (myCoords != sf::Vector2i(-1,-1)){
			_virtualGrid[myCoords.x][myCoords.y] = true;
			sf::Vector2f mySize(g.getNodos()[idNodo].getAncho(), g.getNodos()[idNodo].getAlto());
			sf::Vector2f myPos((myCoords.x * _gridSize) + ((_gridSize - mySize.x) / 2), (myCoords.y * _gridSize) + ((_gridSize - mySize.y) / 2));
			n.setPosition(myPos);
			n.setSize(mySize);
			if (_mejorCC.find(idNodo) != _mejorCC.cend()){
				n.setOutlineColor(sf::Color::Cyan);
			}
			else{
				n.setOutlineColor(sf::Color::Blue);
			}
			for (size_t i = 0; i < roles.size(); ++i){
				if (idNodo == roles[i]){
					n.setOutlineColor(ROL_COLORS[i]);
				}
			}
			n.setOutlineThickness(idNodo);			
			// ^ Por evitar mucho cambio, voy a usar este parametro para identificar cada nodo en _drawableNodes
			// Luego se cambia al terminar
			n.setFillColor(sf::Color::Transparent);
			_drawableNodes.push_back(n);
			
			sf::Vector2f myGridPos((myCoords.x * _gridSize), (myCoords.y * _gridSize) + (_gridSize/2));
			sf::Vector2f myMidPoint((myCoords.x * _gridSize) + (_gridSize / 2), (myCoords.y * _gridSize) + (_gridSize / 2));
			sf::Vector2f myParentPos(parentCoord.x * _gridSize, parentCoord.y * _gridSize);
			sf::Vector2f myParentMidPoint(myParentPos.x + (_gridSize / 2), myParentPos.y + (_gridSize / 2));
			sf::Vertex v1(myMidPoint, sf::Color::Magenta);
			sf::Vertex v2(myParentMidPoint, sf::Color::Magenta);
			_drawableLinks.append(v1);
			_drawableLinks.append(v2);

			sf::Text t(std::to_string(idNodo), _font);
			t.setCharacterSize(10);
			t.setFillColor(sf::Color::Black);
			t.setPosition(myMidPoint);
			_drawableIDs.push_back(t);

			sf::Text t4(("(" + std::to_string(nodo.getAncho()) + "x" + std::to_string(nodo.getAlto()) + ")"), _font);
			t4.setCharacterSize(10);
			t4.setFillColor(sf::Color::Blue);
			t4.setPosition(myGridPos);
			_drawableSizes.push_back(t4);

			sf::Text t2(std::to_string(nodo.getEnemigos()), _font);
			t2.setCharacterSize(10);
			t2.setFillColor(sf::Color::Red);
			t2.setPosition(myGridPos);
			_drawableEnemies.push_back(t2);

			sf::Text t3(std::to_string(nodo.getCofres()), _font);
			t3.setCharacterSize(10);
			t3.setFillColor(sf::Color(34, 177, 76));
			t3.setPosition(myGridPos);
			_drawableLoots.push_back(t3);

			auto ady = g.getAdyacencia()[idNodo];
			auto it = ady.begin();

			while (it != ady.cend()){
				if (drawn.find(*it) == drawn.cend()){
					drawNode(g, *it,drawn, myCoords, roles);
				}
				else{
					for (size_t i = 0; i < _drawableNodes.size(); ++i){
						if (_drawableNodes[i].getOutlineThickness() == *it){
							sf::RectangleShape r = _drawableNodes[i];
							sf::Vector2f myNeighborMidPoint(r.getPosition().x + (r.getSize().x / 2), r.getPosition().y + (r.getSize().y / 2));
							sf::Vertex v1(myMidPoint, sf::Color::Magenta);
							sf::Vertex v2(myNeighborMidPoint, sf::Color::Magenta);
							_drawableLinks.append(v1);
							_drawableLinks.append(v2);
							break;
						}
					}
				}
				it++;
			}
		}
	}

	sf::Vector2i findNearestPositionTo(sf::Vector2i coord){
		if (coord.x >= 0 && coord.x < _cols && coord.y >= 0 && coord.y < _rows){
			std::queue<sf::Vector2i> q;
			sf::Vector2i sol;
			q.push(coord);
			while (!q.empty()){
				sol = q.front();
				q.pop();
				if (!_virtualGrid[sol.x][sol.y]){
					return sol;
				}
				else{
					std::vector<sf::Vector2i> offsets;
					if (sol.y > 0)			offsets.push_back(sf::Vector2i(0, -1));		// North
					if (sol.x < _cols - 1)	offsets.push_back(sf::Vector2i(+1, 0));		// East
					if (sol.y < _rows - 1)	offsets.push_back(sf::Vector2i(0, +1));		// South
					if (sol.x > 0)			offsets.push_back(sf::Vector2i(-1, 0));		// West
					std::random_shuffle(offsets.begin(), offsets.end());
					for (size_t i = 0; i < offsets.size(); ++i){
						q.push(sf::Vector2i(sol.x + offsets[i].x, sol.y + offsets[i].y));
					}
				}
			}
		}
		return sf::Vector2i(-1, -1);
	}

	sf::Vector2i findFreePos(){
		for (size_t i = 0; i < _cols; ++i){
			for (size_t j = 0; j < _rows; ++j){
				if (!_virtualGrid[i][j]){
					return sf::Vector2i(i, j);
				}
			}
		}
		return sf::Vector2i(-1, -1);
	}

	void clearGrid(){
		for (size_t i = 0; i < _cols; ++i){
			for (size_t j = 0; j < _rows; ++j){
				_virtualGrid[i][j] = false;
			}
		}
	}


	sf::Vector2f _pos;
	sf::Vector2f _size;
	Cromosoma _model;
	std::unordered_map<unsigned int, Gen> _mejorCC;

	bool** _virtualGrid;
	unsigned int _gridSize = 50;
	unsigned int _cols;
	unsigned int _rows;
	std::vector<sf::RectangleShape> _drawableNodes;
	std::vector<sf::Text> _drawableIDs;
	std::vector<sf::Text> _drawableSizes;
	std::vector<sf::Text> _drawableEnemies;
	std::vector<sf::Text> _drawableLoots;
	sf::Font _font;
	sf::VertexArray _drawableLinks;

	std::vector<sf::Color> ROL_COLORS;
};

#endif