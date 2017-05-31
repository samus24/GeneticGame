#ifndef PORTAL_HPP
#define PORTAL_HPP

#include "Entity.hpp"
#include "Utils.hpp"

class Portal : public Entity {
public:
	Portal(const sf::Texture& texture, unsigned int room, sf::Vector2i coords) :
		Entity(texture),
		_linkToRoom(room),
		_linkCoords(coords)
	{

	}

	unsigned int getLinkToRoom() const{
		return _linkToRoom;
	}

	sf::Vector2i getLinkCoords() const{
		return _linkCoords;
	}
	

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const{
		states.transform *= getTransform();
		target.draw(sprite);
	}

	unsigned int _linkToRoom;
	sf::Vector2i _linkCoords;
};

#endif