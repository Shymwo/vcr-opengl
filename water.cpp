#include "water.h"

#define	RESOLUTION 64
#define SIZE 8.

vec3 wVertices[2 * RESOLUTION * (RESOLUTION + 1)];
vec3 wNormals[2 * RESOLUTION * (RESOLUTION + 1)];

const float delta = SIZE / RESOLUTION;
const unsigned int leng = 2 * (RESOLUTION + 1);
unsigned int i,j;
float x,y,l;
unsigned int indice,preindice;

vec3 v1,v2,v3,va,vb,n;

// Water animation function

float z (float x, float y, float t)
{
  x=x-2; y=y-4;
  return (2 * sinf (20 * sqrtf (pow(x,2) + pow(y,2)) - 4 * t) / 200);
}

void updateVertices(float t) {
  /* Vertices */
  for (j = 0; j <= RESOLUTION; j++) {
	  y = j * delta - SIZE/2;
	  for (i = 0; i <= RESOLUTION; i++) {
		  indice = 2 * (i + j * (RESOLUTION + 1));

		  x = i * delta - SIZE/2;
		  wVertices[indice + 1] = vec3( x, z (x, y, t), y);

		  if (j != 0) {
			  /* Values were computed during the previous loop */
			  preindice = 2 * (i + (j - 1) * (RESOLUTION + 1));
			  wVertices[indice] = wVertices[preindice + 1];
		  } else {
			  wVertices[indice] = wVertices[indice + 1];
		  }
	  }
  }
}

void updateNormals(float t) {
  /* Normals */
  for (j = 0; j < RESOLUTION; j++) {
	  for (i = 0; i <= RESOLUTION; i++) {
		indice = 2 * (i + j * (RESOLUTION + 1));

		v1 = wVertices[indice+1]; v2 = wVertices[indice]; v3 = wVertices[indice+2];

		va = v2 - v1; vb = v3 - v1;
		n = cross(vb,va); l = glm::length(n);

		if (l != 0) {
			l = 1 / l;
			wNormals[indice + 1] = n * l;
		} else
			wNormals[indice + 1] = vec3(0,1,0);

		if (j != 0) {
			/* Values were computed during the previous loop */
			preindice = 2 * (i + (j - 1) * (RESOLUTION + 1));
			wNormals[indice] = wNormals[preindice + 1];
		} else {
			wNormals[indice] = wNormals[indice + 1];
		}
	  }
  }
}

/*
** Function called to update rendering
*/
void displayWater (float t)
{
	glEnable (GL_TEXTURE_GEN_S);
	glEnable (GL_TEXTURE_GEN_T);
	glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

  updateVertices(t);
  updateNormals(t);

  glEnableClientState (GL_NORMAL_ARRAY);
  glEnableClientState (GL_VERTEX_ARRAY);
  glNormalPointer (GL_FLOAT, 0, wNormals);
  glVertexPointer (3, GL_FLOAT, 0, wVertices);

  for (i = 0; i < RESOLUTION; i++)
    glDrawArrays (GL_TRIANGLE_STRIP, i * leng, leng);

	glDisable (GL_TEXTURE_GEN_S);
	glDisable (GL_TEXTURE_GEN_T);

	glDisableClientState (GL_NORMAL_ARRAY);
	glDisableClientState (GL_VERTEX_ARRAY);

}
