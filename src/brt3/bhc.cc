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
**	Revision 1.16  2004/05/20 19:10:30  sm
**	- Separated shader from scene. this is easier
**	  to handle.
**
**	Revision 1.15  2004/05/16 09:21:50  sm
**	- Corrected camera access methods.
**	
**	Revision 1.14  2004/04/23 11:09:04  sm
**	- Refectored b3Materials for better dialog use.
**	
**	Revision 1.13  2003/11/01 09:47:26  sm
**	- Added CPU bit version with compiler version string.
**	
**	Revision 1.12  2003/10/23 10:12:42  sm
**	- Added more room indexes.
**	
**	Revision 1.11  2003/09/28 20:33:19  sm
**	- Ensure CPU count in image scaling methods.
**	
**	Revision 1.10  2003/08/27 14:54:23  sm
**	- sprintf changed into snprintf to avoid buffer overflows.
**	
**	Revision 1.9  2003/07/20 09:38:30  sm
**	- Registering raytracing items manually.
**	
**	Revision 1.8  2003/07/13 12:19:07  sm
**	- Added unit/measurement on object print
**	- Adjusted bhc tool for level scaling
**	
**	Revision 1.7  2003/07/12 17:44:47  sm
**	- Cleaned up raytracing b3Item registration
**	
**	Revision 1.6  2003/07/10 08:03:16  sm
**	- Some further error messages added.
**	
**	Revision 1.5  2003/07/09 19:04:22  sm
**	- Added white walls and ceils
**	
**	Revision 1.3  2003/07/09 16:15:06  sm
**	- Fixed empty line bug.
**	
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

#define WALL_THICKNESS 10.0

b3BHDParser::~b3BHDParser()
{
	fclose(m_BHD);
}

