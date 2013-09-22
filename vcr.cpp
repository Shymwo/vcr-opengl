#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <vector>
#include "tga.h"
#include "water.h"
#include "ship.h"

using namespace glm;

//Ustawienia okna i rzutowania
int windowPositionX=100;
int windowPositionY=100;
int windowWidth=400;
int windowHeight=400;

//Zmienne do animacji
float t;

//mouse
static int left_click = GLUT_UP;
static int right_click = GLUT_UP;
static int xold = 0;
static int yold = 0;
static float rotate_x = 60;
static float rotate_y = 30;
static float translate_z = 40;

//textures
GLuint wTexture;
GLuint sTexture;

GLuint readTexture(char* filename) {
	GLuint tex;
	TGAImg img;
	glActiveTexture(GL_TEXTURE0);
	if (img.Load(filename)==IMG_OK) {
		glGenTextures(1,&tex); //Zainicjuj uchwyt tex
		glBindTexture(GL_TEXTURE_2D,tex); //Przetwarzaj uchwyt tex
		if (img.GetBPP()==24) //Obrazek 24bit
			glTexImage2D(GL_TEXTURE_2D,0,3,img.GetWidth(),img.GetHeight(),0,
			GL_RGB,GL_UNSIGNED_BYTE,img.GetImg());
		else if (img.GetBPP()==32) //Obrazek 32bit
			glTexImage2D(GL_TEXTURE_2D,0,4,img.GetWidth(),img.GetHeight(),0,
			GL_RGBA,GL_UNSIGNED_BYTE,img.GetImg());
		else {
			printf("Nieobsługiwany format obrazka w pliku: %s \n",filename);
		}
	} else {
		printf("Błąd przy wczytywaniu pliku: %s\n",filename);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	return tex;
}

//Procedura rysująca
void displayFrame() {
	//Wyczyść bufor kolorów i bufor głębokości
	glClearColor(0.6,0.7,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Wylicz macierz rzutowania
	mat4 P=perspective(translate_z, (float)windowWidth/(float)windowHeight, 1.0f, 100.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(P));

	//Wylicz macierz widoku
	mat4 V=lookAt(vec3(0.0f,0.0f,4.0f),vec3(0.0f,0.0f,0.0f),vec3(0.0f,1.0f,0.0f));

	//Wylicz macierz modelu
	mat4 M=rotate(mat4(1.0f),rotate_y,vec3(1,0,0));
	M=rotate(M,rotate_x,vec3(0,1,0));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(V*M));

	//Narysuj wodę
	glBindTexture(GL_TEXTURE_2D,wTexture);
	displayWater(t);


	//Narysuj statek
	mat4 S = scale(mat4(1.0f), vec3(0.0002,0.0002,0.0002));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(V*M*S));
	glBindTexture(GL_TEXTURE_2D,sTexture);
	drawShip();

	//Tylny bufor na przedni
	glutSwapBuffers();
}

//Procedura uruchamiana okresowo. Robi animację.
void nextFrame(void) {
	t = glutGet (GLUT_ELAPSED_TIME) / 1000.;

	glutPostRedisplay();
}

//Procedura wywoływana przy zmianie rozmiaru okna
void changeSize(int w, int h) {
	//Ustawienie wymiarow przestrzeni okna
	glViewport(0,0,w,h);

	//Zapamiętanie nowych wymiarów okna dla poprawnego wyliczania macierzy rzutowania
	windowWidth=w;
	windowHeight=h;
}

void MouseFunc(int button, int state, int x, int y)
{
  if (GLUT_LEFT_BUTTON == button) {
    left_click = state;
  }
  if (GLUT_RIGHT_BUTTON == button) {
	right_click = state;
  }

  xold = x;
  yold = y;
}

void MotionFunc(int x, int y)
{
  if (GLUT_DOWN == left_click)
    {
      rotate_y = rotate_y + (y - yold) / 2;
      rotate_x = rotate_x + (x - xold) / 2;
      if (rotate_y < 15)
	rotate_y = 15;
      if (rotate_y > 90)
	rotate_y = 90;
      glutPostRedisplay ();
    }
  if (GLUT_DOWN == right_click)
    {
      rotate_x = rotate_x + (x - xold) / 5;
      translate_z = translate_z + (yold - y) / 5;
      if (translate_z < 0)
	translate_z = 0;
      if (translate_z > 80)
	translate_z = 80;
      glutPostRedisplay ();
    }
  xold = x;
  yold = y;
}

//Procedura inicjująca biblotekę glut
void initGLUT(int *argc, char** argv) {
	glutInit(argc,argv); //Zainicjuj bibliotekę GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //Alokuj bufory kolorów (podwójne buforowanie) i bufor kolorów

	glutInitWindowPosition(windowPositionX,windowPositionY); //Wskaż początkową pozycję okna
	glutInitWindowSize(windowWidth,windowHeight); //Wskaż początkowy rozmiar okna
	glutCreateWindow("Voyage Century Reborn"); //Utwórz okno i nadaj mu tytuł

	glutReshapeFunc(changeSize); //Zarejestruj procedurę changeSize jako procedurę obsługująca zmianę rozmiaru okna
	glutDisplayFunc(displayFrame); //Zarejestruj procedurę displayFrame jako procedurę obsługująca odświerzanie okna
	glutIdleFunc(nextFrame); //Zarejestruj procedurę nextFrame jako procedurę wywoływaną najczęścięj jak się da (animacja)

	glutMouseFunc(&MouseFunc);
	glutMotionFunc(&MotionFunc);
}

void initGLEW() {
	GLenum err=glewInit();
	if (GLEW_OK!=err) {
		fprintf(stderr,"%s\n",glewGetErrorString(err));
		exit(1);
	}

}

void initOpenGL() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
}

void initTextures() {
	// Water texture
	wTexture = readTexture ( (char *) "images/reflection.tga");

	// Ship texture
	sTexture = readTexture ( (char *) "images/woodalt.tga");
}

void freeTextures() {
	glDeleteTextures(1,&wTexture);
	glDeleteTextures(1,&sTexture);
}

void initModels() {
	loadShip();
}

int main(int argc, char** argv) {
	initGLUT(&argc,argv);
	initGLEW();
	initOpenGL();
	initTextures();
	initModels();

	glutMainLoop();

	freeTextures();
	return 0;
}
