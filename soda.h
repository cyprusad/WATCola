#include <uC++.h>
#include <uFuture.h>
#include "MPRNG.h"
#include <vector>
#include <cstring>
#include <map>


using namespace std;

extern PRNG r;
_Cormonitor Printer;
_Task NameServer;
_Task VendingMachine;
_Task WATCardOffice;
_Monitor Bank;
_Task Truck;
class WATCard;
typedef Future_ISM<WATCard *> FWATCard;		// future WATCard pointer

struct ConfigParms {
    unsigned int sodaCost;                      // MSRP per bottle
    unsigned int numStudents;                   // number of students to create
    unsigned int maxPurchases;                  // maximum number of bottles a student purchases
    unsigned int numVendingMachines;            // number of vending machines
    unsigned int maxStockPerFlavour;            // maximum number of bottles of each flavour stocked
    unsigned int maxShippedPerFlavour;          // number of bottles of each flavour in a shipment
    unsigned int timeBetweenShipments;          // length of time between shipment pickup
    unsigned int parentalDelay;                 // length of time between cash deposits
    unsigned int numCouriers;                   // number of couriers in the pool
};

void processConfigFile( const char *configFile, ConfigParms &cparms );

_Task Student {
	Printer *prt;
	NameServer *nameServer;
	WATCardOffice *cardOffice;
	unsigned int id;
	unsigned int numPurchases;
    unsigned int favouriteFlavour;
	VendingMachine *currMachine;
	FWATCard watcard;
	void main();
  public:
    ~Student();
	Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
             unsigned int maxPurchases );
};

class WATCard {
    WATCard( const WATCard & );			// prevent copying
    WATCard &operator=( const WATCard & );
  	unsigned int balance;
  public:
    WATCard();
    void deposit( unsigned int amount );
    void withdraw( unsigned int amount );
    unsigned int getBalance();
};

_Task WATCardOffice {
    struct Job {				// marshalled arguments and return future
		unsigned int sid;
		unsigned int money;				// call arguments (YOU DEFINE "Args")
		char jobType;
		FWATCard result;			// return future
		Job( unsigned int sid, unsigned int money, char jobType ) {
			this->sid = sid;
			this->money = money; 
			this->jobType = jobType; 
		}
    };
    _Task Courier {
			Printer *prt;
			Bank *bank;
			unsigned int id;
		public:
			void main();
			Courier( Printer &prt, Bank &bank, unsigned int id);
			~Courier();	
	};			// communicates with bank
    Printer *prt;
	Bank *bank;
	vector<Job*> requests;
	unsigned int numCouriers;
	vector<Courier*> couriers;
	void main();
  public:
    _Event Lost {};
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
	FWATCard create( unsigned int sid, unsigned int amount );
    FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork( );
};


_Monitor Bank {
 	unsigned int numStudents;
  	vector<int> accounts;
  	uCondition lock;
  public:
    Bank( unsigned int numStudents );
    ~Bank();
	void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};


_Task Parent {
	unsigned int numStudents;
	unsigned int parentalDelay;
	Bank *bank;
	Printer *prt;
    void main();
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
};


_Task VendingMachine {
	Printer *prt;
	NameServer *nameServer;
	unsigned int maxStockPerFlavour;
	unsigned int sodaCost;
	unsigned int id;
    unsigned int *sodaInventory;
	void main();
	bool isRestocking;
  	uCondition restocking;
  public:
    enum Flavours {	BLUES = 0, CLASSICAL = 1, ROCK = 2, JAZZ = 3 }; 		
    enum Status { BUY, STOCK, FUNDS };		// purchase status: successful buy, out of stock, insufficient funds
    ~VendingMachine();
	VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    Status buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
};


_Task NameServer {
    Printer *prt;
	unsigned int numVendingMachines;
	unsigned int numStudents;
	VendingMachine **machineList;
	vector<int> studentToVendingMachine;
	unsigned int numRegistered;
	void main();
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
	void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};


_Task BottlingPlant {
    Printer *prt;
	NameServer *nameServer;
	Truck *truck;
	unsigned int numVendingMachines;
	unsigned int maxShippedPerFlavour;
	unsigned int maxStockPerFlavour;
	unsigned int timeBetweenShipments;
	unsigned int shipment[4];
	unsigned int bottlesGenerated;
	bool isClosingDown;
	void main();
  public:
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    bool getShipment( unsigned int cargo[] );
};


_Task Truck {
    Printer *prt;
	NameServer *nameServer;
	BottlingPlant *plant;
	unsigned int numVendingMachines;
	unsigned int maxStockPerFlavour;
	unsigned int cargo[4];
	VendingMachine **machineList;
	void main();
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
	~Truck();
};


_Cormonitor Printer {
  unsigned int numStudents;
  unsigned int numVendingMachines;
  unsigned int numCouriers;
  string blank;
  string strParent;
  string strWATCardOffice;
  string strNameServer;
  string strTruck;
  string strPlant;
  string strStudent;
  string strMachine;
  string strCourier;
  //buffer to store the state while printing
  map<string, string> buffer;
  map<string, bool> scratch;
 
  void refresh();
  void main();
  void printStates();
  public:
    enum Kind { Parent, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
	~Printer();
};
