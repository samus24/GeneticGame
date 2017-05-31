#ifndef GAME_HPP
#define GAME_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics.hpp>
#include <memory>
#include "StateStack.hpp"

#include "MenuState.hpp"


const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

class Game {
public:
	Game() :
		_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "DunGen"),
		_stateStack(State::Context(_window, _textures, _fonts))
	{
		_fonts.load(Fonts::Main, "Media/Sansation.ttf");
		_fonts.load(Fonts::Alagard, "Media/alagard.ttf");
		_fonts.load(Fonts::Pixel, "Media/pixel.ttf");

		_textures.load(Textures::TitleScreen, "Media/Textures/TitleScreen.png");
		_textures.load(Textures::Title, "Media/Textures/title.png");
		_textures.load(Textures::GUI, "Media/Textures/GUI.png");
		_textures.load(Textures::TileMap, "Media/Textures/TileMap.png");
		auto menuState = std::make_shared<MenuState>(_stateStack, State::Context(_window, _textures, _fonts));
		_stateStack.pushState(menuState);
	}

	void run(){
		sf::Clock clk;
		sf::Time timeSinceLastUpdate = sf::Time::Zero;
		while (_window.isOpen())
		{

			sf::Time dt = clk.restart();
			timeSinceLastUpdate += dt;
			while (timeSinceLastUpdate > TimePerFrame)
			{
				timeSinceLastUpdate -= TimePerFrame;

				processInput();
				update(TimePerFrame);

				// Check inside this loop, because stack might be empty before update() call
				if (_stateStack.isEmpty())
					_window.close();
			}
			render();
		}
	}
private:
	void update(sf::Time dt){
		_stateStack.update(dt);
	}

	void processInput(){
		sf::Event event;
		while (_window.pollEvent(event))
		{
			_stateStack.handleEvent(event);

			if (event.type == sf::Event::Closed)
				_window.close();
		}
	}

	void render(){
		_window.clear(sf::Color::White);
		_stateStack.draw();
		_window.display();
	}

	sf::RenderWindow _window;
	TextureHolder _textures;
	FontHolder _fonts;
	StateStack _stateStack;
};

#endif