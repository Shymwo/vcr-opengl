#include "water.h"

// Water animation function

float Water::z (float x, float y, float t)
{
  x=x-3; y=y+1;
  return (2 * sinf (20 * sqrtf (x*x + y*y) - 4 * t) / 200);
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
  const float t = glutGet (GLUT_ELAPSED_TIME) / 1000.;
  const float delta = 2. / RESOLUTION;
  const unsigned int length = 2 * (RESOLUTION + 1);
  const float xn = (RESOLUTION + 1) * delta + 1;
  unsigned int i,j;
  float x,y;
  unsigned int indice,preindice;

  /* Yes, I know, this is quite ugly... */
  float v1x,v1y,v1z;
  float v2x,v2y,v2z;
  float v3x,v3y,v3z;
  float vax,vay,vaz;
  float vbx,vby,vbz;
  float nx,ny,nz;
  float l;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity ();
  glTranslatef(0, 0, -4);
  glRotatef(15, 1, 0, 0);
  glRotatef(30, 0, 1, 0);

  /* Vertices */
  for (j = 0; j < RESOLUTION; j++)
    {
      y = (j + 1) * delta - 1;
      for (i = 0; i <= RESOLUTION; i++)
	{
	  indice = 6 * (i + j * (RESOLUTION + 1));

	  x = i * delta - 1;
	  surface[indice + 3] = x;
	  surface[indice + 4] = z (x, y, t);
	  surface[indice + 5] = y;
	  if (j != 0)
	    {
	      /* Values were computed during the previous loop */
	      preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
	      surface[indice] = surface[preindice + 3];
	      surface[indice + 1] = surface[preindice + 4];
	      surface[indice + 2] = surface[preindice + 5];
	    }
	  else
	    {
	      surface[indice] = x;
	      surface[indice + 1] = z (x, -1, t);
	      surface[indice + 2] = -1;
	    }
	}
    }

  /* Normals */
  for (j = 0; j < RESOLUTION; j++)
    for (i = 0; i <= RESOLUTION; i++)
      {
	indice = 6 * (i + j * (RESOLUTION + 1));

	v1x = surface[indice + 3];
	v1y = surface[indice + 4];
	v1z = surface[indice + 5];

	v2x = v1x;
	v2y = surface[indice + 1];
	v2z = surface[indice + 2];

	if (i < RESOLUTION)
	  {
	    v3x = surface[indice + 9];
	    v3y = surface[indice + 10];
	    v3z = v1z;
	  }
	else
	  {
	    v3x = xn;
	    v3y = z (xn, v1z, t);
	    v3z = v1z;
	  }

	vax =  v2x - v1x;
	vay =  v2y - v1y;
	vaz =  v2z - v1z;

	vbx = v3x - v1x;
	vby = v3y - v1y;
	vbz = v3z - v1z;

	nx = (vby * vaz) - (vbz * vay);
	ny = (vbz * vax) - (vbx * vaz);
	nz = (vbx * vay) - (vby * vax);

	l = sqrtf (nx * nx + ny * ny + nz * nz);
	if (l != 0)
	  {
	    l = 1 / l;
	    normal[indice + 3] = nx * l;
	    normal[indice + 4] = ny * l;
	    normal[indice + 5] = nz * l;
	  }
	else
	  {
	    normal[indice + 3] = 0;
	    normal[indice + 4] = 1;
	    normal[indice + 5] = 0;
	  }


	if (j != 0)
	  {
	    /* Values were computed during the previous loop */
	    preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
	    normal[indice] = normal[preindice + 3];
	    normal[indice + 1] = normal[preindice + 4];
	    normal[indice + 2] = normal[preindice + 5];
	  }
	else
	  {
/* 	    v1x = v1x; */
	    v1y = z (v1x, (j - 1) * delta - 1, t);
	    v1z = (j - 1) * delta - 1;

/* 	    v3x = v3x; */
	    v3y = z (v3x, v2z, t);
	    v3z = v2z;

	    vax = v1x - v2x;
	    vay = v1y - v2y;
	    vaz = v1z - v2z;

	    vbx = v3x - v2x;
	    vby = v3y - v2y;
	    vbz = v3z - v2z;

	    nx = (vby * vaz) - (vbz * vay);
	    ny = (vbz * vax) - (vbx * vaz);
	    nz = (vbx * vay) - (vby * vax);

	    l = sqrtf (nx * nx + ny * ny + nz * nz);
	    if (l != 0)
	      {
		l = 1 / l;
		normal[indice] = nx * l;
		normal[indice + 1] = ny * l;
		normal[indice + 2] = nz * l;
	      }
	    else
	      {
		normal[indice] = 0;
		normal[indice + 1] = 1;
		normal[indice + 2] = 0;
	      }
	  }
      }

  /* The water */
  glEnable (GL_TEXTURE_2D);
  glColor3f (1, 1, 1);
  glEnableClientState (GL_NORMAL_ARRAY);
  glEnableClientState (GL_VERTEX_ARRAY);
  glNormalPointer (GL_FLOAT, 0, normal);
  glVertexPointer (3, GL_FLOAT, 0, surface);
  for (i = 0; i < RESOLUTION; i++)
    glDrawArrays (GL_TRIANGLE_STRIP, i * length, length);

  /* End */
  glFlush ();
//  glutSwapBuffers ();
  glutPostRedisplay ();
}
