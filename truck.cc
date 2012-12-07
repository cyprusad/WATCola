#include <uC++.h>
#include <vector>
#include <iostream>
#include "soda.h"

using namespace std;

Truck::Truck ( Printer &prt, NameServer &nameServer, BottlingPlant &plant, 
				unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) {

	this->prt = &prt;
	this->nameServer = &nameServer;
	this->plant = &plant;
	this->numVendingMachines = numVendingMachines;
	this->maxStockPerFlavour = maxStockPerFlavour;
	this->machineList = new VendingMachine*[numVendingMachines];
	for (int i=0; i<4; i++){
		cargo[4] = 0;
	}
}

Truck::~Truck(){
	delete[] this->machineList;
	this->prt->print( Printer::Truck, 'F' );
}

//helper
unsigned int min(unsigned int v1, unsigned int v2) {
	if (v1<v2){
		return v1;
	} else {
		return v2;
	}
}

void Truck::main() {
	bool result;
	unsigned int totalSoda = 0;
	unsigned int *delivery;
	this->prt->print( Printer::Truck, 'S' );	
	this->machineList = nameServer->getMachineList(); //get vending machine addresses
	for (;;) {
		yield(r(1,10)); //Tim Hortons break
		result = plant->getShipment(this->cargo);
		if (result) _Accept( ~Truck ){
			break;
		}
		for (int i=0; i<4; i++){
			totalSoda += cargo[i];
		} //per flavour; pick delivery	
		for (unsigned int i=0; i < this->numVendingMachines; i++) {
			unsigned int numUnfilled = 0;
			this->prt->print( Printer::Truck, 'd', i, totalSoda );
			delivery = machineList[i]->inventory(); //get current inventory of vending machine
			for (int fl=0; fl<4; fl++){ //update stock for each flavour
				unsigned int perFlavour = min(this->cargo[fl], this->maxStockPerFlavour);
				if ( perFlavour != this->maxStockPerFlavour ) numUnfilled += ( this->maxStockPerFlavour - perFlavour );
				delivery[fl] = perFlavour;
				this->cargo[fl] -= perFlavour;
				totalSoda -= perFlavour;
			}
			if ( numUnfilled != 0) this->prt->print( Printer::Truck, 'U', i, numUnfilled );
			this->prt->print( Printer::Truck, 'D', i, totalSoda );
			machineList[i]->restocked(); //call restocked to indicate end of stocking
			if (totalSoda == 0) {
				break; //break out of delivery loop; truck is empty -> go back and pick up shipment
			}
		}
	}
}
