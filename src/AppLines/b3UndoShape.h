/*
**
**	$Filename:	b3UndoShape.h
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Undo/Redo for shape handling
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3UNDOSHAPE_H
#define B3UNDOSHAPE_H

#include "b3Undo.h"
#include "DlgHierarchy.h"

class b3OpShape : public b3Operation
{
protected:
	b3BBox        *m_BBox;
	CDlgHierarchy *m_DlgHierarchy;

protected:
	inline b3OpShape(b3BBox *bbox,CDlgHierarchy *hierarchy)
	{
		m_BBox         = bbox;
		m_DlgHierarchy = hierarchy;
	}
};

class b3OpShapeCreate : public b3OpShape
{
	b3BBox         *m_Selected;
	b3Shape        *m_Shape;
	b3Shape        *m_InsertAfter;
	b3Base<b3Item> *m_Base;

public:
	b3OpShapeCreate(CAppObjectDoc *pDoc, b3BBox *root, CDlgHierarchy *hierarchy);

protected:
	void b3Undo();
	void b3Redo();
	void b3Delete();

	inline int  b3GetId()
	{
		return IDS_OP_SHAPE_CREATE;
	}
};

#endif
