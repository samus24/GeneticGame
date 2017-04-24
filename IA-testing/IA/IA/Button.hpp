#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML\Graphics.hpp>

class Button : public sf::Rect<float>, public sf::Drawable, public sf::Transformable{
public:
	Button(sf::Vector2f pos, sf::Vector2f size) :
	sf::Rect<float>(pos, size){
		
	}

	sf::Vector2f getPos() const{
		return sf::Vector2f(this->left, this->top);
	}

	void setPos(sf::Vector2f pos){
		this->left = pos.x;
		this->top = pos.y;
	}

	sf::Vector2f getSize() const{
		return sf::Vector2f(this->width, this->height);
	}

	void setSize(sf::Vector2f size){
		this->width = size.x;
		this->height = size.y;
	}

private:

};

class TextButton : public Button{
public:
	TextButton(sf::Vector2f pos, sf::Vector2f size, std::string text, sf::Color fillColor = sf::Color::Black, unsigned int charSize = 20) :
		Button(pos, size),
		m_rect(size)
	{
		if (!m_font.loadFromFile("arial.ttf")){
			exit(1);
		}
		m_rect.setPosition(pos);
		m_rect.setFillColor(fillColor);
		m_rect.setOutlineThickness(2);
		m_rect.setOutlineColor(invertColor(fillColor));
		m_fillColor = fillColor;
		m_text = text;
		_charSize = charSize;
	}

	std::string getString(){
		return this->getString();
	}

	void setString(std::string s){
		m_text = s;
	}

	sf::Color getColor(){
		return m_fillColor;
	}	

	void setColor(sf::Color c){
		m_fillColor = c;
		m_rect.setFillColor(c);
		m_rect.setOutlineColor(invertColor(c));
	}
private:

	sf::Color invertColor(sf::Color c) const{
		sf::Color ret;
		ret.r = 255 - c.r;
		ret.g = 255 - c.g;
		ret.b = 255 - c.b;
		return ret;
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		sf::Text text;
		text.setFont(m_font);
		text.setFillColor(invertColor(m_fillColor));
		text.setCharacterSize(_charSize);
		text.setString(m_text);
		// La doble llamada es necesaria por temas de sfml y la posicion del texto
		text.setPosition(0, 0);
		text.setPosition(sf::Vector2f((this->getPos().x + (this->getSize().x - text.getLocalBounds().width) / 2), (this->getPos().y + (this->getSize().y - text.getLocalBounds().height) / 2)));
		target.draw(m_rect);
		target.draw(text, states);
	}
	sf::Font m_font;
	std::string m_text;
	unsigned int _charSize;
	sf::Color m_fillColor;
	sf::RectangleShape m_rect;
};


class ReLoadButton : public Button{
public:
	ReLoadButton(sf::Vector2f pos, sf::Vector2f size) :
		Button(pos, size){
		if (!m_texture.loadFromFile("reload.png")){
			exit(1);
		}


	}
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		sf::Sprite sp(m_texture);
		sp.setPosition(this->left, this->top);
		sp.scale((this->width) / 100, (this->height) / 100);
		target.draw(sp, states);
	}
	sf::Texture m_texture;
};

class ExportButton : public Button{
public:
	ExportButton(sf::Vector2f pos, sf::Vector2f size) :
		Button(pos, size){
		if (!m_texture.loadFromFile("export.png")){
			exit(1);
		}


	}
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		sf::Sprite sp(m_texture);
		sp.setPosition(this->left, this->top);
		sp.scale((this->width) / 100, (this->height) / 100);
		target.draw(sp, states);
	}
	sf::Texture m_texture;
};

class OrganizeButton : public Button{
public:
	OrganizeButton(sf::Vector2f pos, sf::Vector2f size) :
		Button(pos, size){
		if (!m_texture.loadFromFile("organize.png")){
			exit(1);
		}


	}
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		sf::Sprite sp(m_texture);
		sp.setPosition(this->left, this->top);
		sp.scale((this->width) / 100, (this->height) / 100);
		target.draw(sp, states);
	}
	sf::Texture m_texture;
};
#endif