#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <SFML\Graphics.hpp>
#include "ScrollBar.hpp"

/**
La clase Logger es un elemento SFML-like, es decir, es dibujable, transformable y, por motivos evidentes, es en esencia un rectangulo.
Que sea dibujable y transformable simplemente permite (y obliga) a implementar el metodo draw que indica como representar el Logger en
la ventana.
Que sea un rectangulo (hereda de sf::Rect) simplifica la forma de detectar que se ha hecho click dentro de él.
En otras partes del codigo (p.e. en Ventana) puede verse que se llama a _logger.contains(), este metodo no aparece en esta clase, porque
está heredado de sf::Rect.

En lo propio de la clase Logger, es necesario saber posicion y tamano para representarlo en la pantalla. Opcionalmente pueden establecerse
un titulo y el tamano de los caracteres. El funcionamiento basico del Logger es insertar nuevas strings en el para que las represente.
Es un Logger muy basico, asi que solo se pueden insertar strings o borrar todo el contenido.

El Logger cuenta con dos Scrollbar, una vertical y otra horizontal, pero solo se mostraran si es necesario.
*/

class Logger : public sf::Rect<float>, public sf::Drawable, public sf::Transformable{
public:
	Logger(sf::Vector2f pos, sf::Vector2f size, std::string title = "Log", int charSize = 15) :
		sf::Rect<float>(pos, size),
		_scrollH(sf::Vector2f(pos.x, pos.y + size.y - _SCROLLBARTHICKNESS), sf::Vector2f(size.x, _SCROLLBARTHICKNESS)),
		_scrollV(sf::Vector2f(pos.x + size.x - _SCROLLBARTHICKNESS, pos.y), sf::Vector2f(_SCROLLBARTHICKNESS, size.y))
	{
		unsigned int titleHeight = charSize * 1.3;	// Margen superior reservado para el titulo
		_scrollV = sf::ScrollBar(sf::Vector2f(pos.x + size.x - _SCROLLBARTHICKNESS, pos.y + titleHeight), sf::Vector2f(_SCROLLBARTHICKNESS, size.y - titleHeight - _SCROLLBARTHICKNESS));
		_scrollH = sf::ScrollBar(sf::Vector2f(pos.x, pos.y + size.y - _SCROLLBARTHICKNESS), sf::Vector2f(size.x, _SCROLLBARTHICKNESS));
		_pos = pos;
		_size = size;
		_title = title;
		// En esta clase no se permite la personalizacion de colores, el texto será negro sobre blanco
		_drawableRect.setPosition(pos.x, pos.y + titleHeight);
		_drawableRect.setSize(sf::Vector2f(size.x, size.y - titleHeight));
		_drawableRect.setFillColor(sf::Color::White);
		_drawableRect.setOutlineColor(sf::Color(127, 127, 127));
		_drawableRect.setOutlineThickness(2);
		_margin = 2;
		_font.loadFromFile("arial.ttf");
		// La division es entre (charSize/2) porque charSize es en pixeles, no el tamano de fuente real
		_charsPerLine = (size.x - 2 * _margin) / (charSize/2);
		// Analogamente aqui se multiplica por 1.1 porque charSize es tamano horizontal; Asi se muestra 1 linea más, de forma habitual
		_linesToShow = ((size.y - titleHeight) - 2 * _margin) / (charSize * 1.1);
		_charSize = charSize;
		clearLog();
	}

	/**
		Operacion de borrado del Logger
	*/
	void clearLog(){
		_log.clear();
		_showScrollBarH = false;
		_showScrollBarV = false;
		_scrollH.clearElements();
		_scrollV.clearElements();
		_indexOfShowingH = 0;
		_indexOfShowingV = 0;
		_sizeOfLargestString = 0;
	}

	/**
		Añadir una linea nueva al logger
		Se hacen comprobaciones para mostrar scrollbars si es necesario
	*/
	void append(std::string s){
		std::string aux = std::to_string(_log.size()) + ": ";	// Aux contendra el nº de linea y ": " ("1: ", por ejemplo)
		if (_SHOWLINENUMBERS){
			s = aux.append(s);
		}
		_log.push_back(s);
		_scrollV.addElement("");	// Se anade un elemento a la scrollbar vertical para que tenga en cuenta cuantas filas hay en el log
		// Se actualiza el tamano de la string mas larga
		if (s.size() > _sizeOfLargestString){
			_sizeOfLargestString = s.size();
			// Si la scrollbar horizontal no tiene suficientes elementos, se rellena
			while (_scrollH.getElements().size() < _sizeOfLargestString){
				_scrollH.addElement("");
			}
		}
		if (s.size() > _charsPerLine){
			_showScrollBarH = true;
			// Se reajustan las lineas a mostrar, ahora no se tienen los pixels de la scrollbar horizontal
			unsigned int titleHeight = _charSize * 1.3;
			_linesToShow = ((_size.y - titleHeight) - 2 * _margin - _SCROLLBARTHICKNESS) / (_charSize * 1.1);
		}
		if (_log.size() > _linesToShow){
			_showScrollBarV = true;
			// Se reajustan los caracteres a mostrar, ahora no se tienen los pixels de la scrollbar vertical
			_charsPerLine = (_size.x - 2 * _margin - _SCROLLBARTHICKNESS) / (_charSize / 2);
			// Por defecto, si se incluyen mas lineas de las que se pueden mostrar, se siguen mostrando las de mas abajo (mas recientes)
			// Es decir, que el scroll baja al insertar.
			_indexOfShowingV = _log.size() - _linesToShow;
			_scrollV.setSelectedIndex(_indexOfShowingV);	// Se coloca el cursor donde debe
		}
	}

