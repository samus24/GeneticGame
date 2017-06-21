#ifndef GAMEOVERSTATE_HPP
#define GAMEOVERSTATE_HPP

#include "State.hpp"
#include "Button.hpp"
#include "Stats.hpp"

class GameOverState : public State {
public:
	GameOverState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);

	void					setPlayerStats(Stats s);
	Stats					getPlayerStats() const;

private:
	sf::Sprite				_bgSprite;
	sf::Text				_text;
	std::shared_ptr<Button> _buttonRestart;
	std::shared_ptr<Button> _buttonExit;

	Stats					_playerStats;

	sf::Text				_statsText;

	int	_selectedButton;

};

#endif