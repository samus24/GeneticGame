#pragma once
#include <string>
#include <SFML\Graphics.hpp>
#include "Components.hpp"

#define CURSOR_WIDTH 20

namespace sf{
	class ScrollBar
	{
	public:
		ScrollBar(sf::Vector2f position, sf::Vector2f size, std::vector<std::string> elements, sf::Orientation orientation);
		~ScrollBar();
	private:
		sf::Vector2f position;
		sf::Vector2f size;
		std::vector<std::string> elements;
		int selectedIndex;
		int steps;
		float stepSize;
		sf::RectangleShape box;
		sf::RectangleShape cursor;
		sf::Font f;
		sf::RectangleShape textBox;
		sf::Orientation orientation;
	public:
		std::string getSelectedElement();
		int getSelectedIndex();
		int getIndexFromPointer(sf::Vector2i pointer);
		void setSelectedIndex(int index);
		void draw(sf::RenderTarget* target);
	};
}


