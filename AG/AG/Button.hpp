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
	TextButton(sf::Vector2f pos, sf::Vector2f size, std::string text, sf::Color fillColor = sf::Color::Black) :
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
		m_text.setFont(m_font);
		m_text.setFillColor(invertColor(fillColor));
		m_text.setCharacterSize(20);
		m_text.setString(text);
		// La doble llamada es necesaria por temas de sfml y la posicion del texto
		m_text.setPosition(0, 0);
		m_text.setPosition(sf::Vector2f((pos.x + (size.x - m_text.getLocalBounds().width) / 2), (pos.y + (size.y - m_text.getLocalBounds().height) / 2)));
		

	}
private:

	sf::Color invertColor(sf::Color c){
		sf::Color ret;
		ret.r = 255 - c.r;
		ret.g = 255 - c.g;
		ret.b = 255 - c.b;
		return ret;
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(m_rect);
		target.draw(m_text, states);
	}
	sf::Font m_font;
	sf::Text m_text;
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