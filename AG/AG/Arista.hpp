#ifndef ARISTA_HPP
#define ARISTA_HPP

template<class N>
class Arista<N>{
public:
	Arista(){

	}

	Arista(N from, N to){
		_from = &from;
		_to = &to;
	}

	N* get



private:
	N* _from;
	N* _to;
};

#endif