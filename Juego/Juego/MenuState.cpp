#include "MenuState.hpp"
#include "GameState.hpp"
#include "Button.hpp"
#include "ResourceHolder.hpp"

MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	, _selectedButton(-1)
{
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	mBackgroundSprite.setTexture(texture);

	_buttonPlay = std::make_shared<Button>(*context.fonts, *context.textures);
	_buttonPlay->setPosition(100, 250);
	_buttonPlay->setText("Play");
	_buttonPlay->setCallback([this]()
	{
		requestStackPop();
		auto gameState = std::make_shared<GameState>(*_stack, _context);
		requestStackPush(gameState);
	});

	_buttonExit = std::make_shared<Button>(*context.fonts, *context.textures);
	_buttonExit->setPosition(100, 300);
	_buttonExit->setText("Exit");
	_buttonExit->setCallback([this]()
	{
		requestStackPop();
	});

}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(*_buttonPlay);
	window.draw(*_buttonExit);
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
		{
			_selectedButton = (_selectedButton != 1) ? 1 : 0;
			if (_selectedButton == 0){
				_buttonPlay->select();
				_buttonExit->deselect();
			}
			else if (_selectedButton == 1){
				_buttonPlay->deselect();
				_buttonExit->select();
			}
		}
		else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
		{
			if (_selectedButton == 0){
				_buttonPlay->activate();
			}
			else if (_selectedButton == 1){
				_buttonExit->activate();
			}
		}
	}
	return false;
}