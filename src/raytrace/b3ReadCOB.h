/*
**
**	$Filename:	b3ReadCOB.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Converting Caligari objects into Blizzard III
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_READCOB_H
#define B3_READCOB_H

#include "blz3/raytrace/b3Raytrace.h"

typedef int b3_cob_id;

enum b3_cob_type
{
	COB_UNKNOWN = -1,
    COB_GROU    =  1,
	COB_POLH,
	COB_MAT1,
	COB_END ,
	COB_UNIT,
	COB_DEFP,
	COB_CHAN,
	COB_DDIV
};

#define MAX_LINE 4096

class b3COBInfo : public b3Link<b3COBInfo>
{
public:
	b3Item      *refNode;
	b3_cob_id    refID;
	b3_cob_id    refParent;
	b3_cob_type  refType;

public:
	inline b3COBInfo(
		b3Item      *node,
		b3_cob_id    id,
		b3_cob_id    parent,
		b3_cob_type  type) : b3Link<b3COBInfo>(sizeof(b3COBInfo))
	{
		refNode   = node;
		refID     = id;
		refParent = parent;
		refType   = type;
	}
};

class b3COBReader
{
	b3Base<b3COBInfo>   cobInfos;
	b3Base<b3COBInfo>   cobDone;

	                    b3COBReader();
	                   ~b3COBReader();

	       b3_bool      b3COB_AllocObject(b3Item *node,b3_cob_id id,b3_cob_id parent,b3_cob_type type);
	       b3COBInfo   *b3COB_FindInfo(b3_cob_id id);
	       b3Item      *b3COB_Reconstruct();
	       void         b3COB_ComputeAvrgColor(b3Tx *texture,b3Color &color);
	       b3_size      b3COB_ParseGrou(const char *buffer,const char *name);
	       b3_size      b3COB_ParsePolH(const char *buffer,const char *name);
	       b3_size      b3COB_ParseMat(const char *buffer);
	       b3_size      b3COB_ParseDummy(const char *dummy);
	       b3Item      *b3COB_Parse(const char *buffer,const char *name,b3_size size);

	static b3_size      b3COB_GetLine(char *line,const char *buffer,b3_size max);
	static b3_cob_type  b3COB_GetToken(const char *buffer);
	static b3_bool      b3COB_IsDegenerated(b3_vertex *vertices,b3_triangle *tria);


public:
	static b3BBox      *b3ReadCOB(const char *filename);
};

#endif
