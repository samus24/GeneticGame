#ifndef CHEST_HPP
#define CHEST_HPP

#include "PowerUp.hpp"
#include "Entity.hpp"
#include "Utils.hpp"

class Chest : public Entity {
public:
	Chest(const sf::Texture& texture) :
		Entity(texture),
		_opened(false)
	{

	}

	PowerUp open(){
		_opened = true;
		/*
		auto lastTextureRect = sprite.getTextureRect();
		lastTextureRect.left += TILESIZE.x;
		sprite.setTextureRect(lastTextureRect);*/
		return _boost;
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const{
		states.transform *= getTransform();
		target.draw(sprite);
	}

	PowerUp _boost;
	bool _opened;
};

#endif