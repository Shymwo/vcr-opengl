LIBS=-lGL -lglut -lGLEW
CC=g++

SOURCES=vcr.cpp tga.cpp shaderprogram.cpp
HEADERS=tga.h cube.h teapot.h shaderprogram.h
OBJECTS=$(SOURCES:.cpp=.o)

all: vcr

vcr: $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp $(HEADERS)
	$(CC) -c $< -o $@
	
clean:
	-rm -rf *.o vcr
