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

#ifndef BHC_H
#define BHC_H

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/base/b3Aux.h"

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

	static const char *m_TokenNames[];
	b3Array<b3Point> m_Points;
	b3_index    m_Pos;
	char        m_Line[1024];
	FILE       *m_BHD;
	b3Scene    *m_Scene;
	b3_index    m_LineNo;
	b3_f64      m_Scale;
	
private:
	                b3BHDParser(const char *filename);
	               ~b3BHDParser();
	b3_bhd_token    b3ReadLine();
	void            b3CheckToken(b3_bhd_token token);
	void            b3ParseHouse();
	void            b3ParseLevel();
	void            b3ParsePoint();
	void            b3ParseRoom(b3BBox *level,b3_f64 base,b3_f64 height);
	void            b3ParseDoor();
	void            b3ParseWindow();

public:
	static b3Scene *b3Parse(const char *filename);
};

class b3ParseException
{
public:
	char m_Message[1024];

public:
	b3ParseException(char *text)
	{
		strcpy(m_Message,text);
	}

	b3ParseException(char *text,int line)
	{
		sprintf(m_Message,"%s (line: %d)",text,line);
	}
};

#endif
