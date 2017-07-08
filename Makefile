CC=g++ --std=c++14

LFLAGS=
CFLAGS=

SOURCES= src/program/thundershell.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=thundershell


release: $(SOURCES) $(OBJECTS)
	$(CC) -o $(EXECUTABLE) $(OBJECTS) $(LFLAGS)

clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)

debug: CFLAGS += -DDEBUG
debug: clean release

.cc.o:
	$(CC) -c -o $@ $< $(CFLAGS) 

