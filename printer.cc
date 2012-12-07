#include <uC++.h>
#include <iostream> //cin, cout
#include <vector>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <map>
#include "soda.h"

using namespace std;

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) {
	this->numStudents = numStudents;
	this->numVendingMachines = numVendingMachines;
	this->numCouriers = numCouriers;
	
	//Convenience strings
	this->blank = "        "; //8space blank string;
	this->strParent = "Parent";
	this->strWATCardOffice = "WATOff";
	this->strNameServer = "NameS";
	this->strTruck = "Truck";
	this->strPlant = "Plant";
	this->strStudent = "Stud";
	this->strMachine = "Mach";
	this->strCourier = "Cour";

	//Initialize/reset
	this->refresh();
	//Drop into main
	resume();
} //Printer constructor

Printer::~Printer() {
	cout << "***************************" << endl;
} //Destructor


void Printer::refresh() {
	map<string, string>::iterator cleaner;
	map<string, bool>::iterator eraser;
	for ( cleaner = this->buffer.begin(); cleaner!=this->buffer.end(); cleaner++ ) {
		(*cleaner).second = this->blank;
	}
	for ( eraser = this->scratch.begin(); eraser!=this->scratch.end(); eraser++ ) {
		(*eraser).second = false;
	}
}//refresh

void Printer::printStates() {
	unsigned int i;
	stringstream num;
	string tmp;
	cout << setw(8) << buffer[this->strParent] << " "
		 << setw(8) << buffer[this->strWATCardOffice] << " "
		 << setw(8) << buffer[this->strNameServer] << " "
		 << setw(8) << buffer[this->strTruck] << " "
		 << setw(8) << buffer[this->strPlant] << " ";
	for ( i=0; i< this->numStudents; i++ ) {
		num << i;
		tmp = this->strStudent + num.str();
		cout << setw(8) << buffer[tmp] << " ";
		num.str("");
		num.clear();
	}
	for ( i=0; i< this->numVendingMachines; i++ ) {
		num << i;
		tmp = this->strMachine + num.str();
		cout << setw(8) << buffer[tmp] << " ";
		num.str("");
		num.clear();
	}
	for ( i=0; i< this->numCouriers; i++ ) {
		num << i;
		tmp = this->strCourier + num.str();
		cout << setw(8) << buffer[tmp] << " ";
		num.str("");
		num.clear();
	}
	cout << endl;

} //Printer::printStates

void Printer::main() {
	//Initial output of the system
	unsigned int i;
	stringstream num;
	cout << setw(8) << this->strParent << " "
		 << setw(8) << this->strWATCardOffice << " "
		 << setw(8) << this->strNameServer << " "
		 << setw(8) << this->strTruck << " "
		 << setw(8) << this->strPlant << " ";
	for ( i=0; i< this->numStudents; i++ ) {
		num << i;
		cout << setw(6) << this->strStudent << setw(2) << num.str() << " ";
		num.str("");
		num.clear();
	}
	for ( i=0; i< this->numVendingMachines; i++ ) {
		num << i;
		cout << setw(6) << this->strMachine << setw(2) << num.str() << " ";
		num.str("");
		num.clear();
	}
	for ( i=0; i< this->numCouriers; i++ ) {
		num << i;
		cout << setw(6) << this->strCourier << setw(2) << num.str() << " ";
		num.str("");
		num.clear();
	}
	cout << endl;

	for ( i=0; i < (this->numStudents + this->numVendingMachines + this->numCouriers + 5); i++ ) {
		cout << "********" << " ";
	}
	cout << endl;
	
	suspend();

	for (;;) {
		this->printStates();
		this->refresh();
		suspend();
	}
}//Printer::main()

void Printer::print( Kind kind, char state ) {
	stringstream text;
	string key;
	
	if ( kind == Printer::Parent ) {
		key = this->strParent;
	} else if ( kind == Printer::WATCardOffice ){
		key = this->strWATCardOffice;
	} else if ( kind == Printer::NameServer ) {
		key = this->strNameServer;
	} else if ( kind == Printer::Truck ) {
		key = this->strTruck;
	} else if ( kind == Printer::BottlingPlant ) {
		key = this->strPlant;
	}
	

	if ( state == 'F') { //Special case when finish message is pushed
		resume();
		map<string, string>::iterator finished;
		for ( finished=this->buffer.begin(); finished!=this->buffer.end(); finished++ ) {
			if ( (*finished).first == key) {
				text << state;
				(*finished).second = text.str(); // set the finished message for the correct kind
			} else {
				(*finished).second = "..."; //set ... for everything else
			}
		}
		//this->refresh();
		resume();

	}
	else if ( !key.empty() && scratch[key] ) {
		resume();
		text << state;
		buffer[key] = text.str();
		scratch[key] = true;
	} else {
		text << state;
		buffer[key] = text.str();
		scratch[key] = true;
	}
} //Printer::print(Kind kind, char state)


