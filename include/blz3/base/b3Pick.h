/*
**
**	$Filename:	b3Pick.h $  
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Controlling the LDC of a light source
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_PICK_H
#define B3_BASE_PICK_H

#include "blz3/b3Config.h"
#include "blz3/base/b3List.h"
#include "blz3/base/b3UndoOperation.h"

class b3Pick : public b3Link<b3Pick>
{
protected:
	b3_coord m_x,m_y;

public:
	                         b3Pick(b3_coord x=0,b3_coord y=0);
	virtual                 ~b3Pick();
	virtual void             b3Draw(b3DrawContext *dc);
	virtual void             b3Update();
	virtual b3_bool          b3Hit(b3_coord x,b3_coord y);
	virtual b3_bool          b3Moved(b3_coord x,b3_coord y);
	virtual b3UndoOperation *b3GetOperation();
};

class b3PickBase : public b3Base<b3Pick>
{
protected:
	b3Pick          *m_Selected;

public:
	static b3_coord  m_PickSize;

public:
	                 b3PickBase();
	b3_bool          b3IsActive();
	b3_bool          b3Down(b3_coord x,b3_coord y);
	b3_bool          b3Move(b3_coord x,b3_coord y);
	b3_bool          b3Up(b3_coord x,b3_coord y);
	void             b3Draw(b3DrawContext *dc);
	void             b3Update();
	b3UndoOperation *b3GetOperation();
};

#endif
