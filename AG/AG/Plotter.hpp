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
		_font.loadFromFile("arial.ttf");
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

	void pushEjeY(std::vector<double> dataY, sf::Color color, std::string name){
		_datasY.push_back(dataY);
		_colorsY.push_back(color);
		_namesY.push_back(name);
	}

	void removeAllData(){
		_dataX.clear();
		removeDatasY();
	}

	void removeDatasY(){
		_datasY.clear();
		_colorsY.clear();
		_namesY.clear();
	}

	void removeDataY(unsigned int i){
		if (i < _datasY.size()){
			_datasY.erase(_datasY.begin() + i);
			_colorsY.erase(_colorsY.begin() + i);
			_namesY.erase(_namesY.begin() + i);
		}
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		/*
			Probar a cambiarlo, fijando el numero de divisiones por eje (en Java son 10 siempre)
			y en base a eso determinar los valores que se escriben en los ejes
		*/

		int margin = 50;
		int exceso = 15;
		states.transform *= getTransform();
		sf::VertexArray ejes;
		ejes.setPrimitiveType(sf::Lines);
		sf::Vertex topY(sf::Vector2f(_pos.x + margin, _pos.y + margin), sf::Color::Black);
		sf::Vertex bottomY(sf::Vector2f(_pos.x + margin, _pos.y + _size.y - margin), sf::Color::Black);
		sf::Vertex leftX(sf::Vector2f(_pos.x + margin, _pos.y + _size.y - margin), sf::Color::Black);
		sf::Vertex rightX(sf::Vector2f(_pos.x + _size.x - margin, _pos.y + _size.y - margin), sf::Color::Black);
		
		std::vector<sf::Text> datosEjeX;
		std::vector<sf::Text> datosEjeY;

		if ((_size.x - 2 * margin) > _dataX.size()){
			// Hay mas espacio que datos

			int mayor = -1;
			int actual;
			for (size_t i = 0; i < _datasY.size(); ++i){
				actual = _datasY[i].at(_datasY[i].size() - 1);
				if (actual > mayor){
					mayor = actual;
				}
			}

			double space = (_size.x - 2 * margin) / _dataX.size();
			double top = _pos.y + margin;
			double bot = _pos.y + _size.y - margin;
			double left = _pos.x + margin;
			double right = _pos.x + _size.x - margin;
			for (size_t i = 0; i < _dataX.size(); ++i){
				sf::Vertex a(sf::Vector2f(left + (i*space),top), sf::Color(200,200,200));
				sf::Vertex b(sf::Vector2f(left + (i*space), bot + exceso), sf::Color(200, 200, 200));
				ejes.append(a);
				ejes.append(b);
				if (bot - 3 - (i*space) > top){
					sf::Vertex c(sf::Vector2f(left - exceso, bot - (i*space)), sf::Color(200, 200, 200));
					sf::Vertex d(sf::Vector2f(right, bot - (i*space)), sf::Color(200, 200, 200));
					ejes.append(c);
					ejes.append(d);

					sf::Text t2(std::to_string(i * (_dataX.size() / mayor)), _font, 9);
					t2.setFillColor(sf::Color::Black);
					t2.setPosition(c.position);
					datosEjeY.push_back(t2);
				}
				
				sf::Text t(std::to_string(_dataX[i]), _font, 9);
				/* No hay manera de dejarlo en 2 (o 1) decimales, asi que trunco la cadena */
				std::string s = t.getString();
				size_t posPunto = s.find_first_of(".");
				s = s.substr(0, posPunto + 2);
				t.setString(s);
				/***********/
				t.setFillColor(sf::Color::Black);
				t.setPosition(b.position.x, b.position.y - exceso);
				datosEjeX.push_back(t);


				
			}

			



		}
		else{
			// Es necesario omitir datos para representarlos en pantalla

		}
		ejes.append(topY);
		ejes.append(bottomY);
		ejes.append(leftX);
		ejes.append(rightX);
		sf::Text textEjeX(_nombreX, _font, 15);
		textEjeX.setFillColor(sf::Color::Black);
		textEjeX.setPosition(_pos.x + (_size.x / 2), _pos.y + (_size.y - 25));
		sf::Text textEjeY(_nombreY, _font, 15);
		textEjeY.setFillColor(sf::Color::Black);
		textEjeY.setPosition(_pos.x + 3, _pos.y + (_size.y / 2));
		textEjeY.rotate(-90);
		target.draw(ejes, states);
		target.draw(textEjeX, states);
		target.draw(textEjeY, states);
		for (sf::Text t : datosEjeX)
			target.draw(t, states);
		for (sf::Text t : datosEjeY)
			target.draw(t, states);
	}

	sf::Vector2f _pos;
	sf::Vector2f _size;
	std::string _nombreX;
	std::string _nombreY;
	sf::Font _font;
	std::vector<double> _dataX;
	std::vector<std::vector<double>> _datasY;
	std::vector<sf::Color> _colorsY;
	std::vector<std::string> _namesY;
};

#endif