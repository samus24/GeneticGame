#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "State.hpp"
#include "Button.hpp"

class PauseState : public State {
public:
	PauseState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);


private:
	sf::RectangleShape		_bgRect;
	sf::Text				_text;
	std::shared_ptr<Button> _buttonResume;
	std::shared_ptr<Button> _buttonExit;

	int	_selectedButton;

};

#endif