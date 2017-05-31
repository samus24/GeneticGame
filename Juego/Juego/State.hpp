#ifndef STATE_HPP
#define STATE_HPP

#include "ResourceIdentifiers.hpp"

#include <memory>
#include <SFML/Graphics.hpp>

class StateStack;

class State{
public:
	typedef std::shared_ptr<State> Ptr;

	struct Context
	{
		Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts);

		sf::RenderWindow*	window;
		TextureHolder*		textures;
		FontHolder*			fonts;
	};

	State(StateStack& stack, Context context);
	virtual	~State();

	virtual void draw() = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;

protected:
	void				requestStackPush(State::Ptr s);
	void				requestStackPop();
	void				requestStateClear();

	Context				getContext() const;

	StateStack*	_stack;
	Context	_context;
};

#endif 