#ifndef TESTBENCH_HPP
#define TESTBENCH_HPP

#include <fstream>

#include "IAGObserver.hpp"
#include "Parametros.hpp"
#include "ParametrosEval.hpp"
#include "AG.hpp"

class Testbench : public IAGObserver{
public:
	Testbench(){
		_pEval.nodosOptimos = 25;
		_pEval.gradoOptimo = 2;
		_pEval.anchoOptimo = 30;
		_pEval.altoOptimo = 20;
		_pEval.ciclosOptimos = 4;
		_pEval.enemigosOptimos = 35;
		_pEval.cofresOptimos = 10;
	}

	bool openFile(std::string path){
		_outfile.open(path);
		return _outfile.is_open();
	}

	void launchTestbench(){
		if (!_outfile.is_open())
			return;
		unsigned int i = 0;
		/// Estos parametros no van a variar
		_param.minNodos = 10;
		_param.maxNodos = 40;
		_param.densidad = 0.03;
		///

		_param.tamPob = 30;							
		_param.iteraciones = 30;								
		_param.elitismo = false;					
		_param.bloating = true;						
		_param.contractividad = false;				
		_param.probCruce = 0.6;						
		_param.probMutacion = 0.02;					
		_param.seleccion = new SeleccionRuleta();	
		_param.cruce = new CruceMonopunto();		
		_param.mutacion = new MutacionNodo();		

		_outfile << "Ejecucion " << i << std::endl;
	}

private:
	std::ofstream _outfile;
	ParametrosEval _pEval;
	Parametros _param;
	


};

#endif