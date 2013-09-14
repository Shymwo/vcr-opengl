#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include "tga.h"
#include "shaderprogram.h"
#include "teapot.h"
#include "water.h"

using namespace glm;

//Macierze
mat4  matP;//rzutowania
mat4  matV;//widoku
mat4  matM;//modelu

//Ustawienia okna i rzutowania
int windowPositionX=100;
int windowPositionY=100;
int windowWidth=400;
int windowHeight=400;
float cameraAngle=45.0f;

//Zmienne do animacji
float speed=120; //120 stopni/s
int lastTime=0;
float anglee=0;

//mouse
static int	left_click = GLUT_UP;
static int	right_click = GLUT_UP;
static int	xold = 0;
static int	yold = 0;
static float	rotate_x = 30;
static float	rotate_y = 15;
static float	translate_z = 40;

//Uchwyty na shadery
ShaderProgram *shaderProgram; //Wskaźnik na obiekt reprezentujący program cieniujący.

//Uchwyty na VAO i bufory wierzchołków
GLuint vao;
GLuint bufVertices; //Uchwyt na bufor VBO przechowujący tablicę współrzędnych wierzchołków
GLuint bufColors;  //Uchwyt na bufor VBO przechowujący tablicę kolorów
GLuint bufNormals; //Uchwyt na bufor VBO przechowujący tablicę wektorów normalnych
GLuint bufTexCoords; //Uchwyt na bufor VBO przechowujący tablicę współrzędnych teksturowania

//"Model" który rysujemy. Dane wskazywane przez poniższe wskaźniki i o zadanej liczbie wierzchołków są później wysowane przez program.
//W programie są dwa modele, z których jeden można wybrać komentując/odkomentowując jeden z poniższych fragmentów.

//Czajnik
float *vertices=teapotVertices;
float *colors=teapotColors;
float *normals=teapotNormals2;
float *texCoords=teapotTexCoords;
int vertexCount=teapotVertexCount;

GLuint tex0;
GLuint tex1;
GLuint water_tex;

Water *water;


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

//Procedura rysująca jakiś obiekt. Ustawia odpowiednie parametry dla vertex shadera i rysuje.
void drawObject() {
	//Włączenie programu cieniującego, który ma zostać użyty do rysowania
	//W tym programie wystarczyłoby wywołać to raz, w setupShaders, ale chodzi o pokazanie,
	//że mozna zmieniać program cieniujący podczas rysowania jednej sceny
	shaderProgram->use();

	//Przekaż do shadera macierze P,V i M.
	//W linijkach poniżej, polecenie:
	//  shaderProgram->getUniformLocation("P")
	//pobiera numer slotu odpowiadającego zmiennej jednorodnej o podanej nazwie
	//UWAGA! "P" w powyższym poleceniu odpowiada deklaracji "uniform mat4 P;" w vertex shaderze,
	//a matP w glm::value_ptr(matP) odpowiada deklaracji  "glm::mat4 matP;" TYM pliku.
	//Cała poniższa linijka przekazuje do zmiennej jednorodnej P w vertex shaderze dane ze zmiennej matP
	//zadeklarowanej globalnie w tym pliku.
	//Pozostałe polecenia działają podobnie.
	glUniformMatrix4fv(shaderProgram->getUniformLocation((char *)"P"),1, false, value_ptr(matP));
	glUniformMatrix4fv(shaderProgram->getUniformLocation((char *)"V"),1, false, value_ptr(matV));
	glUniformMatrix4fv(shaderProgram->getUniformLocation((char *)"M"),1, false, value_ptr(matM));
	glUniform1i(shaderProgram->getUniformLocation((char *)"textureMap0"),0); //skojarzenie zmiennej jednorodnej textureMap0 z zerową jednostką teksturującą
	glUniform1i(shaderProgram->getUniformLocation((char *)"textureMap1"),1); //skojarzenie zmiennej jednorodnej textureMap1 z pierwszą jednostką teksturującą

	//Związanie tekstury o uchwycie tex0 z zerową jednostką teksturującą
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,tex0);
	//Związanie tekstury o uchwycie tex1 z pierwszą jednostką teksturującą
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,tex1);

	//Uaktywnienie VAO i tym samym uaktywnienie predefiniowanych w tym VAO powiązań slotów atrybutów z tablicami z danymi
	glBindVertexArray(vao);

	//Narysowanie obiektu
	glDrawArrays(GL_TRIANGLES,0,vertexCount);

	//Posprzątanie po sobie (niekonieczne w sumie jeżeli korzystamy z VAO dla każdego rysowanego obiektu)
	glBindVertexArray(0);
}