b3BHDParser::b3_bhd_token b3BHDParser::b3ReadLine()
{
	b3_size  max = sizeof(m_TokenNames) / sizeof(const char *);
	b3_size  i,len;

	while(!feof(m_BHD))
	{
		m_Line[0] = 0;
		m_Pos     = 0;
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
	
	m_Scene = new b3Scene(TRACEPHOTO_MORK);
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
	camera = m_Scene->b3GetFirstCamera();
	camera->b3Orientate(&eye,&view,5,3.2,2.4);
	do
	{
		token = b3ReadLine();
		switch(token)
		{
		case TKN_LEVEL:
			b3ParseLevel(m_Scale);
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

void b3BHDParser::b3ParseLevel(b3_f64 scale)
{
	b3_bhd_token  token;
	b3BBox       *level = new b3BBox(BBOX);
	b3_f64        base,height;

	if (sscanf(&m_Line[m_Pos],"%*s %s %lf %lf %lf\n",level->m_BoxName,&base,&height,&scale) < 3)
	{
		throw b3ParseException("Invalid number of arguments",m_LineNo);
	}
	b3PrintF(B3LOG_DEBUG,"  creating area %s scaling by %3.2f...\n",level->m_BoxName,scale);

	b3CheckToken(TKN_BEGIN);
	m_Points.b3Clear();
	m_Openings.b3Clear();
	do
	{
		token = b3ReadLine();
		switch(token)
		{
		case TKN_ROOM:
			b3ParseRoom(level,base,height,scale);
			m_Scene->b3GetBBoxHead()->b3Append(level);
			break;
		case TKN_POINT:
			b3ParsePoint(scale);
			break;
		case TKN_WINDOW:
			b3ParseWindow(scale);
			break;
		case TKN_DOOR:
			b3ParseDoor(scale);
			break;

		case TKN_END:
			break;

		default:
			throw b3ParseException("Unexptected token parsing area",m_LineNo);
		}
	}
	while (token != TKN_END);
}

void b3BHDParser::b3ParsePoint(b3_f64 scale)
{
	b3Point point;

	if (sscanf(&m_Line[m_Pos],"%*s %lf %lf\n",&point.x,&point.y) != 2)
	{
		throw b3ParseException("Invalid number of arguments",m_LineNo);
	}
	point.x *= scale;
	point.y *= scale;
	m_Points.b3Add(point);
}

void b3BHDParser::b3ParseRoom(b3BBox *level,b3_f64 base,b3_f64 height,b3_f64 scale)
{
	b3BBox          *room = new b3BBox(BBOX);
	b3CondRectangle *cond;
	b3Area          *area;
	b3Light         *light;
	b3Item          *item;
	b3_f64           xMin,xMax,x;
	b3_f64           yMin,yMax,y;
	b3_index         index[16],i;
	b3_count         args;
	b3_vector        normal;

	args = sscanf(&m_Line[m_Pos],"%*s %s %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",
		room->m_BoxName,
		&index[ 0],&index[ 1],&index[ 2],&index[ 3],&index[ 4],
		&index[ 5],&index[ 6],&index[ 7],&index[ 8],&index[ 9],
		&index[10],&index[11],&index[12],&index[13],&index[14]);
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

	for (i = 0;i < args;i++)
	{
		area = new b3Area(AREA);
		area->m_Base.x = m_Points[index[i]].x;
		area->m_Base.y = m_Points[index[i]].y;
		area->m_Base.z = base;
		area->m_Dir1.x = m_Points[index[i+1]].x - m_Points[index[i]].x;
		area->m_Dir1.y = m_Points[index[i+1]].y - m_Points[index[i]].y;
		area->m_Dir2.x = 0;
		area->m_Dir2.y = 0;
		area->m_Dir2.z = height;
		b3Vector::b3CrossProduct(&area->m_Dir1,&area->m_Dir2,&normal);
		b3Vector::b3Normalize(&normal,-WALL_THICKNESS);
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

	area = (b3Area *)room->b3GetShapeHead()->First;
	for (i = 0;i < args;i++)
	{
		b3CheckOpenings(room,area,index[i],index[i+1]);
		area = (b3Area *)area->Succ;
	}

	// ceil
	area = new b3Area(AREA);
	area->m_Base.x = xMin;
	area->m_Base.y = yMin;
	area->m_Base.z = base + height;
	area->m_Dir1.x = xMax - xMin;
	area->m_Dir2.y = yMax - yMin;
	room->b3GetShapeHead()->b3Append(area);

	cond = new b3CondRectangle(COND_ARECTANGLE);
	cond->m_xStart = 0;
	cond->m_yStart = 0;
	cond->m_xEnd   = 1;
	cond->m_yEnd   = 1;
	area->b3GetConditionHead()->b3Append(cond);
	b3AddWall(room);

	// floor
	area = new b3Area(AREA);
	area->m_Base.x = xMin;
	area->m_Base.y = yMin;
	area->m_Base.z = base;
	area->m_Dir1.x = xMax - xMin;
	area->m_Dir2.y = yMax - yMin;
	room->b3GetShapeHead()->b3Append(area);

	cond = new b3CondRectangle(COND_ARECTANGLE);
	cond->m_xStart = 0;
	cond->m_yStart = 0;
	cond->m_xEnd   = 1;
	cond->m_yEnd   = 1;
	area->b3GetConditionHead()->b3Append(cond);

	light = new b3Light(POINT_LIGHT);
	light->b3SetName(room->m_BoxName);
	light->m_Position.x = (xMin + xMax) * 0.5;
	light->m_Position.y = (yMin + yMax) * 0.5;
	light->m_Position.z = base + height - 5;
	light->m_Distance = 200;
	m_Scene->b3GetLightHead()->b3Append(light);
}

void b3BHDParser::b3AddWall(b3BBox *room)
{
	b3Item      *item;
	b3Shape     *shape;
	b3MatNormal *material;

	B3_FOR_BASE(room->b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		material = new b3MatNormal(MATERIAL);
		material->m_Diffuse.b3Init(1,1,1);
		material->m_Ambient.b3Init(0.2,0.2,0.2);
		material->m_Specular.b3Init(1,1,1);
		material->m_Reflection = 0.0;
		material->m_SpecularExp = 100000.0;
		shape->b3GetMaterialHead()->b3Append(material);
	}
}

void b3BHDParser::b3CheckOpenings(b3BBox *room,b3Area *area,b3_index a,b3_index b)
{
	b3Area          *left,*right,*top,*bottom;
	b3CondRectangle *cond;
	b3_vector normal;
	b3_index  i;
	b3_f64    width,height;

	width  = b3Vector::b3Length(&area->m_Dir1);
	height = b3Vector::b3Length(&area->m_Dir2);
	for (i = 0;i < m_Openings.b3GetCount();i++)
	{
		if ((m_Openings[i].a == a) && (m_Openings[i].b == b))
		{
			cond = new b3CondRectangle(COND_NRECTANGLE);
			cond->m_xStart =  m_Openings[i].pos   / width;
			cond->m_yStart =  m_Openings[i].base  / height;
			cond->m_xEnd   = (m_Openings[i].pos  + m_Openings[i].width)   / width;
			cond->m_yEnd   = (m_Openings[i].base + m_Openings[i].height)  / height;
			if ((cond->m_xStart < 0) || (cond->m_xEnd > 1) ||
			    (cond->m_yStart < 0) || (cond->m_yEnd > 1))
			{
				char message[1024];

				snprintf(message,sizeof(message),"Door/window definition oversized: (%1.3f - %1.3f,%1.3f - %1.3f)",
					cond->m_xStart,cond->m_xEnd,cond->m_yStart,cond->m_yEnd);
				throw b3ParseException(message,m_Openings[i].line);
			}
			area->b3GetConditionHead()->b3Append(cond);

			b3Vector::b3CrossProduct(&area->m_Dir1,&area->m_Dir2,&normal);
			b3Vector::b3Normalize(&normal,WALL_THICKNESS);

			left = new b3Area(AREA);
			left->m_Base.x = m_Points[a].x + area->m_Dir1.x * cond->m_xStart;
			left->m_Base.y = m_Points[a].y + area->m_Dir1.y * cond->m_xStart;
			left->m_Base.z = area->m_Base.z + m_Openings[i].base;
			left->m_Dir1   = normal;
			left->m_Dir2.x = 0;
			left->m_Dir2.y = 0;
			left->m_Dir2.z = m_Openings[i].height;
			room->b3GetShapeHead()->b3Append(left);

			right = new b3Area(AREA);
			right->m_Base.x = m_Points[a].x + area->m_Dir1.x * cond->m_xEnd;
			right->m_Base.y = m_Points[a].y + area->m_Dir1.y * cond->m_xEnd;
			right->m_Base.z = area->m_Base.z + m_Openings[i].base;
			right->m_Dir1   = normal;
			right->m_Dir2.x = 0;
			right->m_Dir2.y = 0;
			right->m_Dir2.z = m_Openings[i].height;
			room->b3GetShapeHead()->b3Append(right);

			top = new b3Area(AREA);
			top->m_Base.x = (left->m_Base.x + right->m_Base.x) * 0.5;
			top->m_Base.y = (left->m_Base.y + right->m_Base.y) * 0.5;
			top->m_Base.z =  area->m_Base.z + m_Openings[i].base + m_Openings[i].height;
			top->m_Dir2   = normal;
			top->m_Dir1.x = (right->m_Base.x - left->m_Base.x) * 0.5;
			top->m_Dir1.y = (right->m_Base.y - left->m_Base.y) * 0.5;
			top->m_Dir1.z = 0;
			room->b3GetShapeHead()->b3Append(top);

			if (m_Openings[i].type == b3_door::BHC_WINDOW)
			{
				bottom = new b3Area(AREA);
				bottom->m_Base = top->m_Base;
				bottom->m_Base.z = area->m_Base.z + m_Openings[i].base;
				bottom->m_Dir1   = top->m_Dir1;
				bottom->m_Dir2   = top->m_Dir2;
				room->b3GetShapeHead()->b3Append(bottom);
			}

			cond = new b3CondRectangle(COND_ARECTANGLE);
			cond->m_xStart = -1;
			cond->m_yStart =  0;
			cond->m_xEnd   =  1;
			cond->m_yEnd   =  1;
			left->b3GetConditionHead()->b3Append(cond);

			cond = new b3CondRectangle(COND_ARECTANGLE);
			cond->m_xStart = -1;
			cond->m_yStart =  0;
			cond->m_xEnd   =  1;
			cond->m_yEnd   =  1;
			right->b3GetConditionHead()->b3Append(cond);

			b3PrintF(B3LOG_DEBUG,"       Added door/window of line %d\n",m_Openings[i].line);
		}

		if ((m_Openings[i].a == b) && (m_Openings[i].b == a))
		{
			cond = new b3CondRectangle(COND_NRECTANGLE);
			cond->m_xStart =   m_Openings[i].pos   / width;
			cond->m_yStart =   m_Openings[i].base  / height;
			cond->m_xEnd   =  (m_Openings[i].pos  + m_Openings[i].width)   / width;
			cond->m_yEnd   =  (m_Openings[i].base + m_Openings[i].height)  / height;
			if ((cond->m_xStart < 0) || (cond->m_xEnd > 1) ||
			    (cond->m_yStart < 0) || (cond->m_yEnd > 1))
			{
				char message[1024];

				snprintf(message,sizeof(message),"Door/window definition oversized: (%1.3f - %1.3f,%1.3f - %1.3f)",
			         -cond->m_xStart,-cond->m_xEnd,cond->m_yStart,cond->m_yEnd);
				throw b3ParseException(message,m_Openings[i].line);
			}
 			area->b3GetConditionHead()->b3Append(cond);

			b3PrintF(B3LOG_DEBUG,"       Used door/window of line %d\n",m_Openings[i].line);
		}
	}
}

void b3BHDParser::b3ParseWindow(b3_f64 scale)
{
	b3_door window;

	b3PrintF(B3LOG_DEBUG,"    creating window...\n");
	if (sscanf(&m_Line[m_Pos],"%*s %ld %ld %lf %lf %lf %lf\n",
		&window.a,&window.b,
		&window.pos,&window.base,
		&window.width,&window.height) != 6)
	{
		throw b3ParseException("Invalid number of arguments",m_LineNo);
	}
	window.pos    *= scale;
	window.width  *= scale;
	window.type    = b3_door::BHC_WINDOW;
	window.line    = m_LineNo;
	m_Openings.b3Add(window);
}

void b3BHDParser::b3ParseDoor(b3_f64 scale)
{
	b3_door  door;

	b3PrintF(B3LOG_DEBUG,"    creating door...\n");
	if (sscanf(&m_Line[m_Pos],"%*s %ld %ld %lf %lf\n",
		&door.a,&door.b,
		&door.pos,&door.width) != 4)
	{
		throw b3ParseException("Invalid number of arguments",m_LineNo);
	}
	door.pos    *= scale;
	door.width  *= scale;
	door.height  = 200;
	door.base    =   0;
	door.type    = b3_door::BHC_DOOR;
	door.line    = m_LineNo;
	m_Openings.b3Add(door);
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

	b3Log::b3SetLevel(B3LOG_FULL);	
	b3RaytracingItems::b3Register();
	for (i = 1;i < argc;i++)
	{
		b3BHDParser::b3Parse(argv[i]);
	}
}
