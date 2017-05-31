#include "StateStack.hpp"

StateStack::PendingChange::PendingChange(Action action, State::Ptr s)
	: action(action)
	, state(s)
{
}

StateStack::StateStack(State::Context context)
	: _stack()
	, _context(context)
	, _pendingList()
{
}

void StateStack::update(sf::Time dt)
{
	// Iterate from top to bottom, stop as soon as update() returns false
	for (auto itr = _stack.rbegin(); itr != _stack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::draw()
{
	// Draw all active states from bottom to top
	for(State::Ptr& state : _stack)
		state->draw();
}

void StateStack::handleEvent(const sf::Event& event)
{
	// Iterate from top to bottom, stop as soon as handleEvent() returns false
	for (auto itr = _stack.rbegin(); itr != _stack.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(event))
			break;
	}

	applyPendingChanges();
}

void StateStack::pushState(State::Ptr s)
{
	_pendingList.push_back(PendingChange(Push, s));
}

void StateStack::popState()
{
	_pendingList.push_back(PendingChange(Pop, nullptr));
}

void StateStack::clearStates()
{
	_pendingList.push_back(PendingChange(Clear, nullptr));
}

bool StateStack::isEmpty() const
{
	return _stack.empty();
}


void StateStack::applyPendingChanges()
{
	for(PendingChange change : _pendingList)
	{
		switch (change.action)
		{
		case Push:
			_stack.push_back(change.state);
			break;

		case Pop:
			_stack.pop_back();
			break;

		case Clear:
			_stack.clear();
			break;
		}
	}

	_pendingList.clear();
}