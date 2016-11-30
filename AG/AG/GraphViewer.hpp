#ifndef GRAPHVIEWER_HPP
#define GRAPHVIEWER_HPP

#include <SFML\Graphics.hpp>
#include "Cromosoma.hpp"

class GraphViewer : public sf::Rect<float>, public sf::Drawable, public sf::Transformable{
public:
	GraphViewer(sf::Vector2f pos, sf::Vector2f size) :
		sf::Rect<float>(pos, size)
	{
		_size = size;

		_gridSize = 50;
		_cols = (int)_size.x / _gridSize;
		_rows = (int)_size.y / _gridSize;
		_virtualGrid = new bool*[_cols];
		for (size_t i = 0; i < _cols; ++i){
			_virtualGrid[i] = new bool[_rows];
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
		update();
	}

	Cromosoma getModel() const{
		return _model;
	}


private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (sf::RectangleShape r : _drawableNodes){
			target.draw(r, states);
		}
		target.draw(_drawableLinks, states);
	}

	void update(){
		clearGrid();
		Grafo<Gen> g = _model.getGenotipo();
		_drawableLinks.clear();
		_drawableNodes.clear();

		std::set<unsigned int> drawn;
		auto nodos = g.getNodos();
		auto it = nodos.begin();
		while (it != nodos.cend()){
			if (drawn.find(it->first) == drawn.cend()){
				// the node hasn't benn drawn yet
				drawNode(g, it->first, drawn, findFreePos());
			}
			it++;
		}
		for (size_t i = 0; i < _drawableNodes.size(); ++i){
			_drawableNodes[i].move(0, 25);
		}
		for (size_t i = 0; i < _drawableLinks.getVertexCount(); ++i){
			_drawableLinks[i].position.y += 25;
		}
	}

	void drawNode(Grafo<Gen> g, unsigned int idNodo, std::set<unsigned int> &drawn, sf::Vector2i parentCoord){
		drawn.insert(idNodo);
		sf::RectangleShape n;
		sf::Vector2i myCoords = findNearestPositionTo(parentCoord);
		if (myCoords != parentCoord){
			_virtualGrid[myCoords.x][myCoords.y] = true;
			sf::Vector2f mySize(g.getNodos()[idNodo].getAncho(), g.getNodos()[idNodo].getAlto());
			sf::Vector2f myPos((myCoords.x * _gridSize) + (mySize.x / 2), (myCoords.y * _gridSize) + (mySize.y / 2));
			n.setPosition(myPos);
			n.setSize(mySize);
			n.setOutlineColor(sf::Color::Blue);
			n.setOutlineThickness(2);
			n.setFillColor(sf::Color::Transparent);
			_drawableNodes.push_back(n);
			sf::Vector2f myMidPoint(myPos.x + (_gridSize / 2), myPos.y + (_gridSize /2));
			sf::Vector2f myParentPos(parentCoord.x * _gridSize, parentCoord.y * _gridSize);
			sf::Vector2f myParentMidPoint(myParentPos.x + (_gridSize / 2), myParentPos.y + (_gridSize / 2));
			sf::Vertex v1(myMidPoint, sf::Color::Green);
			sf::Vertex v2(myParentMidPoint, sf::Color::Green);
			_drawableLinks.append(v1);
			_drawableLinks.append(v2);

			auto ady = g.getAdyacencia()[idNodo];
			auto it = ady.begin();
			while (it != ady.cend()){
				if (drawn.find(*it) == drawn.cend()){
					drawNode(g, *it, drawn, myCoords);
				}
				it++;
			}
		}
	}

	sf::Vector2i findNearestPositionTo(sf::Vector2i coord){
		if (coord.x < _cols && coord.y < _rows){
			///////////////////////////////////////////
			// First check for North - East - South - West position
			if (coord.y > 0 && !_virtualGrid[coord.x][coord.y - 1]){
				return sf::Vector2i(coord.x, coord.y - 1);
			}
			if (coord.x < _cols-1 &&  !_virtualGrid[coord.x + 1][coord.y]){
				return sf::Vector2i(coord.x + 1, coord.y);
			}
			if (coord.y < _rows - 1 && !_virtualGrid[coord.x][coord.y + 1]){
				return sf::Vector2i(coord.x, coord.y + 1);
			}
			if (coord.x > 0 && !_virtualGrid[coord.x - 1][coord.y]){
				return sf::Vector2i(coord.x - 1, coord.y);
			}
			//
			//////////////////////////////////////////
			//////////////////////////////////////////
			// Now checking for combinated ones
			if (coord.x < _cols - 1 && coord.y > 0 && !_virtualGrid[coord.x + 1][coord.y - 1]){
				return sf::Vector2i(coord.x + 1, coord.y - 1);
			}
			if (coord.x < _cols - 1 && coord.y < _rows - 1 && !_virtualGrid[coord.x + 1][coord.y + 1]){
				return sf::Vector2i(coord.x + 1, coord.y + 1);
			}
			if (coord.x > 0 && coord.y < _rows - 1 && !_virtualGrid[coord.x - 1][coord.y + 1]){
				return sf::Vector2i(coord.x - 1, coord.y + 1);
			}
			if (coord.x > 0 && coord.y > 0 && !_virtualGrid[coord.x - 1][coord.y - 1]){
				return sf::Vector2i(coord.x - 1, coord.y -1);
			}
		}
		return coord;
	}

	sf::Vector2i findFreePos(){
		for (size_t i = 0; i < _cols; ++i){
			for (size_t j = 0; j < _rows; ++j){
				if (!_virtualGrid[i][j]){
					return sf::Vector2i(i, j);
				}
			}
		}
	}

	void clearGrid(){
		for (size_t i = 0; i < _cols; ++i){
			for (size_t j = 0; j < _rows; ++j){
				_virtualGrid[i][j] = false;
			}
		}
	}


	sf::Vector2f _size;
	Cromosoma _model;

	bool** _virtualGrid;
	unsigned int _gridSize = 50;
	unsigned int _cols;
	unsigned int _rows;
	std::vector<sf::RectangleShape> _drawableNodes;
	sf::VertexArray _drawableLinks;
};

#endif