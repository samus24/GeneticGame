#ifndef VENTANAPARAMETROS_HPP
#define VENTANAPARAMETROS_HPP

#include <SFML/Graphics.hpp>

#include "parametros.hpp"
#include "TextField.hpp"
#include "CheckButton.hpp"
#include "ComboBox.hpp"

class VentanaParametros : public sf::Drawable{
public:
	VentanaParametros(sf::Vector2f size) :
		_size(size),
		_locked(false),
		_tfPob(sf::Vector2f(0,0), sf::Vector2f(0,0)),
		_tfGen(sf::Vector2f(0, 0), sf::Vector2f(0, 0)),
		_tfProfMin(sf::Vector2f(0, 0), sf::Vector2f(0, 0)),
		_tfProfMax(sf::Vector2f(0, 0), sf::Vector2f(0, 0)),
		_cbElit(sf::Vector2f(0, 0), 15),
		_cbBloat(sf::Vector2f(0, 0), 15),
		_cbContr(sf::Vector2f(0, 0), 15),
		_tfProbCru(sf::Vector2f(0, 0), sf::Vector2f(0, 0)),
		_tfProbMut(sf::Vector2f(0, 0), sf::Vector2f(0, 0)),
		_cbSel(sf::Vector2f(0, 0), sf::Vector2f(0, 0)),
		_cbCru(sf::Vector2f(0, 0), sf::Vector2f(0, 0)),
		_cbMut(sf::Vector2f(0, 0), sf::Vector2f(0, 0))
	{
		_param.tamPob = 10;
		_param.iteraciones = 100;
		_param.minNodos = 2;
		_param.maxNodos = 5;
		_param.elitismo = false;
		_param.bloating = true;
		_param.contractividad = false;
		_param.probCruce = 0.65;
		_param.probMutacion = 0.25;
		_param.seleccion = new seleccionTorneo();
		_param.cruce = new cruceSimple();
		_param.mutacion = new mutacionCombinada();

		std::vector<std::string> sel = {
			"Estocastica",
			"Ruleta",
			"Torneo"
		};

		std::vector<std::string> cru = {
			"Simple"
		};

		std::vector<std::string> mut = {
			"Arbol",
			"Funcion",
			"Terminal",
			"Combinada"
		};

		std::vector<std::string> lab = {
			"Tamaño Pob",
			"Generaciones",
			"Prof. Minima",
			"Prof. Maxima",
			"Elitismo",
			"Bloating",
			"Contractividad",
			"Prob. Cruce",
			"Prob. Mutacion",
			"Met. Seleccion",
			"Met. Cruce",
			"Met. Mutacion"
		};
		sf::Vector2f textFieldsSize(100,20);

		_font.loadFromFile("arial.ttf");

		_tipText.setFont(_font);
		_tipText.setCharacterSize(15);
		_tipText.setFillColor(sf::Color::Black);
		_tipText.setPosition(0, size.y - 20);

		sf::Vector2f pos(5, 5);
		for (size_t i = 0; i < lab.size(); ++i){
			sf::Text t;
			t.setFont(_font);
			t.setCharacterSize(15);
			t.setFillColor(sf::Color::Black);
			t.setPosition(pos);
			t.setString(lab[i]);
			pos.y += textFieldsSize.y*1.5;
			_labels.push_back(t);
		}


		pos = sf::Vector2f(5 + 110, 5);
		_tfPob.setPosition(pos);
		_tfPob.setSize(textFieldsSize);
		_tfPob.setText(std::to_string(_param.tamPob));
		pos.y += textFieldsSize.y*1.5;
		_tfGen.setPosition(pos);
		_tfGen.setSize(textFieldsSize);
		_tfGen.setText(std::to_string(_param.iteraciones));
		pos.y += textFieldsSize.y*1.5;
		_tfProfMin.setPosition(pos);
		_tfProfMin.setSize(textFieldsSize);
		_tfProfMin.setText(std::to_string(_param.minNodos));
		pos.y += textFieldsSize.y*1.5;
		_tfProfMax.setPosition(pos);
		_tfProfMax.setSize(textFieldsSize);
		_tfProfMax.setText(std::to_string(_param.maxNodos));
		pos.y += textFieldsSize.y*1.5;
		_cbElit.setPosition(pos);
		if (_param.elitismo) _cbElit.toogleMarked();
		pos.y += textFieldsSize.y*1.5;
		_cbBloat.setPosition(pos);
		if (_param.bloating) _cbBloat.toogleMarked();
		pos.y += textFieldsSize.y*1.5;
		_cbContr.setPosition(pos);
		if (_param.contractividad) _cbContr.toogleMarked();
		pos.y += textFieldsSize.y*1.5;
		_tfProbCru.setPosition(pos);
		_tfProbCru.setSize(textFieldsSize);
		_tfProbCru.setText(std::to_string(_param.probCruce));
		pos.y += textFieldsSize.y*1.5;
		_tfProbMut.setPosition(pos);
		_tfProbMut.setSize(textFieldsSize);
		_tfProbMut.setText(std::to_string(_param.probMutacion));
		pos.y += textFieldsSize.y*1.5;
		_cbSel.setPosition(pos);
		_cbSel.setSize(textFieldsSize);
		for (std::string s : sel){
			_cbSel.addElement(s);
		}
		_cbSel.setSelectedIndex(0);
		pos.y += textFieldsSize.y*1.5;
		_cbCru.setPosition(pos);
		_cbCru.setSize(textFieldsSize);
		for (std::string s : cru){
			_cbCru.addElement(s);
		}
		_cbCru.setSelectedIndex(0);
		pos.y += textFieldsSize.y*1.5;
		_cbMut.setPosition(pos);
		_cbMut.setSize(textFieldsSize);
		for (std::string s : mut){
			_cbMut.addElement(s);
		}
		_cbMut.setSelectedIndex(0);
	}

