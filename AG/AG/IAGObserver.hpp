#ifndef IAGOBSERVER_HPP
#define IAGOBSERVER_HPP

class IAGObserver{
public:
	virtual ~IAGObserver() {};

	virtual void onGeneracionTerminada(double, double, double) = 0;	// mejor, mejorGen, media

	virtual void onAGTerminado() = 0;
};

#endif