#include <uC++.h>
#include <uFuture.h>
#include <vector>
#include <iostream>
#include "soda.h"

using namespace std;

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
								unsigned int maxStockPerFlavour ){
	this->prt = &prt;
	this->nameServer = &nameServer;
	this->id = id;
	this->sodaCost = sodaCost;
	this->maxStockPerFlavour = maxStockPerFlavour;
	this->sodaInventory = new unsigned int[4];
	for (int i=0; i<4; i++){ 
		this->sodaInventory[i] = 0;
	}
	bool isRestocking = false;
}

VendingMachine::~VendingMachine(){
	delete this->sodaInventory;
}

void VendingMachine::main() {
	this->prt->print( Printer::Vending, this->id, 'S', this->sodaCost );
	this->nameServer->VMregister(this); //register self with name server
	for(;;){
		_Accept( ~VendingMachine ){
			this->prt->print( Printer::Vending, this->id, 'F' );
			break;
		} or _Accept( inventory ){
		} or _Accept( restocked ){
		} or _Accept( buy ){
		} 
	}
}

VendingMachine::Status VendingMachine::buy ( VendingMachine::Flavours flavour, WATCard &card ) {
	if ( this->isRestocking ) restocking.wait(); //wait on restocking
	if ( this->sodaInventory[flavour] == 0 ) return VendingMachine::STOCK; //flavour is not available 
	if ( card.getBalance() < this->sodaCost ) return VendingMachine::FUNDS;
	if ( card.getBalance() > this->sodaCost ) {
		card.withdraw( this->sodaCost );
		this->sodaInventory[flavour]--;
		this->prt->print( Printer::Vending, 'B', flavour, this->sodaInventory[flavour] );
	}
}

unsigned int *VendingMachine::inventory() {
	this->isRestocking = true;  //restocking begins
	this->prt->print( Printer::Vending, this->id, 'r' );
	return this->sodaInventory;	
}

void VendingMachine::restocked() {
	this->isRestocking = false;
	this->prt->print( Printer::Vending, this->id, 'R' );
	restocking.signal(); //signal student tasks waiting on this condition
}

_Nomutex unsigned int VendingMachine::cost() {
	return this->sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() {
	return this->id;
}
