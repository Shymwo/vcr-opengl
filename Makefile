LIBS=-lGL -lglut -lGLEW -ljpeg
CC=g++

SOURCES=tga.cpp water.cpp ship.cpp vcr.cpp
HEADERS=tga.h water.h ship.h
OBJECTS=$(SOURCES:.cpp=.o)

all: vcr

vcr: $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp $(HEADERS)
	$(CC) -c $< -o $@
	
clean:
	-rm -rf *.o vcr