	bool isLocked() const{
		return _locked;
	}

	void setLocked(bool l){
		_locked = l;
	}

	void handleClick(sf::Vector2f pos){
		if (!_locked){
			_tfPob.setSelected(_tfPob.contains(pos));
			_tfGen.setSelected(_tfGen.contains(pos));
			_tfProfMin.setSelected(_tfProfMin.contains(pos));
			_tfProfMax.setSelected(_tfProfMax.contains(pos));
			if (_cbElit.isPointInside(sf::Vector2i(pos))){
				_cbElit.toogleMarked();
				_param.elitismo = _cbElit.isMarked();
			}
			if (_cbBloat.isPointInside(sf::Vector2i(pos))){
				_cbBloat.toogleMarked();
				_param.elitismo = _cbBloat.isMarked();
			}
			if (_cbContr.isPointInside(sf::Vector2i(pos))){
				_cbContr.toogleMarked();
				_param.elitismo = _cbContr.isMarked();
			}
			_tfProbCru.setSelected(_tfProbCru.contains(pos));
			_tfProbMut.setSelected(_tfProbMut.contains(pos));
			_cbSel.setSelected(_cbSel.contains(pos));
			_cbCru.setSelected(_cbCru.contains(pos));
			_cbMut.setSelected(_cbMut.contains(pos));
			_tipText.setString("Pulsa Enter para establecer el parametro");
			if (_cbSel.isSelected() || _cbCru.isSelected() || _cbMut.isSelected())
				_tipText.setString("Usa las flechas para seleccionar");

		}
	}

