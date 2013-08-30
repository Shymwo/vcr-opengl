#ifndef WATER_H
#define WATER_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <jerror.h>
#include <string.h>

#define	RESOLUTION 64

class Water {
private:
	GLuint texture;
	int	load_texture (const char * filename, unsigned char * dest, const int format, const unsigned int size);
	float z(const float x, const float y, const float t);
public:
	float surface[6 * RESOLUTION * (RESOLUTION + 1)];
	float normal[6 * RESOLUTION * (RESOLUTION + 1)];

	Water(char* reflection, char* alpha);
	void displayWater(void);
};

#endif
