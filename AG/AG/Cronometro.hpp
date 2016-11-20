#ifndef CRONOMETRO_HPP
#define CRONOMETRO_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include <chrono>

struct Lapso {
	std::chrono::high_resolution_clock::time_point ini;
	std::chrono::high_resolution_clock::time_point fin;

	double asMilli(){
		std::chrono::duration<double, std::milli> t = (fin - ini);
		return t.count();
	}

	double asMicro(){
		std::chrono::duration<double, std::micro> t = (fin - ini);
		return t.count();
	}
};

class Cronometro {
public:
	Cronometro(){

	}

	void creaMedida(std::string nombre){
		_medidas.emplace(nombre, std::vector<Lapso>());
	}

	bool iniciaMedida(std::string nombre, std::chrono::high_resolution_clock::time_point t){
		if (_medidas.find(nombre) == _medidas.end()){
			return false;
		}
		else{
			Lapso l;
			l.ini = t;
			_medidas[nombre].push_back(l);
			return true;
		}
	}

	bool finalizaMedida(std::string nombre, std::chrono::high_resolution_clock::time_point t){
		if (_medidas.find(nombre) == _medidas.end()){
			return false;
		}
		else{
			Lapso l = *(_medidas[nombre].end()-1);
			l.fin = t;
			_medidas[nombre].pop_back();
			_medidas[nombre].push_back(l);
			return true;
		}
	}

	double getMediaAsMilli(std::string nombre){
		if (_medidas.find(nombre) == _medidas.end()){
			return -1;
		}
		else{
			double media = 0;
			for (Lapso l : _medidas[nombre]){
				media += l.asMilli();
			}
			return (media / _medidas[nombre].size());
		}
	}

	double getMediaAsMicro(std::string nombre){
		if (_medidas.find(nombre) == _medidas.end()){
			return -1;
		}
		else{
			double media = 0;
			for (Lapso l : _medidas[nombre]){
				media += l.asMicro();
			}
			return (media / _medidas[nombre].size());
		}
	}

private:
	std::unordered_map<std::string, std::vector<Lapso>> _medidas;
};

#endif