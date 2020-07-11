/*
**
**	$Filename:	rp.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Converting tool for Utah teapot import
**
**      (C) Copyright 2004  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#define no_DEBUG_VIEW

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"
#include "blz3/base/b3Array.h"
#include "blz3/raytrace/b3BBox.h"
#include "blz3/raytrace/b3Shape.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

struct b3_patch
{
	b3_index m_Indices[16];
};

class b3Patch
{
	b3Array<b3_patch>   m_Patches;
	b3Array<b3_vector>  m_Vertices;
	b3BBox       *      m_BBox;

public:
	b3Patch(const char * filename)
	{
		FILE * patchfile;
		char  line[1024];
		b3_count count, i;

		m_BBox = null;
		patchfile = fopen(filename, B3_TREAD);
		if(patchfile != null)
		{
			b3PrintF(B3LOG_NORMAL, "Reading %s...\n", filename);
			if(b3ReadLine(line, sizeof(line), patchfile))
			{
				count = 0;
				sscanf(line, " %ld", &count);
				b3PrintF(B3LOG_NORMAL, "Trying to read %d patches...\n", count);
				for(i = 0; i < count; i++)
				{
					if(b3ReadLine(line, sizeof(line), patchfile))
					{
						b3_patch patch;

						if(sscanf(line, "%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld",
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
							b3PrintF(B3LOG_NORMAL, "Cannot parse line:\n %s\n", line);
						}
					}
				}
			}
			else
			{
				b3PrintF(B3LOG_NORMAL, "Cannot parse count line:\n %s\n", line);
			}

			if(b3ReadLine(line, sizeof(line), patchfile))
			{
				count = 0;
				sscanf(line, " %ld", &count);
				b3PrintF(B3LOG_NORMAL, "Trying to read %d vertices...\n", count);
				for(i = 0; i < count; i++)
				{
					if(b3ReadLine(line, sizeof(line), patchfile))
					{
						b3_vector vertex;

						if(sscanf(line, "%f,%f,%f",
								&vertex.x, &vertex.y, &vertex.z) == 3)
						{
							b3_f64 factor = 20;

							vertex.x *= factor;
							vertex.y *= factor;
							vertex.z *= factor;
							m_Vertices.b3Add(vertex);
						}
					}
				}
			}
			else
			{
				b3PrintF(B3LOG_NORMAL, "Cannot parse count line:\n %s\n", line);
			}

			fclose(patchfile);
			b3PrintF(B3LOG_NORMAL, "Read %d patches and %d vertices.\n",
				m_Patches.b3GetCount(), m_Vertices.b3GetCount());
		}
	}

	~b3Patch()
	{
		if(m_BBox != null)
		{
			delete m_BBox;
		}
	}

	void b3Create()
	{
		b3PrintF(B3LOG_NORMAL, "Creating Blizzard objects...\n");

#if 1
		if((m_Patches.b3GetCount() == 32) && (m_Vertices.b3GetCount() == 306))
		{
			b3CreateTeapot();
		}
		else
#endif
		{
			b3CreateObject();
		}
	}

	void b3SaveObject(const char * filename)
	{
		b3PrintF(B3LOG_NORMAL, "Saving Blizzard object %s...\n", filename);

		b3Dir::b3SplitFileName(filename, null, m_BBox->m_BoxName);
		b3Dir::b3RemoveExt(m_BBox->m_BoxName);

		if(m_BBox != null)
		{
			b3World world;
			b3File  file(filename, B_WRITE);

			world.b3SetFirst(m_BBox);
			b3BBox::b3Recount(world.b3GetHead());
			world.b3Write(&file);
			m_BBox = null;
		}
	}

private:
	static b3_bool b3ReadLine(char * line, size_t size, FILE * in)
	{
		line[0] = 0;
		return fgets(line, size, in) != null;
	}

	void b3CreateTeapot()
	{
		b3SplineShape * shape;

		b3PrintF(B3LOG_NORMAL, "Creating teapot...\n");
		m_BBox = new b3BBox(BBOX);
		strcpy(m_BBox->m_BoxName, "Teapot");

		//// Body
		shape = new b3SplineShape(SPLINES_CYL);
		shape->b3Init(3, 3, 12, 13);
		m_BBox->b3GetShapeHead()->b3Append(shape);
		b3CreateControls(shape, 0, 12, 4);
		b3CreateControls(shape, 28, 32, 4, 9, true);

		//// Handle
		shape = new b3SplineShape(SPLINES_CYL);
		shape->b3Init(3, 3, 6, 7);
		m_BBox->b3GetShapeHead()->b3Append(shape);
		b3CreateControls(shape, 12, 16, 2);

		//// Spout
		shape = new b3SplineShape(SPLINES_CYL);
		shape->b3Init(3, 3, 6, 7);
		m_BBox->b3GetShapeHead()->b3Append(shape);
		b3CreateControls(shape, 16, 20, 2);

		//// Lid
		shape = new b3SplineShape(SPLINES_CYL);
		shape->b3Init(3, 3, 12, 7);
		m_BBox->b3GetShapeHead()->b3Append(shape);
		b3CreateControls(shape, 20, 28, 4);
	}

	void b3CreateControls(
		b3SplineShape * shape,
		b3_count       start,
		b3_count       end,
		b3_count       step,
		b3_index       index  = 0,
		b3_bool        invert = false)
	{
		b3_count xp, xo, xi, x, i;
		b3_index pos;

		for(i = 0; i < shape->m_Spline[0].m_KnotNum; i++)
		{
			shape->m_Spline[0].m_Knots[i] = i / shape->m_Spline[0].m_Degree;
		}

		for(i = 0; i < shape->m_Spline[1].m_KnotNum; i++)
		{
			shape->m_Spline[1].m_Knots[i] = (i - 1) / shape->m_Spline[1].m_Degree;
		}
		shape->m_Spline[1].m_Knots[i - 1] = shape->m_Spline[1].m_Knots[i - 2];

		index *= shape->m_Spline[1].m_Offset;
		for(i = start; i < end; i += step)
		{
			for(xp = 0; xp < 4; xp++)
			{
				x = 0;
				for(xo = 0; xo < 4; xo++)
				{
					for(xi = 1; xi < 4; xi++)
					{
						pos = xp * 4 + xi;
						if(invert)
						{
							shape->m_Controls[x + index] = m_Vertices[m_Patches[i - xo + 3].m_Indices[15 - pos] - 1];
						}
						else
						{
							shape->m_Controls[x + index] = m_Vertices[m_Patches[i + xo].m_Indices[pos] - 1];
						}
						x++;
					}
				}
				index += shape->m_Spline[1].m_Offset;
			}
			index -= shape->m_Spline[1].m_Offset;
		}
	}

	void b3CreateObject()
	{
		b3SplineShape * shape;
		b3_count       i, x, y;
		b3_index       index, pos;

		m_BBox = new b3BBox(BBOX);

		b3PrintF(B3LOG_NORMAL, "Creating object...\n");
		for(i = 0; i < m_Patches.b3GetCount(); i++)
		{
			shape = new b3SplineShape(SPLINES_AREA);
			shape->b3Init(3, 3, 4, 4);
			shape->m_Spline[0].b3ToBezier();
			shape->m_Spline[1].b3ToBezier();
			m_BBox->b3GetShapeHead()->b3Append(shape);

			index = 0;
			pos   = 0;
			for(y = 0; y < shape->m_Spline[1].m_ControlNum; y++)
			{
				for(x = 0; x < shape->m_Spline[0].m_ControlNum; x += shape->m_Spline[0].m_Offset)
				{
					shape->m_Controls[x + index] = m_Vertices[m_Patches[i].m_Indices[pos++] - 1];
				}
				index += shape->m_Spline[1].m_Offset;
			}
		}
	}
};

int main(int argc, char * argv[])
{
	if(argc == 3)
	{
		b3Patch patch = argv[1];

		patch.b3Create();
		patch.b3SaveObject(argv[2]);
	}
	else
	{
		printf("USAGE:\n%s patchfile BOD-File\n", argv[0]);
	}
	return EXIT_SUCCESS;
}
