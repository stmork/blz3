#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"
#include "blz3/base/b3Array.h"
#include "blz3/raytrace/b3BBox.h"
#include "blz3/raytrace/b3Shape.h"

struct b3_patch
{
	b3_index m_Indices[16];
};

class b3Patch
{
	b3Array<b3_patch>   m_Patches;
	b3Array<b3_vector>  m_Vertices;
	b3BBox             *m_BBox;

public:
	b3Patch(const char *filename)
	{
		FILE *patchfile;
		char  line[1024];
		b3_count count,i;

		m_BBox = null;
		patchfile = fopen(filename,B3_TREAD);
		if (patchfile != null)
		{
			b3PrintF(B3LOG_NORMAL,"Reading %s...\n",filename);
			if (b3ReadLine(line,sizeof(line),patchfile))
			{
				count = 0;
				sscanf(line," %ld",&count);
				b3PrintF(B3LOG_NORMAL,"Trying to read %d patches...\n",count);
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
						else
						{
							b3PrintF(B3LOG_NORMAL,"Cannot parse line:\n %s\n",line);
						}
					}
				}
			}
			else
			{
				b3PrintF(B3LOG_NORMAL,"Cannot parse count line:\n %s\n",line);
			}

			if (b3ReadLine(line,sizeof(line),patchfile))
			{
				count = 0;
				sscanf(line," %ld",&count);
				b3PrintF(B3LOG_NORMAL,"Trying to read %d vertices...\n",count);
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
			else
			{
				b3PrintF(B3LOG_NORMAL,"Cannot parse count line:\n %s\n",line);
			}

			fclose(patchfile);
			b3PrintF(B3LOG_NORMAL,"Read %d patches and %d vertices.\n",
				m_Patches.b3GetCount(),m_Vertices.b3GetCount());
		}
	}

	void b3CreateObject()
	{
		b3SplineShape *shape;
		b3_count       i,x,y;
		b3_index       index,pos;

		m_BBox = new b3BBox(BBOX);
		for (i = 0;i < m_Patches.b3GetCount();i++)
		{
			shape = new b3SplineShape(SPLINES_AREA);
			shape->b3Init(3,3,4,4);
			shape->m_Spline[0].b3ToBezier();
			shape->m_Spline[1].b3ToBezier();

			index = 0;
			pos   = 0;
			for (y = 0;y < shape->m_Spline[1].control_num;y++)
			{
				for (x = 0;x < shape->m_Spline[0].control_num;x += shape->m_Spline[0].offset)
				{
					shape->m_Controls[x + index] = m_Vertices[m_Patches[i].m_Indices[pos++] - 1];
				}
				index += shape->m_Spline[1].offset;
			}
			m_BBox->b3GetShapeHead()->b3Append(shape);
		}
	}

	void b3SaveObject(const char *filename)
	{
		b3Dir::b3SplitFileName(filename,null,m_BBox->m_BoxName);
		b3Dir::b3RemoveExt(m_BBox->m_BoxName);

		if (m_BBox != null)
		{
			b3World world;
			b3File  file(filename,B_WRITE);

			world.b3SetFirst(m_BBox);
			b3BBox::b3Recount(world.b3GetHead());
			world.b3Write(&file);
			m_BBox = null;
		}
	}

private:
	static b3_bool b3ReadLine(char *line,size_t size,FILE *in)
	{
		line[0] = 0;
		return fgets(line,size,in) != null;
	}
};

int main(int argc,char *argv[])
{
	if (argc == 3)
	{
		b3Patch patch = argv[1];

		patch.b3CreateObject();
		patch.b3SaveObject(argv[2]);
	}
	else
	{
		printf("USAGE:\n%s patchfile BOD-File\n",argv[0]);
	}
	return 0;
}
