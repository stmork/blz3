#include "blz3/system/b3Log.h"
#include "blz3/base/b3Render.h"
#include "blz3/base/b3Matrix.h"
#include "GL/glut.h"

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

	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutCreateWindow("Matrix Test");
	glClearColor(0.7f,0.7f,1.0f,1.0f);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(1.0,2.0,3.0);

	b3RenderContext::b3GetMatrix(B3_MATRIX_OBJECT,&gl);
	b3MatrixMove(null,&b3,&move);

	b3PrintF(B3LOG_NORMAL,"GL Matrix\n");
	b3PrintMatrix(&gl.m11);
	b3PrintF(B3LOG_NORMAL,"Blizzard III Matrix\n");
	b3PrintMatrix(&b3.m11);
}

