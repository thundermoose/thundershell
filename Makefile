CC=g++ --std=c++14

LFLAGS=
CFLAGS=

SOURCES= src/program/thundershell.cc src/shell_reader/shell_reader.cc src/single_particles/single_particles.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=thundershell


release: $(SOURCES) $(OBJECTS)
	$(CC) -o $(EXECUTABLE) $(OBJECTS) $(LFLAGS)

clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)

debug: CFLAGS += -DDEBUG -ggdb
debug: LFlags += -ggdb
debug: clean release

.cc.o:
	$(CC) -c -o $@ $< $(CFLAGS) 

