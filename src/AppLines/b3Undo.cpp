/*
**
**	$Filename:	b3Undo.cpp
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Undo/Redo
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3Undo.h"
#include "blz3/b3Config.h"
#include "AppRenderDoc.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2003/01/28 15:58:27  sm
**	- Added support for undoing/redoing picking
**
**	Revision 1.4  2003/01/12 10:39:45  sm
**	- Fixed proper delete for undo/redo.
**	
**	Revision 1.3  2003/01/11 17:16:15  sm
**	- Object handling with undo/redo
**	- Light handling with undo/redo
**	
**	Revision 1.2  2003/01/07 16:14:38  sm
**	- Lines III: object editing didn't prepared any more. Fixed.
**	- Some prepare optimizations.
**	
**	Revision 1.1  2003/01/05 16:13:24  sm
**	- First undo/redo implementations
**	
**
*/

/*************************************************************************
**                                                                      **
**                        Undo/Redo                                     **
**                                                                      **
*************************************************************************/

b3LinesUndoBuffer::b3LinesUndoBuffer(CAppRenderDoc *pDoc)
{
	m_PrepareInfo = new b3LinesUndoPrepareInfo(pDoc);
}

b3LinesUndoBuffer::~b3LinesUndoBuffer()
{
	delete m_PrepareInfo;
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo                                     **
**                                                                      **
*************************************************************************/

b3Operation::b3Operation()
{
	m_PrepareGeometry         = true;
	m_PrepareChangedStructure = false;
}

b3Operation::~b3Operation()
{
}

void b3Operation::b3Undo()
{
	CString desc;

	desc.LoadString(b3GetId());
	b3PrintF(B3LOG_NORMAL,"Undo-Operation for %s not overloaded.\n",desc);
}

void b3Operation::b3Redo()
{
	CString desc;

	desc.LoadString(b3GetId());
	b3PrintF(B3LOG_NORMAL,"Redo-Operation for %s not overloaded.\n",desc);
}

void b3Operation::b3Prepare(b3UndoPrepareInfo *info)
{
	b3LinesUndoPrepareInfo *lupi = (b3LinesUndoPrepareInfo *)info;

	lupi->m_pDoc->b3Prepare(
		m_PrepareGeometry,
		m_PrepareChangedStructure);
}
