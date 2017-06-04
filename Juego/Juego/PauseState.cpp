#include "PauseState.hpp"
#include "MenuState.hpp"
#include "Button.hpp"
#include "ResourceHolder.hpp"

PauseState::PauseState(StateStack& stack, Context context)
: State(stack, context)
, _selectedButton(0)
{
	_text.setFont(context.fonts->get(Fonts::Alagard));
	_text.setCharacterSize(60);
	_text.setString("Pause");
	centerOrigin(_text);
	_text.setPosition(WINDOW_WIDTH*0.5, WINDOW_HEIGHT*0.4);

	_bgRect.setPosition(0, 0);
	_bgRect.setSize(sf::Vector2f(context.window->getSize()));
	_bgRect.setFillColor(sf::Color(127, 127, 127, 127));

	_buttonResume = std::make_shared<Button>(*context.fonts, *context.textures);
	_buttonResume->setText("Resume");
	_buttonResume->setPosition(WINDOW_WIDTH*0.33, WINDOW_HEIGHT*0.5);
	_buttonResume->setCallback([this]()
	{
		requestStackPop();
	});
	_buttonResume->select();

	_buttonExit = std::make_shared<Button>(*context.fonts, *context.textures);
	_buttonExit->setPosition(WINDOW_WIDTH*0.33, WINDOW_HEIGHT*0.6);
	_buttonExit->setText("Exit");
	_buttonExit->setCallback([this]()
	{
		requestStateClear();
		auto menuState = std::make_shared<MenuState>(*_stack, _context);
		requestStackPush(menuState);
	});

}

void PauseState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(_bgRect);
	window.draw(_text);
	window.draw(*_buttonResume);
	window.draw(*_buttonExit);
}

bool PauseState::update(sf::Time)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape){
			requestStackPop();
		}
	}
	if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
		{
			_selectedButton = (_selectedButton != 1) ? 1 : 0;
			if (_selectedButton == 0){
				_buttonResume->select();
				_buttonExit->deselect();
			}
			else if (_selectedButton == 1){
				_buttonResume->deselect();
				_buttonExit->select();
			}
		}
		else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
		{
			if (_selectedButton == 0){
				_buttonResume->activate();
			}
			else if (_selectedButton == 1){
				_buttonExit->activate();
			}
		}
	}
	return false;
}