//Procedura rysująca
void displayFrame() {
	//Wyczyść bufor kolorów i bufor głębokości
	glClearColor(0.6,0.7,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Wylicz macierz rzutowania
	matP=perspective(translate_z, (float)windowWidth/(float)windowHeight, 1.0f, 100.0f);

	//Wylicz macierz widoku
	matV=lookAt(vec3(0.0f,0.0f,4.0f),vec3(0.0f,0.0f,0.0f),vec3(0.0f,1.0f,0.0f));

	//Wylicz macierz modelu
	matM=rotate(mat4(1.0f),rotate_y,vec3(1,0,0));
	matM=rotate(matM,rotate_x,vec3(0,1,0));

	//Narysuj wodę
	water->displayWater();

	//Narysuj obiekt
	//drawObject();

	//Tylny bufor na przedni
	glutSwapBuffers();
}

GLuint makeBuffer(void *data, int vertexCount, int vertexSize) {
	GLuint handle;

	glGenBuffers(1,&handle);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), który będzie zawierał tablicę danych
	glBindBuffer(GL_ARRAY_BUFFER,handle);  //Uaktywnij wygenerowany uchwyt VBO
	glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, GL_STATIC_DRAW);//Wgraj tablicę do VBO

	return handle;
}

//Procedura tworząca bufory VBO zawierające dane z tablic opisujących rysowany obiekt.
void setupVBO() {
	bufVertices=makeBuffer(vertices, vertexCount, sizeof(float)*4); //Współrzędne wierzchołków
	bufColors=makeBuffer(colors, vertexCount, sizeof(float)*4);//Kolory wierzchołków
	bufNormals=makeBuffer(normals, vertexCount, sizeof(float)*4);//Wektory normalne wierzchołków
	//bufTexCoords=makeBuffer(texCoords, vertexCount, sizeof(float)*2);//Wspołrzędne teksturowania
}

