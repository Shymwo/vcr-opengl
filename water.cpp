#include "water.h"

// Water animation function

float Water::z (float x, float y, float t)
{
  return (2 * sinf (20 * sqrtf (pow(x,2) + pow(y,2)) - 4 * t) / 200);
}

// Water Texture loading

Water::Water (GLuint tex) {
	texture = tex;

//	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glEnable (GL_TEXTURE_GEN_S);
	glEnable (GL_TEXTURE_GEN_T);
	glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
}

/*
** Function called to update rendering
*/
void	Water::displayWater (void)
{
  const float delta = 2. / RESOLUTION;
  unsigned int i,j;
  float x,y,l;
  unsigned int indice,preindice;
  const float t = glutGet (GLUT_ELAPSED_TIME) / 1000.;

  vec3 v1,v2,v3,va,vb,n;

//  glLoadIdentity ();

  /* Vertices */
  for (j = 0; j < RESOLUTION; j++) {
      y = (j + 1) * delta - 1;
      for (i = 0; i <= RESOLUTION; i++) {
		  indice = 2 * (i + j * (RESOLUTION + 1));

		  x = i * delta - 1;
		  vertices[indice + 1] = vec3( x, y, z (x, y, t));

		  if (j != 0) {
			  /* Values were computed during the previous loop */
			  preindice = 2 * (i + (j - 1) * (RESOLUTION + 1));
			  vertices[indice] = vertices[preindice + 1];
		  } else {
			  vertices[indice] = vertices[indice + 1];
		  }
      }
  }

  /* Normals */
  for (j = 0; j < RESOLUTION; j++) {
	  for (i = 0; i <= RESOLUTION; i++) {
		indice = 2 * (i + j * (RESOLUTION + 1));

		v1 = vertices[indice+1];
		v2 = vertices[indice];
		v3 = vertices[indice+2];

		va = v2 - v1;
		vb = v3 - v1;

		n = cross(vb,va);

		l = glm::length(n);

		if (l != 0) {
			l = 1 / l;
			normals[indice + 1] = n * l;
		} else {
			normals[indice + 1] = vec3(0,0,1);
		}


		if (j != 0) {
			/* Values were computed during the previous loop */
			preindice = 2 * (i + (j - 1) * (RESOLUTION + 1));
			normals[indice] = normals[preindice + 1];
		} else {
			normals[indice] = normals[indice + 1];
		}
	  }
  }

  //texture coords
  for (j = 0; j < RESOLUTION; j++) {
	  y = (j + 1) * delta - 1;
	  for (i = 0; i <= RESOLUTION; i++) {
		indice = 2 * (i + j * (RESOLUTION + 1));

		//some code here
		x = i * delta - 1;
		texCoords[indice + 1] = vec2(0.5, 0.5);

		if (j != 0) {
			/* Values were computed during the previous loop */
			preindice = 2 * (i + (j - 1) * (RESOLUTION + 1));
			texCoords[indice] = texCoords[preindice + 1];
		} else {
			texCoords[indice] = texCoords[indice + 1];
		}
	  }
  }

  /* The water */

//  glEnable (GL_TEXTURE_2D);
//  glColor3f (1, 1, 1);
//  glEnableClientState (GL_NORMAL_ARRAY);
//  glEnableClientState (GL_VERTEX_ARRAY);
//  glNormalPointer (GL_FLOAT, 0, normals);
//  glVertexPointer (3, GL_FLOAT, 0, vertices);

  /* End */
//  glFlush ();
//  glutSwapBuffers ();
//  glutPostRedisplay ();
}
