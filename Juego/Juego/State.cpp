#include "State.hpp"
#include "StateStack.hpp"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts)
	: window(&window)
	, textures(&textures)
	, fonts(&fonts)
{
}

State::State(StateStack& stack, Context context)
	: _stack(&stack)
	, _context(context)
{
}

State::~State()
{
}

void State::requestStackPush(State::Ptr s)
{
	_stack->pushState(s);
}

void State::requestStackPop()
{
	_stack->popState();
}

void State::requestStateClear()
{
	_stack->clearStates();
}

State::Context State::getContext() const
{
	return _context;
}