void assignVBOtoAttribute(char* attributeName, GLuint bufVBO, int variableSize) {
	GLuint location=shaderProgram->getAttribLocation(attributeName); //Pobierz numery slotów dla atrybutu
	glBindBuffer(GL_ARRAY_BUFFER,bufVBO);  //Uaktywnij uchwyt VBO
	glEnableVertexAttribArray(location); //Włącz używanie atrybutu o numerze slotu zapisanym w zmiennej location
	glVertexAttribPointer(location,variableSize,GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu location mają być brane z aktywnego VBO
}

//Procedura tworząca VAO - "obiekt" OpenGL wiążący numery slotów atrybutów z buforami VBO
void setupVAO() {
	//Wygeneruj uchwyt na VAO i zapisz go do zmiennej globalnej
	glGenVertexArrays(1,&vao);

	//Uaktywnij nowo utworzony VAO
	glBindVertexArray(vao);

	assignVBOtoAttribute((char *)"vertex",bufVertices,4); //"vertex" odnosi się do deklaracji "in vec4 vertex;" w vertex shaderze
	assignVBOtoAttribute((char *)"color",bufColors,4); //"color" odnosi się do deklaracji "in vec4 color;" w vertex shaderze
	assignVBOtoAttribute((char *)"normal",bufNormals,4); //"normal" odnosi się do deklaracji "in vec4 normal;" w vertex shaderze
	assignVBOtoAttribute((char *)"texCoords0",bufTexCoords,2); //"texCoords0" odnosi się do deklarachu "in vec2 texCoords0;" w vertex shaderze

	glBindVertexArray(0);
}

//Procedura uruchamiana okresowo. Robi animację.
void nextFrame(void) {
	int actTime=glutGet(GLUT_ELAPSED_TIME);
	int interval=actTime-lastTime;
	lastTime=actTime;
	anglee+=speed*interval/1000.0;
	if (anglee>360) anglee-=360;
	glutPostRedisplay();
}

//Procedura wywoływana przy zmianie rozmiaru okna
void changeSize(int w, int h) {
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity ();
	gluPerspective (15, w / (float) h, 0.1, 15);

	//Ustawienie wymiarow przestrzeni okna
	glViewport(0,0,w,h);

	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();

	//Zapamiętanie nowych wymiarów okna dla poprawnego wyliczania macierzy rzutowania
	windowWidth=w;
	windowHeight=h;
}

/*
** Function called when a mouse button is hit
*/
void MouseFunc(int button, int state, int x, int y)
{
  if (GLUT_LEFT_BUTTON == button)
    left_click = state;
  if (GLUT_RIGHT_BUTTON == button)
    right_click = state;
  xold = x;
  yold = y;
}

/*
** Function called when the mouse is moved
*/
void MotionFunc(int x, int y)
{
  if (GLUT_DOWN == left_click)
    {
      rotate_y = rotate_y + (y - yold) / 2;
      rotate_x = rotate_x + (x - xold) / 2;
      if (rotate_y > 90)
	rotate_y = 90;
      if (rotate_y < -90)
	rotate_y = -90;
      glutPostRedisplay ();
    }
  if (GLUT_DOWN == right_click)
    {
      rotate_x = rotate_x + (x - xold) / 5;
      translate_z = translate_z + (yold - y) / 5;
      if (translate_z < 20)
	translate_z = 20;
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


//Procedura inicjująca bibliotekę glew
void initGLEW() {
	GLenum err=glewInit();
	if (GLEW_OK!=err) {
		fprintf(stderr,"%s\n",glewGetErrorString(err));
		exit(1);
	}

}


//Wczytuje tekstury
void initTextures() {
	tex0=readTexture( (char *) "images/metal.tga");
	tex1=readTexture( (char *) "images/metal_spec.tga");

	water_tex = readTexture ( (char *) "images/reflection.tga");
	water = new Water(water_tex);
}

//Wczytuje vertex shader i fragment shader i łączy je w program cieniujący
void setupShaders() {
	shaderProgram=new ShaderProgram((char *)"vshader.txt",NULL,(char *)"fshader.txt");
}

//procedura inicjująca różne sprawy związane z rysowaniem w OpenGL
void initOpenGL() {
	setupShaders();
	setupVBO();
	setupVAO();
	glEnable(GL_DEPTH_TEST);

}

//Zwolnij pamięć karty graficznej z shaderów i programu cieniującego
void cleanShaders() {
	delete shaderProgram;
}

void freeVBO() {
	glDeleteBuffers(1,&bufVertices);
	glDeleteBuffers(1,&bufColors);
	glDeleteBuffers(1,&bufNormals);
	glDeleteBuffers(1,&bufTexCoords);
}

void freeVAO() {
	glDeleteVertexArrays(1,&vao);
}

void freeTextures() {
	glDeleteTextures(1,&tex0);
	glDeleteTextures(1,&tex1);
	glDeleteTextures(1,&water_tex);
}

int main(int argc, char** argv) {
	initGLUT(&argc,argv);
	initGLEW();
	initOpenGL();
	initTextures();

	glutMainLoop();

	freeVAO();
	freeVBO();
	freeTextures();
	cleanShaders();
	return 0;
}
