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