CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lglfw -lGL -lGLEW
SOURCES=$(wildcard src/*.cpp) $(wildcard src/engine/*.cpp) $(wildcard src/engine/components/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=my_app

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)
