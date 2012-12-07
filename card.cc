#include <uC++.h>
#include <iostream>
#include <vector>
#include "soda.h"

using namespace std;

WATCard::WATCard(){
	this->balance = 0;
}

void WATCard::deposit( unsigned int amount ) {
	this->balance += amount;
}

void WATCard::withdraw( unsigned int amount ) {
	this->balance -= amount;
}

unsigned int WATCard::getBalance(){
	return this->balance;
}
