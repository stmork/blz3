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

#include "blz3/base/b3Aux.h"
#include "blz3/raytrace/b3BBox.h"
#include "blz3/raytrace/b3Material.h"
#include "blz3/raytrace/b3Shape.h"
#include "blz3/raytrace/b3Scene.h"

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
	} m_Token = TKN_UNKNOWN;

	static const char * m_TokenNames[];
	b3Array<b3Point>    m_Points;
	b3Array<b3_door>    m_Openings;
	char                m_Line[1024];
	b3_size             m_Pos    = 0;
	FILE        *       m_BHD    = nullptr;
	b3Scene      *      m_Scene  = nullptr;
	b3_count            m_LineNo = 0;
	b3_f64              m_Scale  = 0.0;

private:
	explicit b3BHDParser(const char * filename);

	b3BHDParser(const b3BHDParser & other) = delete;
	b3BHDParser & operator=(const b3BHDParser & other) = delete;

	virtual~b3BHDParser();

	b3_bhd_token    b3ReadLine();
	void            b3CheckToken(b3_bhd_token token);
	void            b3ParseHouse();
	void            b3ParseLevel(b3_f64 scale);
	void            b3ParsePoint(b3_f64 scale);
	void            b3ParseRoom(const b3BBox * level, b3_f64 base, b3_f64 height, b3_f64 scale);
	void            b3CheckOpenings(const b3BBox * room, b3Area * area, b3_index a, b3_index b);
	void            b3AddWall(const b3BBox * room);
	void            b3ParseDoor(b3_f64 scale);
	void            b3ParseWindow(b3_f64 scale);

public:
	static b3Scene * b3Parse(const char * filename);
};

class b3ParseException : public std::exception
{
public:
	char m_Message[2048];

public:
	explicit b3ParseException(const char * text)
	{
		strncpy(m_Message, text, sizeof(m_Message));
	}

	explicit b3ParseException(const char * text, const b3_count line)
	{
		snprintf(m_Message, sizeof(m_Message), "%s (line: %d)", text, line);
	}

	inline const char * what() const noexcept override
	{
		return m_Message;
	}
};

#endif
