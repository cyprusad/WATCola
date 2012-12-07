#include <uC++.h>
#include <uFuture.h>
#include <iostream>
#include <vector>
#include "soda.h"

using namespace std;

WATCardOffice::Courier::Courier( Printer &prt, Bank &bank, unsigned int id ) {
	this->prt = &prt;
	this->bank = &bank;
	this->id = id;
}

WATCardOffice::Courier::~Courier(){
	this->prt->print(Printer::Courier, id, 'F');
}

void WATCardOffice::Courier::main(){
	this->prt->print(Printer::Courier, id, 'S');
}
