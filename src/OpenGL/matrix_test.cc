/*
**
**	$Filename:	matrix_test.cc $
**	$Release:	Dortmund 2001, 2002 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Testing matrix manipulation
**
**      (C) Copyright 2001, 2002  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/
  
#include "blz3/system/b3Log.h"
#include "blz3/base/b3Render.h"
#include "blz3/base/b3Matrix.h"

#ifdef BLZ3_USE_OPENGL
#include "GL/glut.h"
#endif

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.2  2002/08/03 18:05:10  sm
**      - Cleaning up BL3_USE_OPENGL for linux/m68k without OpenGL
**      - Moved b3PrepareInfo into b3Scene class as member. This
**        saves memory allocation calls and is an investment into
**        faster Lines III object transformation.
**
**
*/

static void b3PrintMatrix(b3_f32 *values)
{
	b3_index i,k;

	for (i = 0;i < 4;i++)
	{
		for(k = 0;k < 4;k++)
		{
			b3PrintF(B3LOG_NORMAL," %3.3f",*values++);
		}
		b3PrintF(B3LOG_NORMAL,"\n");
	}
}


int main(int argc,char *argv[])
{
	b3_matrix b3;
	b3_matrix gl;
	b3_vector move;

	move.x = 1;
	move.y = 2;
	move.z = 3;

#ifdef BLZ3_USE_OPENGL
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutCreateWindow("Matrix Test");
	glClearColor(0.7f,0.7f,1.0f,1.0f);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(1.0,2.0,3.0);
#endif

	b3RenderContext::b3GetMatrix(B3_MATRIX_OBJECT,&gl);
	b3MatrixMove(null,&b3,&move);

	b3PrintF(B3LOG_NORMAL,"GL Matrix\n");
	b3PrintMatrix(&gl.m11);
	b3PrintF(B3LOG_NORMAL,"Blizzard III Matrix\n");
	b3PrintMatrix(&b3.m11);
}
