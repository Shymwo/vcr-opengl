LIBS=-lGL -lglut -lGLEW -ljpeg
CC=g++

SOURCES=vcr.cpp tga.cpp shaderprogram.cpp water.cpp
HEADERS=tga.h teapot.h shaderprogram.h water.h
OBJECTS=$(SOURCES:.cpp=.o)

all: vcr

vcr: $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp $(HEADERS)
	$(CC) -c $< -o $@
	
clean:
	-rm -rf *.o vcr
