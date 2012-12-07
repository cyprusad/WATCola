#include <uC++.h>
#include <iostream> // cin/cout
#include <vector>
#include "soda.h"

using namespace std;

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ) {
	this->prt = &prt;
	this->numVendingMachines = numVendingMachines;
	this->numStudents = numStudents;
	this->machineList = new VendingMachine*[numVendingMachines];
	for (unsigned int i=0; i < numStudents; i++ ){
		this->studentToVendingMachine.push_back(0); //initially every student has id=0 vending machine assigned
	}
	this->numRegistered = 0;
}


NameServer::~NameServer() {
	delete[] this->machineList;
}

void NameServer::main() {
	unsigned int numStudentsInitialized = 0;
	this->prt->print(Printer::NameServer, 'S');
	for (;;) {
		_Accept( ~NameServer ) {
			this->prt->print( Printer::NameServer, 'F' );
			break;
		} or _Accept( VMregister ) {
			numRegistered++;
			studentToVendingMachine[numStudentsInitialized] = (numRegistered - 1) % numVendingMachines;
			numStudentsInitialized++;
		} or _Accept( getMachine ){ 
		} or _Accept( getMachineList ) {
		}
	}
}

void NameServer::VMregister( VendingMachine *vendingmachine ) {
	unsigned int id = vendingmachine->getId(); 
	this->prt->print(Printer::NameServer, 'R', id); //print
	this->machineList[id] = vendingmachine; //add the vending machine to the list of machines already known by nameserver
}

VendingMachine *NameServer::getMachine( unsigned int id ) {
	unsigned int machineId = studentToVendingMachine[id]; //find the machine assigned to the student
	studentToVendingMachine[id] = (machineId + 1) % numVendingMachines; //update the vending machine assigned (for next call to this function)
	this->prt->print( Printer::NameServer, 'N', id, machineId ); //print
	return this->machineList[machineId]; //return the vending machine found
}

VendingMachine **NameServer::getMachineList() {
	return this->machineList; //array of pointers
}
