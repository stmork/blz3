/*
**
**	$Filename:	bhc.cc $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Creating a house from a description file
**
**      (C) Copyright 2003  Steffen A. Mork
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
  
#include "bhc.h"
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2003/07/09 12:57:18  sm
**	- Further house buiding...
**
**	Revision 1.1  2003/07/09 10:09:38  sm
**	- Changed brt3's default image file format to JPEG
**	- Increased default quality of JPEG images from 75 to 85
**	- Added house creating support ;-)
**	
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

const char *b3BHDParser::m_TokenNames[] =
{
	"begin",
	"end",
	"house",
	"level",
	"point",
	"room",
	"window",
	"door"
};

b3BHDParser::b3BHDParser(const char *filename)
{
	b3PrintF(B3LOG_DEBUG,"Parsing file %s\n",filename);
	m_Scene = null;
	m_LineNo = 0;
	m_BHD   = fopen(filename,B3_TREAD);
	m_Scale = 1;
	if (m_BHD == null)
	{
		throw b3ParseException("File not found");
	}
}

b3BHDParser::~b3BHDParser()
{
	fclose(m_BHD);
}

b3BHDParser::b3_bhd_token b3BHDParser::b3ReadLine()
{
	b3_size  max = sizeof(m_TokenNames) / sizeof(const char *);
	b3_size  i,len;

	m_Line[0] = 0;
	m_Pos     = 0;
	while(!feof(m_BHD))
	{
		if (fgets(m_Line,sizeof(m_Line),m_BHD) != null)
		{
			len = strlen(m_Line);
			while(isspace(m_Line[m_Pos]) && (m_Pos < len))
			{
				m_Pos++;
			}
			m_LineNo++;
			if ((m_Pos < len) && (m_Line[m_Pos] != '#'))
			{
				for (i = 0;i < max;i++)
				{
					if (strncmp(&m_Line[m_Pos],m_TokenNames[i],strlen(m_TokenNames[i])) == 0)
					{
						return (b3_bhd_token)i;
					}
				}
				throw b3ParseException("Syntax error",m_LineNo);
			}
		}
		else
		{
			throw b3ParseException("Line buffer overflow",m_LineNo);
		}
	}
	return TKN_UNKNOWN;
}

void b3BHDParser::b3CheckToken(b3_bhd_token token)
{
	if (b3ReadLine() != token)
	{
		throw b3ParseException("Unexpected token",m_LineNo);
	}
}

void b3BHDParser::b3ParseHouse()
{
	b3Path       name;
	b3ModellerInfo *info;
	b3CameraPart   *camera;
	b3_bhd_token token;
//	b3_vector     lower,upper;
	b3_vector     eye,view;

	b3PrintF(B3LOG_DEBUG,"Creating house...\n");
	b3CheckToken(TKN_HOUSE);
	
	m_Scene = new b3SceneMork(TRACEPHOTO_MORK);
	if (sscanf(&m_Line[m_Pos],"%*s %s %lf\n",(char *)name,&m_Scale) < 1)
	{
		throw b3ParseException("Invalid number of arguments",m_LineNo);
	}
	b3CheckToken(TKN_BEGIN);

	m_Scene->b3SetFilename(name);
	b3PrintF(B3LOG_DEBUG,"  Filename: %s\n",m_Scene->b3GetFilename());

	info = m_Scene->b3GetModellerInfo();
	info->m_Unit = B3_UNIT_CM;
	info->b3SetMeasure(B3_MEASURE_50);

	b3Vector::b3Init(&eye,-200.0,-750,170);
	b3Vector::b3Init(&view, 250,0,300);
	camera = m_Scene->b3GetCamera();
	camera->b3Orientate(&eye,&view,5,3.2,2.4);
	do
	{
		token = b3ReadLine();
		switch(token)
		{
		case TKN_LEVEL:
			b3ParseLevel();
			break;

		case TKN_END:
			b3BBox::b3Recount(m_Scene->b3GetBBoxHead());
//			m_Scene->b3ComputeBounds(&lower,&upper);
			break;

		default:
			throw b3ParseException("Unexpected token parsing house",m_LineNo);
		}
	}
	while (token != TKN_END);
}

void b3BHDParser::b3ParseLevel()
{
	b3_bhd_token  token;
	b3BBox       *level = new b3BBox(BBOX);
	b3_f64        base,height;

	if (sscanf(&m_Line[m_Pos],"%*s %s %lf %lf\n",level->m_BoxName,&base,&height) != 3)
	{
		throw b3ParseException("Invalid number of arguments",m_LineNo);
	}
	b3PrintF(B3LOG_DEBUG,"  creating area (%s)...\n",level->m_BoxName);

	b3CheckToken(TKN_BEGIN);
	m_Points.b3Clear();
	do
	{
		token = b3ReadLine();
		switch(token)
		{
		case TKN_ROOM:
			b3ParseRoom(level,base,height);
			m_Scene->b3GetBBoxHead()->b3Append(level);
			break;
		case TKN_POINT:
			b3ParsePoint();
			break;
		case TKN_WINDOW:
			b3ParseWindow();
			break;
		case TKN_DOOR:
			b3ParseDoor();
			break;

		case TKN_END:
			break;

		default:
			throw b3ParseException("Unexptected token parsing area",m_LineNo);
		}
	}
	while (token != TKN_END);
}

void b3BHDParser::b3ParsePoint()
{
	b3Point point;

	if (sscanf(&m_Line[m_Pos],"%*s %lf %lf\n",&point.x,&point.y) != 2)
	{
		throw b3ParseException("Invalid number of arguments",m_LineNo);
	}
	point.x *= m_Scale;
	point.y *= m_Scale;
	m_Points.b3Add(point);
}

void b3BHDParser::b3ParseRoom(b3BBox *level,b3_f64 base,b3_f64 height)
{
	b3BBox *room = new b3BBox(BBOX);
	b3CondRectangle *cond;
	b3Area *area;
	b3Item *item;
	b3_f64  xMin,xMax,x;
	b3_f64  yMin,yMax,y;
	int index[16],i;
	b3_count args;
	b3_vector normal;

	args = sscanf(&m_Line[m_Pos],"%*s %s %d %d %d %d %d %d %d %d %d %d\n",
		room->m_BoxName,
		&index[0],&index[1],&index[2],&index[3],&index[4],
		&index[5],&index[6],&index[7],&index[8],&index[9]);
	if (args < 4)
	{
		throw b3ParseException("Invalid number of arguments",m_LineNo);
	}

	b3PrintF(B3LOG_DEBUG,"    creating room (%s)...\n",room->m_BoxName);
	level->b3GetBBoxHead()->b3Append(room);

	index[--args] = index[0];
	for (i = 0;i < args;i++)
	{
		if ((index[i] >= m_Points.b3GetCount()) || (index[i] < 0))
		{
			throw b3ParseException("Invalid point index",m_LineNo);
		}
	}
	xMin = xMax = m_Points[index[0]].x;
	yMin = yMax = m_Points[index[0]].y;
	for (i = 1;i < args;i++)
	{
		x = m_Points[index[i]].x;
		y = m_Points[index[i]].y;
		if (x < xMin) xMin = x;
		if (x > xMax) xMax = x;
		if (y < yMin) yMin = y;
		if (y > yMax) yMax = y;
	}

	// floor
	area = new b3Area(AREA);
	area->m_Base.x = xMin;
	area->m_Base.y = yMin;
	area->m_Base.z = base;
	area->m_Dir1.x = xMax - xMin;
	area->m_Dir2.y = yMax - yMin;
	room->b3GetShapeHead()->b3Append(area);

	// ceil
	area = new b3Area(AREA);
	area->m_Base.x = xMin;
	area->m_Base.y = yMin;
	area->m_Base.z = base + height;
	area->m_Dir1.x = xMax - xMin;
	area->m_Dir2.y = yMax - yMin;
	room->b3GetShapeHead()->b3Append(area);

	for (i = 0;i < args;i++)
	{
		area = new b3Area(AREA);
		area->m_Base.x = m_Points[index[i]].x;
		area->m_Base.y = m_Points[index[i]].y;
		area->m_Base.z = base;
		area->m_Dir1.x = m_Points[index[i+1]].x - m_Points[index[i]].x;
		area->m_Dir1.y = m_Points[index[i+1]].y - m_Points[index[i]].y;
		area->m_Dir2.y = 0;
		area->m_Dir2.z = height;
		b3Vector::b3CrossProduct(&area->m_Dir1,&area->m_Dir2,&normal);
		b3Vector::b3Normalize(&normal,-10.0);
		b3Vector::b3Add(&normal,&area->m_Base);

		room->b3GetShapeHead()->b3Append(area);
	}

	B3_FOR_BASE(room->b3GetShapeHead(),item)
	{
		area = (b3Area *)item;
		cond = new b3CondRectangle(COND_ARECTANGLE);
		cond->m_xStart = 0;
		cond->m_yStart = 0;
		cond->m_xEnd   = 1;
		cond->m_yEnd   = 1;
		area->b3GetConditionHead()->b3Append(cond);
	}
}

void b3BHDParser::b3ParseWindow()
{
	b3PrintF(B3LOG_DEBUG,"    creating window...\n");
}

void b3BHDParser::b3ParseDoor()
{
	b3PrintF(B3LOG_DEBUG,"    creating door...\n");
}

b3Scene *b3BHDParser::b3Parse(const char *filename)
{
	b3Scene *scene = null;

	try
	{
		b3BHDParser parser(filename);
		b3World     world;

		parser.b3ParseHouse();
		scene = parser.m_Scene;
		world.b3SetFirst(scene);
		world.b3Write(scene->b3GetFilename());
	}
	catch(b3WorldException &we)
	{
		b3PrintF(B3LOG_NORMAL,"ERROR:\n");
		b3PrintF(B3LOG_NORMAL,"Write error: %s\n",we.b3GetErrorMsg());
	}
	catch(b3ParseException &pe)
	{
		b3PrintF(B3LOG_NORMAL,"ERROR:\n");
		b3PrintF(B3LOG_NORMAL,"%s\n",pe.m_Message);
	}
	catch(...)
	{
		b3PrintF(B3LOG_NORMAL,"ERROR:\n");
		b3PrintF(B3LOG_NORMAL,"Unknown source:\n");
	}
	return scene;
}

int main(int argc,char *argv[])
{
	int i;

	b3InitRaytrace::b3Init();
	b3Log::b3SetLevel(B3LOG_FULL);	
	for (i = 1;i < argc;i++)
	{
		b3BHDParser::b3Parse(argv[i]);
	}
}
