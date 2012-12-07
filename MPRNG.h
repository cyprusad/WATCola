#include <uC++.h>
_Monitor PRNG { 
	public:
		PRNG( unsigned int seed = 1009 ) { srand( seed ); }
		void seed( unsigned int seed ) { srand( seed ); }
		unsigned int operator()() { return rand(); }
		unsigned int operator()( unsigned int u ) { return operator()() % (u + 1); } // [0,u]
		unsigned int operator()( unsigned int l, unsigned int u ) { return operator()( u - l ) + l; } // [l,u]
}; //PRNG
