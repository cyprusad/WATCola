#include <uC++.h>
#include <iostream>
#include <vector>
#include <map>
#include "soda.h"

using namespace std;

Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay ) {
	this->prt = &prt;
	this->bank = &bank;
	this->numStudents = numStudents;
	this->parentalDelay = parentalDelay;

}

void Parent::main() {
	unsigned int studentId;
	unsigned int amountTransferred;
	this->prt->print(Printer::Parent, 'S'); //print statement
	for (;;) {
		_Accept( ~Parent ) {
			this->prt->print( Printer::Parent, 'F' );
			break;
		} else {
		yield( this->parentalDelay );
		studentId = r(this->numStudents - 1);
		amountTransferred = r(1,3);
		this->prt->print( Printer::Parent, 'D', studentId, amountTransferred ); //print statement
		this->bank->deposit( studentId, amountTransferred );
		}
	}//for
}
