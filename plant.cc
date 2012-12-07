#include <uC++.h>
#include <vector>
#include "soda.h"

using namespace std;

BottlingPlant::BottlingPlant ( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
								unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
								unsigned int timeBetweenShipments ) {
	this->prt = &prt;
	this->nameServer = &nameServer;
	this->numVendingMachines = numVendingMachines;
	this->maxShippedPerFlavour = maxShippedPerFlavour;
	this->maxStockPerFlavour = maxStockPerFlavour;
	this->timeBetweenShipments = timeBetweenShipments;
	this->truck = new Truck( prt, nameServer, *this, numVendingMachines, maxStockPerFlavour );
	for ( int i=0; i < 4; i++ ) {
		this->shipment[i] = 0;
	}
	this->bottlesGenerated = 0;
	this->isClosingDown = false;
}

bool BottlingPlant::getShipment( unsigned int cargo[] ) {
	if (this->isClosingDown){ //don't update cargo, return true
		return this->isClosingDown;
	} else {unsigned int totalCargo = 0; //update cargo, return false
		for (int i=0; i<4; i++){
			cargo[i] = shipment[i];
			totalCargo += cargo[i];
		}
		this->prt->print( Printer::Truck, 'P', totalCargo );
		return this->isClosingDown;
	}
}

void BottlingPlant::main() {
	this->prt->print( Printer::BottlingPlant, 'S' );
	yield(this->timeBetweenShipments);//first shipment
	for (int i=0; i<4; i++){
		this->shipment[i] = r(this->maxShippedPerFlavour);
		this->bottlesGenerated += this->shipment[i];
	}

	this->prt->print( Printer::BottlingPlant, 'G', this->bottlesGenerated ); //print
	
	for (;;){
		_Accept( ~BottlingPlant ){
			this->isClosingDown = true; //bottling plant is closing down
			break;
		} or _Accept( getShipment ){
			this->prt->print( Printer::BottlingPlant, 'P' );
			this->bottlesGenerated = 0; //reset
			yield(this->timeBetweenShipments);//first shipment
			for (int i=0; i<4; i++){
				this->shipment[i] = r(this->maxShippedPerFlavour);
				this->bottlesGenerated += this->shipment[i];
			}
			this->prt->print( Printer::BottlingPlant, 'G', this->bottlesGenerated ); //print
		}
	}
	_Accept( getShipment ){
			this->prt->print( Printer::BottlingPlant, 'P' );
			this->bottlesGenerated = 0; //reset
			yield(this->timeBetweenShipments);//first shipment
			for (int i=0; i<4; i++){
				this->shipment[i] = r(this->maxShippedPerFlavour);
				this->bottlesGenerated += this->shipment[i];
			}
			this->prt->print( Printer::BottlingPlant, 'G', this->bottlesGenerated ); //print
	}
}
