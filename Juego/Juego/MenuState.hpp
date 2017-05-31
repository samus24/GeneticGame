#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "State.hpp"
#include "Button.hpp"

class MenuState : public State {
public:
	MenuState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);


private:
	sf::Sprite				mBackgroundSprite;
	sf::Sprite				mTitle;
	sf::Text				mFooter;
	std::shared_ptr<Button> _buttonPlay;
	std::shared_ptr<Button> _buttonExit;

	int	_selectedButton;

};

#endif