#ifndef NODE_HPP
#define NODE_HPP

#include "SFML\Graphics.hpp"

class Node : public sf::Drawable, public sf::Transformable{
public:
	Node(unsigned int id, sf::Vector2f size, sf::Vector2f pos){
		this->m_id = id;
		this->m_size = size;
		this->m_pos = pos;
		this->m_rect = sf::Rect<float>(pos, size);
		this->m_selected = false;
		this->m_marked = false;
	}


	unsigned int getId() const {
		return this->m_id;
	}

	void setId(unsigned int id){
		this->m_id = id;
	}

	const sf::Vector2f getSize() const{
		return this->m_size;
	}

	void setSize(sf::Vector2f size){
		this->m_size = size;
		this->m_rect.width = size.x;
		this->m_rect.height = size.y;
	}

	const sf::Vector2f getPos() const{
		return this->m_pos;
	}

	void setPos(sf::Vector2f pos){
		this->m_pos = pos;
		this->m_rect.left = pos.x;
		this->m_rect.top = pos.y;
	}

	void setSelected(bool selected) {
		m_selected = selected;
	}

	bool isSelected() const{
		return m_selected;
	}

	void setMarked(bool marked) {
		m_marked = marked;
	}

	bool isMarked() const{
		return m_marked;
	}

	// Toggle selected and return the final state
	bool toggleSelected() {
		m_selected = !m_selected;
		return m_selected;
	}

	bool toggleMarked() {
		m_marked = !m_marked;
		return m_marked;
	}

	const sf::Rect<float> getRect() const{
		return this->m_rect;
	}

	float getArea() const {
		return this->m_size.x * this->m_size.y;
	}

	sf::Vector2f getMiddlePoint() const{
		return sf::Vector2f(m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2);
	}

	bool contains(const sf::Vector2f &point){
		return this->m_rect.contains(point);
	}

	bool intersects(const Node& other) const{
		return this->m_rect.intersects(other.getRect());
	}

	/* Operators */

	bool operator<(const Node& other) const{
		return this->getArea() < other.getArea();
	}

	bool operator>(const Node& other) const{
		return this->getArea() > other.getArea();
	}

	bool operator<=(const Node& other) const{
		return this->getArea() <= other.getArea();
	}

	bool operator>=(const Node& other) const{
		return this->getArea() >= other.getArea();
	}

	bool operator==(const Node& other) const{
		return this->getArea() == other.getArea();
	}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		sf::RectangleShape r(this->m_size);
		r.setFillColor(sf::Color::Transparent);
		if (m_selected){
			r.setOutlineColor(sf::Color::Yellow);
		}
		else if (m_marked){
			r.setOutlineColor(sf::Color::Green);
		}
		else{
			r.setOutlineColor(sf::Color::Blue);
		}
		r.setOutlineThickness(2);
		r.setPosition(this->m_pos);
		target.draw(r, states);
		sf::Font f;
		f.loadFromFile("arial.ttf");
		sf::Text t(std::to_string(this->m_id), f);
		t.setCharacterSize(12);
		t.setPosition(m_pos);
		t.setFillColor(sf::Color::White);
		target.draw(t, states);
	}

	unsigned int m_id;
	sf::Vector2f m_size;
	sf::Vector2f m_pos;
	sf::Rect<float> m_rect;
	bool m_selected;
	bool m_marked;
};

#endif