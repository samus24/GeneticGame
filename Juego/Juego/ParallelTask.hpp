#ifndef PARALLELTASK_HPP
#define PARALLELTASK_HPP

#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Clock.hpp>

#include "IAGObserver.hpp"
#include "Parametros.hpp"

class ParallelTask : public IAGObserver
{
public:
	ParallelTask();
	void				execute();
	bool				isFinished();
	float				getCompletion();

	Cromosoma getMejor() const;

	virtual void onGeneracionTerminada(Cromosoma);

	virtual void onAGTerminado(Cromosoma);

private:
	void				runTask();

private:
	sf::Thread			mThread;
	bool				mFinished;
	sf::Clock			mElapsedTime;
	sf::Mutex			mMutex;

	Parametros _p;
	Cromosoma	_mejor;
	unsigned int _generaciones;
	
};

#endif