	/**
		Se permite al Logger gestionar los eventos de click
	*/
	void handleClick(sf::Vector2f point){
		int i;
		if (_showScrollBarV){
			i = _scrollV.getIndexFromPointer(point);
			if (i != -1){
				_scrollV.setSelectedIndex(i);
				_indexOfShowingV = (i * _log.size()) / 100;
			}
		}
		if (_showScrollBarH){
			i = _scrollH.getIndexFromPointer(point);
			if (i != -1){
				_scrollH.setSelectedIndex(i);
				_indexOfShowingH = (i * _sizeOfLargestString) / 100;
				if (_indexOfShowingH > (_sizeOfLargestString - _charsPerLine)){
					_indexOfShowingH = (_sizeOfLargestString - _charsPerLine);
				}
			}
		}
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		unsigned int titleHeight = _charSize * 1.3;
		// virtualLog sera de manera efectiva la string que se representara por pantalla
		std::string virtualLog = "";	
		// drawableLog simplemente es el texto (SFML) con la string virtualLog
		sf::Text drawableLog;
		drawableLog.setFont(_font);
		drawableLog.setFillColor(sf::Color::Black);
		drawableLog.setCharacterSize(_charSize);
		drawableLog.setPosition(_pos.x + _margin, _pos.y + _margin + titleHeight);
		// Se recorren todas las lineas del log
		for (size_t i = 0; i < _log.size(); ++i){
			// Primero se comprueba si la linea esta dentro del rango de lineas a mostrar
			if (i >= _indexOfShowingV && i < _indexOfShowingV + _linesToShow){
				// Luego se selecciona el segmento de linea a mostrar y se añade al virtualLog
				virtualLog += _log[i].substr(_indexOfShowingH, _charsPerLine);	
				if (virtualLog.back() != '\n') virtualLog += '\n';	// todas las lineas deben terminar con \n
			}
		}
		drawableLog.setString(virtualLog);

		sf::Text title;
		title.setFont(_font);
		title.setFillColor(sf::Color::Black);
		title.setCharacterSize(_charSize);
		title.setPosition(_pos.x, _pos.y);
		title.setString(_title);

		target.draw(title);
		target.draw(_drawableRect);
		target.draw(drawableLog);
		if (_showScrollBarV){
			target.draw(_scrollV);
		}
		if (_showScrollBarH){
			target.draw(_scrollH);
		}
		
	}

	std::vector<std::string> _log;			// El log no es mas que un array de strings
	std::string _title;						// Cadena que representa el titulo del logger (por defecto "Log")
	sf::Font _font;							// Fuente para los caracteres
	bool _showScrollBarH;					// Indica si debe mostrarse la ScrollBar horizontal
	bool _showScrollBarV;					// Indica si debe mostrarse la ScrollBar vertical
	sf::ScrollBar _scrollH;					// ScrollBar Horizontal
	sf::ScrollBar _scrollV;					// ScrollBar Vertical
	unsigned int _margin;					// Margen a dejar entre los bordes de la caja y el texto
	unsigned int _charSize;					// Tamano de los caracteres
	unsigned int _charsPerLine;				// Numero de caracteres que pueden representarse en una linea
	unsigned int _linesToShow;				// Numero de lineas que pueden representarse en la caja
	unsigned int _indexOfShowingV;			// Indica a partir de qué linea se deben empezar a mostrar las lineas
	unsigned int _indexOfShowingH;			// Indica a partir de que caracter se debe mostrar cada linea
	unsigned int _sizeOfLargestString;		// Guarda el tamano de la string mas larga en el log.
	sf::Vector2f _pos;						// Posicion del logger (tambien se guarda de manera implicita al heredar de sf::Rect)
	sf::Vector2f _size;						// Tamano del logger (igualmente en sf::Rect, pero asi el codigo es mas limpio)
	sf::RectangleShape _drawableRect;		// Caja dibujable del logger (hará de fondo si es necesario)

	const bool _SHOWLINENUMBERS = true;				// Por defecto, se muestran numeros de linea	
	const unsigned int _SCROLLBARTHICKNESS = 20;	// Grosor de las ScrollBars
};

#endif
