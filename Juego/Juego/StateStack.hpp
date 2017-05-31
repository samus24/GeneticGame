#ifndef STATESTACK_HPP
#define STATESTACK_HPP

#include "State.hpp"


class StateStack {
public:
public:
	enum Action
	{
		Push,
		Pop,
		Clear,
	};

	explicit StateStack(State::Context context);

	void update(sf::Time dt);
	void draw();
	void handleEvent(const sf::Event& event);

	void pushState(State::Ptr s);
	void popState();
	void clearStates();

	bool isEmpty() const;
private:
	void applyPendingChanges();
private:
	struct PendingChange
	{
		explicit PendingChange(Action action, State::Ptr s);

		Action action;
		State::Ptr state;
	};

private:
	std::vector<State::Ptr> _stack;
	std::vector<PendingChange> _pendingList;
	State::Context _context;
};

#endif