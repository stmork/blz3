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

class b3RenderObject;

/**
 * This class provides methods for pick points. Pick points
 * are small clickable points for manipulating something.
 */
class b3Pick : public b3Link<b3Pick>
{
protected:
	b3_coord m_x;   //!< The y position of the pick point.
	b3_coord m_y;   //!< The x position of the pick point.
	b3_f64   m_z;   //!< The depth.

public:
	b3Pick(const b3_coord x=0, const b3_coord y=0);
	virtual                 ~b3Pick();
	virtual void             b3Draw(const b3DrawContext *dc);
	virtual void             b3Update();
	virtual const b3_bool    b3Hit(const b3_coord x, const b3_coord y) const;
	virtual const b3_bool    b3Moved(const b3_coord x, const b3_coord y);
	virtual b3UndoOperation *b3GetOperation(const b3RenderObject *object) = 0;
};

/**
 * This class represents a collection of pick points.
 */
class b3PickBase : public b3Base<b3Pick>
{
protected:
	b3Pick          *m_Selected;  //!< The actually selected pick point.

public:
	static b3_coord  m_PickSize;  //!< The size of the pick point as radius.

public:
	b3PickBase();
	const b3_bool    b3IsActive() const;
	const b3_bool    b3Down(const b3_coord x, const b3_coord y);
	const b3_bool    b3Move(const b3_coord x, const b3_coord y);
	const b3_bool    b3Up(const b3_coord x, const b3_coord y);
	void             b3Draw(const b3DrawContext *dc);
	void             b3Update();
	b3UndoOperation *b3GetOperation(const b3RenderObject *object) const;
};

#endif
