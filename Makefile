CC=u++
CFLAGS=-c -Wall -Wno-unused-label -MMD
LDFLAGS=
SOURCES=main.cc printer.cc config.cc bank.cc parent.cc server.cc vmachine.cc plant.cc truck.cc student.cc office.cc card.cc courier.cc
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=soda

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
