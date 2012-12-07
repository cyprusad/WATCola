#include <uC++.h>
#include <uFuture.h>
#include <iostream>
#include <vector>
#include "soda.h"

using namespace std;

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, unsigned int maxPurchases ) {
	this->prt = &prt;
	this->nameServer = &nameServer;
	this->cardOffice = &cardOffice;
	this->id = id;
	this->numPurchases = r(1,maxPurchases);
	this->favouriteFlavour = r(3); //pick flavour

}

Student::~Student(){
	this->prt->print( Printer::Student, this->id, 'F' );
}

void Student::main(){
	unsigned int totalPurchases = 0;
	VendingMachine::Flavours flavour = (VendingMachine::Flavours)this->favouriteFlavour;
	
	this->prt->print( Printer::Student, this->id, 'S', this->favouriteFlavour, this->numPurchases );	
	this->watcard = this->cardOffice->create(this->id, 5); //create and update, using future
	this->nameServer->getMachine(this->id); //extra call initially needed - this is a bug
	this->currMachine = this->nameServer->getMachine(this->id); //pointer to current machine
	this->prt->print( Printer::Student, this->id, 'V', this->currMachine->getId() ); //print
	
	while( totalPurchases != this->numPurchases ){
		yield(r(1,10)); //yield before purchase	
		VendingMachine::Status state = this->currMachine->buy( flavour, *this->watcard() );
		if (state == VendingMachine::BUY) { //purchase was successful
			totalPurchases++;
			this->prt->print( Printer::Student, this->id, 'B', this->watcard()->getBalance() );
		} else if (state == VendingMachine::STOCK) {
			for(;;) {//busy wait till you find a vending machine with
				this->currMachine = this->nameServer->getMachine(this->id);
				this->prt->print( Printer::Student, this->id, 'V', this->currMachine->getId() ); //print	
				state = this->currMachine->buy(flavour, *(this->watcard()) );
				if ( state != VendingMachine::STOCK ){
					break; //break when finally found vending machine with flavour
				}
			}
			//once the correct vending machine with the flavour is found, 
			if (state == VendingMachine::BUY) {
				totalPurchases++;
				this->prt->print( Printer::Student, this->id, 'B', this->watcard()->getBalance() );
			} else if( state== VendingMachine::FUNDS ) { //insufficient funds
				this->cardOffice->transfer(this->id, 5 + this->currMachine->cost(), this->watcard());
			}
		} else if (state == VendingMachine::FUNDS) { //insufficient funds
			this->cardOffice->transfer(this->id, 5 + this->currMachine->cost(), this->watcard());
		}
	}
}
