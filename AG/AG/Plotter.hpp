#ifndef PLOTTER_HPP
#define PLOTTER_HPP

#include <vector>
#include <SFML\Graphics.hpp>
#include "Pair.hpp"

class Plotter : public sf::Rect<float>, public sf::Drawable, public sf::Transformable{
public:
	Plotter(sf::Vector2f pos, sf::Vector2f size) :
	_pos(pos),
	_size(size)
	{
		_nombreX = "Eje X";
		_nombreY = "Eje Y";
	}

	void setNombreEjeX(std::string nombre){
		_nombreX = nombre;
	}

	void setNombreEjeY(std::string nombre){
		_nombreY = nombre;
	}

	void setEjeX(std::vector<double> dataX){
		_dataX = dataX;
	}

	void pushEjeY(std::vector<double> dataY, sf::Color color){
		_datasY.push_back(dataY);
		_colorsY.push_back(color);
	}

	void removeAllData(){
		_dataX.clear();
		removeDatasY();
	}

	void removeDatasY(){
		_datasY.clear();
	}

	void removeDataY(unsigned int i){
		if (i < _datasY.size())
			_datasY.erase(_datasY.begin() + i);
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		int margin = 20;
		states.transform *= getTransform();
		sf::VertexArray va;
		va.setPrimitiveType(sf::Lines);
		sf::Vertex topY(sf::Vector2f(_pos.x + margin, _pos.y + margin), sf::Color::Black);
		sf::Vertex bottomY(sf::Vector2f(_pos.x + margin, _pos.y + _size.y - margin), sf::Color::Black);
		sf::Vertex leftX(sf::Vector2f(_pos.x + margin, _pos.y + _size.y - margin), sf::Color::Black);
		sf::Vertex rightX(sf::Vector2f(_pos.x + _size.x - margin, _pos.y + _size.y - margin), sf::Color::Black);

		if ((_size.x - 2 * margin) > _dataX.size()){
			// Tengo mas espacio que datos
			double space = (_size.x - 2 * margin) / _dataX.size();
			double top = _pos.y + margin;
			double bot = _pos.y + _size.y - margin + 3;
			double left = _pos.x + margin;
			double right = _pos.x + _size.x - margin;
			for (size_t i = 0; i < _dataX.size(); ++i){
				sf::Vertex a(sf::Vector2f(left + (i*space),top), sf::Color(200,200,200));
				sf::Vertex b(sf::Vector2f(left + (i*space), bot), sf::Color(200, 200, 200));
				sf::Vertex c(sf::Vector2f(left - 3, top + (i*space)), sf::Color(200, 200, 200));
				sf::Vertex d(sf::Vector2f(right + 3, top + (i*space)), sf::Color(200, 200, 200));
				va.append(a);
				va.append(b);
				va.append(c);
				va.append(d);
			}

		}
		else{
			// Es necesario omitir datos para representarlos en pantalla

		}
		va.append(topY);
		va.append(bottomY);
		va.append(leftX);
		va.append(rightX);
		target.draw(va, states);
	}

	sf::Vector2f _pos;
	sf::Vector2f _size;
	std::string _nombreX;
	std::string _nombreY;
	std::vector<double> _dataX;
	std::vector<std::vector<double>> _datasY;
	std::vector<sf::Color> _colorsY;
};

#endif