void Printer::print( Kind kind, char state, int value1 ) {
	stringstream text;
	string key;
	
	if ( kind == Printer::Parent ) {
		key = this->strParent;
	} else if ( kind == Printer::WATCardOffice ) {
		key = this->strWATCardOffice;
	} else if ( kind == Printer::NameServer ) {
		key = this->strNameServer;
	} else if ( kind == Printer::Truck ) {
		key = this->strTruck;
	} else if ( kind == Printer::BottlingPlant ) {
		key = this->strPlant;
	}

	if ( !key.empty() && scratch[key] ) {
		resume();
		text << state << value1;
		buffer[key] = text.str();
		scratch[key] = true;
	} else {
		text << state << value1;
		buffer[key] = text.str();
		scratch[key] = true;
	}	
}//Printer::print(Kind kind, char state, int value1)


void Printer::print( Kind kind, char state, int value1, int value2 ) {
	stringstream text;
	string key;
	
	if ( kind == Printer::Parent ) {
		key = this->strParent;
	} else if ( kind == Printer::WATCardOffice ) {
		key = this->strWATCardOffice;
	} else if ( kind == Printer::NameServer ) {
		key = this->strNameServer;
	} else if ( kind == Printer::Truck ) {
		key = this->strTruck;
	} else if ( kind == Printer::BottlingPlant ) {
		key = this->strPlant;
	}

	if ( !key.empty() && scratch[key] ) {
		resume();
		text << state << value1 << "," << value2;
		buffer[key] = text.str();
		scratch[key] = true;
	} else {
		text << state << value1 << "," << value2;
		buffer[key] = text.str();
		scratch[key] = true;
	}	
}//Printer::print(Kind kind, char state, int value1, int value2)

void Printer::print( Kind kind,	unsigned int lid, char state ) {
	stringstream text;
	string keyPrefix;
	stringstream keyStream;
	string key;

	if ( kind == Printer::Student ) {
		keyPrefix = this->strStudent;
	} else if ( kind == Printer::Vending ) {
		keyPrefix = this->strMachine;
	} else if ( kind == Printer::Courier ) {
		keyPrefix = this->strCourier;
	} 

	keyStream << keyPrefix << lid;
	key = keyStream.str();

	if ( state == 'F') { //Special case when finish message is pushed
		resume();
		map<string, string>::iterator finished;
		for ( finished=this->buffer.begin(); finished!=this->buffer.end(); finished++ ) {
			if ( (*finished).first == key) {
				text << state;
				(*finished).second = text.str(); // set the finished message for the correct kind
			} else {
				(*finished).second = "..."; //set ... for everything else
			}
		}
		//this->refresh();
		resume();

	}
	else if ( !key.empty() && scratch[key] ) {
		resume();
		text << state;
		buffer[key] = text.str();
		scratch[key] = true;
	} else {
		text << state;
		buffer[key] = text.str();
		scratch[key] = true;
	}
} //Printer::print(Kind kind, unsigned int lid,  char state)


void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
	stringstream text;
	string keyPrefix;
	stringstream keyStream;
	string key;

	if ( kind == Printer::Student ) {
		keyPrefix = this->strStudent;
	} else if ( kind == Printer::Vending) {
		keyPrefix = this->strMachine;
	} else if ( kind == Printer::Courier ) {
		keyPrefix = this->strCourier;
	} 

	keyStream << keyPrefix << lid;
	key = keyStream.str();
	
	if ( !key.empty() && scratch[key] ) {
		resume();
		text << state << value1;
		buffer[key] = text.str();
		scratch[key] = true;
	} else {
		text << state << value1;
		buffer[key] = text.str();
		scratch[key] = true;
	}	
}//Printer::print(Kind kind, unsigned int lid, char state, int value1)


void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
	stringstream text;
	string keyPrefix;
	stringstream keyStream;
	string key;

	if ( kind == Printer::Student ) {
		keyPrefix = this->strStudent;
	} else if ( kind == Printer::Vending) {
		keyPrefix = this->strMachine;
	} else if ( kind == Printer::Courier ) {
		keyPrefix = this->strCourier;
	} 

	keyStream << keyPrefix << lid;
	key = keyStream.str();
	
	if ( !key.empty() && scratch[key] ) {
		resume();
		text << state << value1 << "," << value2;
		buffer[key] = text.str();
		scratch[key] = true;
	} else {
		text << state << value1 << "," << value2;
		buffer[key] = text.str();
		scratch[key] = true;
	}	
}//Printer::print(Kind kind, unsigned int lid, char state, int value1, int value2)


