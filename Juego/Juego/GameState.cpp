#include "GameState.hpp"
#include "ResourceHolder.hpp"

GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
{
	

}

void GameState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
	/*
	window.draw(mBackgroundSprite);
	window.draw(*_buttonPlay);
	window.draw(*_buttonExit);*/
}

bool GameState::update(sf::Time)
{
	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		
	}
	return false;
}