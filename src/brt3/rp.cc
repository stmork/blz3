#include "blz3/base/b3Array.h"
#include "blz3/raytrace/b3BBox.h"
#include "blz3/raytrace/b3Shape.h"

struct b3_patch
{
	b3_index m_Indices[16];
};

class b3Patch
{
	b3Array<b3_patch>  m_Patches;
	b3Array<b3_vector> m_Vertices;

public:
	b3Patch(const char *filename)
	{
		FILE *patchfile;
		char  line[1024];
		b3_count count,i;

		patchfile = fopen(filename,B3_TREAD);
		if (patchfile != null)
		{
			if (b3ReadLine(line,sizeof(line),patchfile))
			{
				count = atoi(line);
				for (i = 0;i < count;i++)
				{
					if (b3ReadLine(line,sizeof(line),patchfile))
					{
						b3_patch patch;

						if (sscanf(line,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
							&patch.m_Indices[ 0],
							&patch.m_Indices[ 1],
							&patch.m_Indices[ 2],
							&patch.m_Indices[ 3],
							&patch.m_Indices[ 4],
							&patch.m_Indices[ 5],
							&patch.m_Indices[ 6],
							&patch.m_Indices[ 7],
							&patch.m_Indices[ 8],
							&patch.m_Indices[ 9],
							&patch.m_Indices[10],
							&patch.m_Indices[11],
							&patch.m_Indices[12],
							&patch.m_Indices[13],
							&patch.m_Indices[14],
							&patch.m_Indices[15]) == 16)
						{
							m_Patches.b3Add(patch);
						}
					}
				}
			}

			if (b3ReadLine(line,sizeof(line),patchfile))
			{
				count = atoi(line);
				for (i = 0;i < count;i++)
				{
					if (b3ReadLine(line,sizeof(line),patchfile))
					{
						b3_vector vertex;

						if (sscanf(line,"%f,%f,%f",
							&vertex.x,&vertex.y,&vertex.z) == 3)
						{
							m_Vertices.b3Add(vertex);
						}
					}
				}
			}
			fclose(patchfile);
		}
	}

private:
	static b3_bool b3ReadLine(char *line,size_t size,FILE *in)
	{
		line[0] = 0;
		fgets(line,size,in);
	}
};

int main(int argc,char *argv[])
{
	if (argc == 3)
	{
		b3Patch patch = argv[1];
	}
	else
	{
		printf("USAGE:\n%s patchfile BOD-File\n",argv[0]);
	}
	return 0;
}
