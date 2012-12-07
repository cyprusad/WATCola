#include <uC++.h>
#include <iostream>
#include <vector>
#include <map>
#include "soda.h"

using namespace std;

Bank::Bank ( unsigned int numStudents ) {
	this->numStudents = numStudents;
	for (unsigned int i=0; i<numStudents; i++) {
		this->accounts.push_back(0);
	}	
}

Bank::~Bank() {
	for (unsigned int i=0; i<numStudents; i++) {
		this->accounts.pop_back();
	}
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
	this->accounts[id] = this->accounts[id] + amount;
	lock.signal(); //everytime a deposit is made, signal to check if the correct student received the money
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
	do {
		if ( this->accounts[id] < amount ) lock.wait();
	}  
	while ( this->accounts[id] < amount ); //since we are using only one lock, re-check if the condition was acutally satisfied
	this->accounts[id] -= amount; //if it got past the previous do-while loop, the amount for the student's a/c is > amount

}
