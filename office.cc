#include <uC++.h>
#include <uFuture.h>
#include <iostream>
#include <vector>
#include "soda.h"

using namespace std;

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ){
	this->prt = &prt;
	this->bank = &bank;
	this->numCouriers = numCouriers;
	for (unsigned int i=0; i < numCouriers; i++) { //create initial pool of couriers
		Courier *courier = new Courier( prt, bank, i );
		couriers.push_back(courier);
	}
}

WATCardOffice::~WATCardOffice() {
	for (unsigned int i=0; i < this->numCouriers; i++) {
		delete couriers[i]; //delete pool of couriers
	}
	this->prt->print( Printer::WATCardOffice, 'F' );
}

FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ){
	FWATCard result; //future
	WATCard *card = new WATCard();
	card->deposit(amount);
	this->prt->print( Printer::WATCardOffice, 'C', sid, amount );
	result.delivery( card ); //delivery of future
	return result;
}

FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ){
	FWATCard result;
	card->deposit(amount);//hacky update watcard funds
	this->prt->print( Printer::WATCardOffice, 'T', sid, amount );
	result.delivery( card );//delivery of future
	return result;
}

WATCardOffice::Job *WATCardOffice::requestWork() { 
	//TODO
}


void WATCardOffice::main(){
	this->prt->print( Printer::WATCardOffice, 'S' );
	for (;;){
		_Accept( ~WATCardOffice ){
			break;
		} or _Accept( create ) {
		} or _Accept( transfer ){ 
		} or _Accept( requestWork ){
		
		}
	}
}




