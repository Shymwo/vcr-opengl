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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "tga.h"

using namespace glm;

#define	RESOLUTION 32

class Water {
private:
	GLuint texture;
	float z(float x, float y, float t);
public:
	float vertices[6 * RESOLUTION * (RESOLUTION + 1)];
	float normals[6 * RESOLUTION * (RESOLUTION + 1)];

	Water(GLuint tex);
	void displayWater(void);
};

#endif
