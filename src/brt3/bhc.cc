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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
	b3_size  i;

	m_Line[0] = 0;
	m_Pos     = 0;
	if (fgets(m_Line,sizeof(m_Line),m_BHD) != null)
	{
		while(isspace(m_Line[m_Pos]))
		{
			m_Pos++;
		}
		m_LineNo++;
		for (i = 0;i < max;i++)
		{
			if (strncmp(&m_Line[m_Pos],m_TokenNames[i],strlen(m_TokenNames[i])) == 0)
			{
				return (b3_bhd_token)i;
			}
		}
		throw b3ParseException("Syntax error",m_LineNo);
	}
	throw b3ParseException("Line buffer overflow",m_LineNo);
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
	b3_bhd_token token;
//	b3_vector     lower,upper;

	b3PrintF(B3LOG_DEBUG,"Creating house...\n");
	b3CheckToken(TKN_HOUSE);
	b3CheckToken(TKN_BEGIN);
	
	m_Scene = new b3SceneMork(TRACEPHOTO_MORK);
	sscanf(&m_Line[m_Pos],"%*s %s\n",(char *)name);

	m_Scene->b3SetFilename("Bla.bwd");
	b3PrintF(B3LOG_DEBUG,"  Filename: %s\n",m_Scene->b3GetFilename());
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
	
	do
	{
		token = b3ReadLine();
		switch(token)
		{
		case TKN_ROOM:
			b3ParseRoom(level,base,height);
			m_Scene->b3GetBBoxHead()->b3Append(level);
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

void b3BHDParser::b3ParseRoom(b3BBox *level,b3_f64 base,b3_f64 height)
{
	b3BBox *room = new b3BBox(BBOX);
	b3CondRectangle *cond;
	b3Area *area;
	b3Item *item;
	b3_f64  ox,oy;
	b3_f64  xSize,ySize;

	if (sscanf(&m_Line[m_Pos],"%*s %s %lf %lf %lf %lf\n",room->m_BoxName,&ox,&oy,&xSize,&ySize) != 5)
	{
		throw b3ParseException("Invalid number of arguments",m_LineNo);
	}
	b3PrintF(B3LOG_DEBUG,"    creating room (%s)...\n",room->m_BoxName);
	level->b3GetBBoxHead()->b3Append(room);

	// floor
	area = new b3Area(AREA);
	area->m_Base.x = ox;
	area->m_Base.y = oy;
	area->m_Base.z = base;
	area->m_Dir1.x = xSize;
	area->m_Dir2.y = ySize;
	room->b3GetShapeHead()->b3Append(area);

	// ceil
	area = new b3Area(AREA);
	area->m_Base.x = ox;
	area->m_Base.y = oy;
	area->m_Base.z = base + height;
	area->m_Dir1.x = xSize;
	area->m_Dir2.y = ySize;
	room->b3GetShapeHead()->b3Append(area);

	// front
	area = new b3Area(AREA);
	area->m_Base.x = ox;
	area->m_Base.y = oy;
	area->m_Base.z = base;
	area->m_Dir1.x = xSize;
	area->m_Dir2.y = 0;
	area->m_Dir2.z = height;
	room->b3GetShapeHead()->b3Append(area);

	// right
	area = new b3Area(AREA);
	area->m_Base.x = ox + xSize;
	area->m_Base.y = oy;
	area->m_Base.z = base;
	area->m_Dir1.x = 0;
	area->m_Dir1.y = ySize;
	area->m_Dir2.y = 0;
	area->m_Dir2.z = height;
	room->b3GetShapeHead()->b3Append(area);

	// left
	area = new b3Area(AREA);
	area->m_Base.x = ox;
	area->m_Base.y = oy;
	area->m_Base.z = base;
	area->m_Dir1.x = 0;
	area->m_Dir1.y = ySize;
	area->m_Dir2.y = 0;
	area->m_Dir2.z = height;
	room->b3GetShapeHead()->b3Append(area);

	// back
	area = new b3Area(AREA);
	area->m_Base.x = ox;
	area->m_Base.y = oy + ySize;
	area->m_Base.z = base;
	area->m_Dir1.x = xSize;
	area->m_Dir2.y = 0;
	area->m_Dir2.z = height;
	room->b3GetShapeHead()->b3Append(area);

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
