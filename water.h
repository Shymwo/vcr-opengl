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
#include "tga.h"

#define	RESOLUTION 64

class Water {
private:
	GLuint texture;
	float z(float x, float y, float t);
public:
	float surface[6 * RESOLUTION * (RESOLUTION + 1)];
	float normal[6 * RESOLUTION * (RESOLUTION + 1)];

	Water(GLuint tex);
	void displayWater(void);
};

#endif
