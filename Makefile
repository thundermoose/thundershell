CC=g++ --std=c++14

LFLAGS=
CFLAGS=

SOURCES=
OBJECTS=$(SOURCES:.cc=o)
EXECUTABLE=thundershell


release: $(SOURCES) $(OBJECTS) 
	$(CC) -o $(EXECUTABLE)

debug: CFLAGS += -DDEBUG
debug: release

.cc.o:
	$(CC) -c -o $@ $< $(CFLAGS) 

