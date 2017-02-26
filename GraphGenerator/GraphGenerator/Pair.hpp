#ifndef PAIR_HPP
#define PAIR_HPP
template<class F, class S>
class Pair{
public:
	F first;
	S second;
	Pair<F, S>(){

	}

	Pair<F, S>(F first, S second){
		this->first = first;
		this->second = second;
	}
};

#endif