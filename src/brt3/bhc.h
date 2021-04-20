/*
**
**	$Filename:	bhc.cc $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Creating a house from a description file
**
**      (C) Copyright 2003  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef BHC_H
#define BHC_H

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/base/b3Aux.h"

#include <stdexcept>

struct b3_door
{
	b3_index a, b;
	b3_f64   pos, base, width, height;
	b3_count line;
	enum b3_door_type
	{
		BHC_DOOR,
		BHC_WINDOW
	} type;
};

class b3BHDParser
{
	enum b3_bhd_token
	{
		TKN_UNKNOWN = -1,
		TKN_BEGIN   =  0,
		TKN_END,
		TKN_HOUSE,
		TKN_LEVEL,
		TKN_POINT,
		TKN_ROOM,
		TKN_WINDOW,
		TKN_DOOR
	} m_Token;

	static const char * m_TokenNames[];
	b3Array<b3Point>   m_Points;
	b3Array<b3_door>   m_Openings;
	b3_size            m_Pos;
	char               m_Line[1024];
	FILE       *       m_BHD;
	b3Scene      *     m_Scene;
	b3_count           m_LineNo;
	b3_f64             m_Scale;

private:
	b3BHDParser(const char * filename);
	~b3BHDParser();
	b3_bhd_token    b3ReadLine();
	void            b3CheckToken(b3_bhd_token token);
	void            b3ParseHouse();
	void            b3ParseLevel(b3_f64 scale);
	void            b3ParsePoint(b3_f64 scale);
	void            b3ParseRoom(b3BBox * level, b3_f64 base, b3_f64 height, b3_f64 scale);
	void            b3CheckOpenings(b3BBox * room, b3Area * area, b3_index a, b3_index b);
	void            b3AddWall(b3BBox * room);
	void            b3ParseDoor(b3_f64 scale);
	void            b3ParseWindow(b3_f64 scale);

public:
	static b3Scene * b3Parse(const char * filename);
};

class b3ParseException : public std::exception
{
public:
	char m_Message[1024];

public:
	explicit b3ParseException(const char * text)
	{
		strcpy(m_Message, text);
	}

	explicit b3ParseException(const char * text, b3_count line)
	{
		sprintf(m_Message, "%s (line: %d)", text, line);
	}

	inline const char * what() const noexcept override
	{
		return m_Message;
	}
};

#endif