	void handleKeyboardEvent(sf::Event ev){
		if (ev.type != sf::Event::KeyReleased)
			return;
		if (ev.key.code >= sf::Keyboard::Num0 && ev.key.code <= sf::Keyboard::Num9){
			std::string s;
			if (_tfPob.isSelected()){
				s = _tfPob.getText();
				s += std::to_string(ev.key.code-26);
				_param.tamPob = std::stoi(s);
				_tfPob.setText(std::to_string(_param.tamPob));
			}
			if (_tfGen.isSelected()){
				s = _tfGen.getText();
				s += std::to_string(ev.key.code-26);
				_param.iteraciones = std::stoi(s);
				_tfGen.setText(std::to_string(_param.iteraciones));
			}
			if (_tfProfMin.isSelected()){
				s = _tfProfMin.getText();
				s += std::to_string(ev.key.code-26);
				_param.minNodos = std::stoi(s);
				_tfProfMin.setText(std::to_string(_param.minNodos));
			}
			if (_tfProfMax.isSelected()){
				s = _tfProfMax.getText();
				s += std::to_string(ev.key.code-26);
				_param.maxNodos = std::stoi(s);
				_tfProfMax.setText(std::to_string(_param.maxNodos));
			}
			if (_tfProbCru.isSelected()){
				s = _tfProbCru.getText();
				s += std::to_string(ev.key.code-26);
				_param.probCruce = std::stoi(s);
				_tfProbCru.setText(std::to_string(_param.probCruce));
			}
			if (_tfProbMut.isSelected()){
				s = _tfProbMut.getText();
				s += std::to_string(ev.key.code-26);
				_param.probMutacion = std::stoi(s);
				_tfProbMut.setText(std::to_string(_param.probMutacion));
			}
			
		}
		else if (ev.key.code == sf::Keyboard::BackSpace){
			std::string s;
			if (_tfPob.isSelected()){
				s = _tfPob.getText();
				if(!s.empty()) s.pop_back();
				if (s.empty()) s = "0";
				_tfPob.setText(s);
				_param.tamPob = std::stoi(s);
			}
			if (_tfGen.isSelected()){
				s = _tfGen.getText();
				if(!s.empty()) s.pop_back();
				if (s.empty()) s = "0";
				_tfGen.setText(s);
				_param.iteraciones = std::stoi(s);
			}
			if (_tfProfMin.isSelected()){
				s = _tfProfMin.getText();
				if(!s.empty()) s.pop_back();
				if (s.empty()) s = "0";
				_tfProfMin.setText(s);
				_param.minNodos = std::stoi(s);
			}
			if (_tfProfMax.isSelected()){
				s = _tfProfMax.getText();
				if(!s.empty()) s.pop_back();
				if (s.empty()) s = "0";
				_tfProfMax.setText(s);
				_param.maxNodos = std::stoi(s);
			}
			if (_tfProbCru.isSelected()){
				s = _tfProbCru.getText();
				if(!s.empty()) s.pop_back();
				if (s.empty()) s = "0";
				_tfProbCru.setText(s);
				_param.probCruce = std::stoi(s);
			}
			if (_tfProbMut.isSelected()){
				s = _tfProbMut.getText();
				if(!s.empty()) s.pop_back();
				if (s.empty()) s = "0";
				_tfProbMut.setText(s);
				_param.probMutacion = std::stoi(s);
			}
		}
		else if (ev.key.code == sf::Keyboard::Up || ev.key.code == sf::Keyboard::Down){
			int incr = (ev.key.code == sf::Keyboard::Up) ? -1 : 1;
			int nuevoIndice;
			if (_cbSel.isSelected()){
				nuevoIndice = _cbSel.getMarkedIndex() + incr;
				if (nuevoIndice < 0){
					nuevoIndice = _cbSel.getSize() - 1;
				}
				else if (nuevoIndice >= _cbSel.getSize()){
					nuevoIndice = 0;
				}
				_cbSel.setMarkedIndex(nuevoIndice);
			}
			if (_cbCru.isSelected()){
				nuevoIndice = _cbCru.getMarkedIndex() + incr;
				if (nuevoIndice < 0){
					nuevoIndice = _cbCru.getSize() - 1;
				}
				else if (nuevoIndice >= _cbCru.getSize()){
					nuevoIndice = 0;
				}
				_cbCru.setMarkedIndex(nuevoIndice);
			}
			if (_cbMut.isSelected()){
				nuevoIndice = _cbMut.getMarkedIndex() + incr;
				if (nuevoIndice < 0){
					nuevoIndice = _cbMut.getSize() - 1;
				}
				else if (nuevoIndice >= _cbMut.getSize()){
					nuevoIndice = 0;
				}
				_cbMut.setMarkedIndex(nuevoIndice);
			}

		}
		else if (ev.key.code == sf::Keyboard::Escape || ev.key.code == sf::Keyboard::Return){
			_tipText.setString("");

			_tfPob.setSelected(false);
			_tfGen.setSelected(false);
			_tfProfMin.setSelected(false);
			_tfProfMax.setSelected(false);
			_tfProbCru.setSelected(false);
			_tfProbMut.setSelected(false);
			_cbSel.setSelected(false);
			_cbSel.setSelectedIndex(_cbSel.getMarkedIndex());
			switch (_cbSel.getSelectedIndex()){
			case 0:
				_param.seleccion = new seleccionEstocastica();
				break;
			case 1:
				_param.seleccion = new seleccionRuleta();
				break;
			case 2:
				_param.seleccion = new seleccionTorneo();
				break;
			default:
				_param.seleccion = new seleccionEstocastica();
				break;
			}
			_cbCru.setSelected(false);
			_cbCru.setSelectedIndex(_cbCru.getMarkedIndex());
			switch (_cbCru.getSelectedIndex()){
			case 0:
				_param.cruce = new cruceSimple();
				break;
			default:
				_param.cruce = new cruceSimple();
				break;
			}
			_cbMut.setSelected(false);
			_cbMut.setSelectedIndex(_cbMut.getMarkedIndex());
			switch (_cbMut.getSelectedIndex()){
			case 0:
				_param.mutacion = new mutacionArbol();
				break;
			case 1:
				_param.mutacion = new mutacionFuncion();
				break;
			case 2:
				_param.mutacion = new mutacionTerminal();
				break;
			case 3:
				_param.mutacion = new mutacionCombinada();
				break;
			default:
				_param.mutacion = new mutacionArbol();

			}
		}
	}

	Parametros getParametros(){
		return _param;
	}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (sf::Text t : _labels){
			target.draw(t, states);
		}
		target.draw(_tfPob, states);
		target.draw(_tfGen, states);
		target.draw(_tfProfMin, states);
		target.draw(_tfProfMax, states);
		target.draw(_cbElit, states);
		target.draw(_cbBloat, states);
		target.draw(_cbContr, states);
		target.draw(_tfProbCru, states);
		target.draw(_tfProbMut, states);
		target.draw(_cbSel, states);
		target.draw(_cbCru, states);
		target.draw(_cbMut, states);
		if (_cbSel.isSelected()) target.draw(_cbSel, states);
		if (_cbCru.isSelected()) target.draw(_cbCru, states);
		if (_cbMut.isSelected()) target.draw(_cbMut, states);
		target.draw(_tipText, states);
	}

	sf::Vector2f _size;
	Parametros _param;
	bool _locked;

	sf::Font _font;
	std::vector<sf::Text> _labels;

	TextField _tfPob;
	TextField _tfGen;
	TextField _tfProfMin;
	TextField _tfProfMax;
	sf::CheckButton _cbElit;
	sf::CheckButton _cbBloat;
	sf::CheckButton _cbContr;
	TextField _tfProbCru;
	TextField _tfProbMut;
	ComboBox _cbSel;
	ComboBox _cbCru;
	ComboBox _cbMut;

	sf::Text _tipText;


};

#endif