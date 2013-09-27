#ifndef SHIP_H_
#define SHIP_H_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;
using namespace glm;

void loadShip(char* filename);
void drawShip();

void loadPaddle(char* filename);
void drawPaddle();

void deleteShipBuffers();

#endif
