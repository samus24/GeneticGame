#ifndef GAMEOVERSTATE_HPP
#define GAMEOVERSTATE_HPP

#include "State.hpp"
#include "Button.hpp"

class GameOverState : public State {
public:
	GameOverState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);


private:
	sf::Sprite				_bgSprite;
	sf::Text				_text;
	std::shared_ptr<Button> _buttonRestart;
	std::shared_ptr<Button> _buttonExit;

	int	_selectedButton;

};

#endif