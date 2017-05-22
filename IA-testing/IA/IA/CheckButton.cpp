#include "CheckButton.hpp"

namespace sf {
	CheckButton::CheckButton(sf::Vector2f position, unsigned int size) :
		position(position),
		box(sf::Vector2f(size, size)),
		insideBox(sf::Vector2f(size - INSIDE_MARGIN(size), size - INSIDE_MARGIN(size)))

	{
		marked = false;
		this->size = size;
		box.setFillColor(sf::Color::Transparent);
		box.setOutlineColor(sf::Color(127, 127, 127, 255));
		box.setOutlineThickness(2);
		box.setPosition(position);

		insideBox.setFillColor(sf::Color(63, 63, 63, 255));
		insideBox.setOutlineColor(sf::Color::Transparent);
		insideBox.setOutlineThickness(1);
		insideBox.setPosition(sf::Vector2f(position.x + INSIDE_MARGIN(size) / 2, position.y + INSIDE_MARGIN(size) / 2));
	}


	CheckButton::~CheckButton()
	{
	}


	bool CheckButton::isMarked()
	{
		return marked;
	}


	void CheckButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(box);
		target.draw(insideBox);
	}

	bool CheckButton::isPointInside(sf::Vector2i pointer){
		return (pointer.x > position.x) && (pointer.x < position.x + size) && (pointer.y > position.y) && (pointer.y < position.y + size);
	}
	void CheckButton::toogleMarked(){
		marked = !marked;
		if (marked){
			insideBox.setFillColor(sf::Color::Green);
		}
		else{
			insideBox.setFillColor(sf::Color(63, 63, 63, 255));
		}
	}

	void CheckButton::setPosition(sf::Vector2f pos){
		position = pos;
		box.setPosition(position);
		insideBox.setPosition(sf::Vector2f(position.x + INSIDE_MARGIN(size) / 2, position.y + INSIDE_MARGIN(size) / 2));
	}

}