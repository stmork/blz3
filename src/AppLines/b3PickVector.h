
/*
**
**	$Filename:	b3PickVector.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Definition of pick points and pick direction vectors
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_PICK_VECTOR_H
#define B3_PICK_VECTOR_H

#include "blz3/raytrace/b3Shape.h"
#include "blz3/raytrace/b3RenderView.h"
#include "blz3/base/b3Pick.h"
#include "b3UndoPick.h"

class b3PickPoint : public b3Pick
{
	b3ModellerInfo *m_Info;
	b3_vector      *m_Pos;
	const char     *m_Text;

protected:
	b3RenderView   *m_RenderView;
	b3_vector       m_StartVector;

public:
	static COLORREF m_PickColor;

public:
	b3PickPoint(
		b3RenderView   *renderview,
		b3_vector      *pos,
		const char     *text = null,
		b3ModellerInfo *info = null);

	        void             b3Draw(b3DrawContext *dc);
	        b3_bool          b3Moved(b3_coord x,b3_coord y);
	virtual b3UndoOperation *b3GetOperation();
};

class b3PickDir : public b3PickPoint
{
	b3_vector *m_OrigPos;
	b3_vector *m_OrigDir;
	b3_vector  m_AuxPos;

public:
	b3PickDir(
		b3RenderView   *renderview,
		b3_vector      *pos,
		b3_vector      *dir,
		const char     *text = null,
		b3ModellerInfo *info = null);

	        void             b3Draw(b3DrawContext *dc);
	        b3_bool          b3Moved(b3_coord x,b3_coord y);
	virtual b3UndoOperation *b3GetOperation();
};

#endif
