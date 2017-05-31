#include "MenuState.hpp"
#include "GameState.hpp"
#include "Button.hpp"
#include "ResourceHolder.hpp"

MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	, _selectedButton(0)
{
	mFooter.setFont(context.fonts->get(Fonts::Alagard));
	mFooter.setCharacterSize(15);
	mFooter.setPosition(WINDOW_WIDTH*0.6, WINDOW_HEIGHT*0.95);
	mFooter.setString("developed by @alseether and @samus24");
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	mBackgroundSprite.setTexture(texture);

	sf::Texture& texture2 = context.textures->get(Textures::Title);
	mTitle.setTexture(texture2);

	mTitle.setScale(0.8,0.8);
	mTitle.setPosition(WINDOW_WIDTH*0.53, WINDOW_HEIGHT*0.2);

	_buttonPlay = std::make_shared<Button>(*context.fonts, *context.textures);
	_buttonPlay->setPosition(WINDOW_WIDTH*0.54, WINDOW_HEIGHT*0.5);
	_buttonPlay->setText("Play");
	_buttonPlay->setCallback([this]()
	{
		requestStackPop();
		auto gameState = std::make_shared<GameState>(*_stack, _context);
		requestStackPush(gameState);
	});
	_buttonPlay->select();

	_buttonExit = std::make_shared<Button>(*context.fonts, *context.textures);
	_buttonExit->setPosition(WINDOW_WIDTH*0.54, WINDOW_HEIGHT*0.6);
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
	window.draw(mTitle);
	window.draw(mFooter);
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