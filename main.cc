#include <uC++.h>
#include <iostream> //cin, cout
#include <vector>
#include <cstring> //atoi
#include <iomanip> //setw
#include <sstream> 
#include "soda.h"

using namespace std;

PRNG r;

void usage ( char *argv[] ) {
	cerr << "Usage: " << argv[0]
		 << " [ config-file [ random-seed ]]" << endl;
	exit(1);
}//usage

void uMain::main(){
	unsigned int seed;
	const char* configFile;	
	//Shell interface
	switch (argc) {
		case 3: //config file and seed provided
			configFile = argv[1];
			seed = atoi(argv[2]);
			break;
		case 2: //only config file is provided
			configFile = argv[1];
			seed = getpid();
			break;
		case 1:
			configFile = "soda.config";
			seed = getpid();
			break;
		default:
			usage(argv);
	}
	//do we have to make sure that string is null terminated?

	if (seed < 0) {
		cerr << "Seed should be a positive number" << endl;
		exit(1);
	}
	
	//Set the seed
	r.seed(seed);

	//Instantiate the stuff
	Printer *pr;
	Bank *bank;
	Parent *parent;
	WATCardOffice *cardOffice;
	NameServer *server;
	BottlingPlant *plant;
	Student *s;
	WATCard *wc;
	ConfigParms cfgParams;
	unsigned int i;
	vector<Student*> students;
	VendingMachine **machines;

	//Get values
	processConfigFile( configFile, cfgParams );
	
	//Initialize
	pr = new Printer( cfgParams.numStudents, cfgParams.numVendingMachines, cfgParams.numCouriers );
	bank = new Bank( cfgParams.numStudents );
	cardOffice = new WATCardOffice( *pr, *bank, cfgParams.numCouriers );
	parent = new Parent( *pr, *bank, cfgParams.numStudents, cfgParams.parentalDelay );
	server = new NameServer( *pr, cfgParams.numVendingMachines, cfgParams.numStudents );
	for (i=0; i < cfgParams.numStudents; i++){
		Student *student = new Student( *pr, *server, *cardOffice, i, cfgParams.maxPurchases );
		students.push_back(student);
	}
	plant = new BottlingPlant( *pr, *server, cfgParams.numVendingMachines, cfgParams.maxShippedPerFlavour, cfgParams.maxStockPerFlavour, cfgParams.timeBetweenShipments );
	machines = new VendingMachine*[cfgParams.numVendingMachines];
	for (i=0; i < cfgParams.numVendingMachines; i++) {
		VendingMachine *vm = new VendingMachine( *pr, *server, i, cfgParams.sodaCost, cfgParams.maxStockPerFlavour);
		machines[i] = vm;
	}
	
	//dummy work
	//yield(600);

	//Delete all the iniitialized data structures
	//delete server;
	for (i=0; i < cfgParams.numStudents; i++) {
		delete students[i];
	}
	delete plant;
	for (i=0; i < cfgParams.numVendingMachines; i++) {
		delete machines[i];
	}
	delete[] machines;
	//delete plant;
	delete server;
	delete parent;
	delete cardOffice;
	delete bank;
	delete pr;

} // uMain::main
