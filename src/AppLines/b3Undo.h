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

#include "blz3/b3Config.h"
#include "blz3/base/b3List.h"

class b3UndoBuffer;
class CAppRenderDoc;

class b3Operation : public b3Link<b3Operation>
{
	friend class b3UndoBuffer;

protected:
	b3_bool          m_Initialized;
public:
	                 b3Operation();
	virtual         ~b3Operation();

protected:
	virtual void     b3Do();
	virtual void     b3Undo();
	virtual void     b3Redo();
	virtual void     b3Prepare(CAppRenderDoc *pDoc);
	virtual int      b3GetId() = 0;
	        b3_bool  b3IsInitialized();
};

class b3UndoBuffer
{
	b3Base<b3Operation>  m_UndoBuffer;
	b3Base<b3Operation>  m_RedoBuffer;
	CAppRenderDoc       *m_Doc;

public:
	         b3UndoBuffer(CAppRenderDoc *pDoc);
	virtual ~b3UndoBuffer();

	void     b3Clear();
	b3_bool  b3Do(b3Operation *operation);
	void     b3Undo();
	void     b3Redo();
	b3_bool  b3HasUndo();
	b3_bool  b3HasRedo();
};

#endif
