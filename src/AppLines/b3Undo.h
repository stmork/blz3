/*
**
**	$Filename:	b3Undo.h $
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

#ifndef B3UNDO_H
#define B3UNDO_H

#include "blz3/base/b3UndoOperation.h"

class CAppRenderDoc;

class b3LinesUndoPrepareInfo : public b3UndoPrepareInfo
{
public:
 	CAppRenderDoc *m_pDoc;

public:
	inline b3LinesUndoPrepareInfo(CAppRenderDoc *pDoc)
	{
		m_pDoc = pDoc;
	}

	inline void b3Prepare()
	{
	}
};

class b3Operation : public b3UndoOperation
{
protected:
	b3_bool          m_PrepareGeometry;
	b3_bool          m_PrepareChangedStructure;

public:
	                 b3Operation();
	virtual         ~b3Operation();

protected:
	virtual void     b3Undo();
	virtual void     b3Redo();
	virtual void     b3Prepare(b3UndoPrepareInfo *pDoc);
};

class b3LinesUndoBuffer : public b3UndoBuffer
{
public:
	              b3LinesUndoBuffer(CAppRenderDoc *pDoc);
	virtual      ~b3LinesUndoBuffer();
};

#endif
