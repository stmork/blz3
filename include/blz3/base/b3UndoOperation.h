/*
**
**	$Filename:	b3UndoOperation.h $
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

#ifndef B3_BASE_UNDOOPERATION_H
#define B3_BASE_UNDOOPERATION_H

#include "blz3/b3Config.h"
#include "blz3/base/b3List.h"

class b3UndoBuffer;

class b3UndoPrepareInfo
{
public:
	virtual void b3Prepare()
	{
	}
};

class b3UndoOperation : public b3Link<b3UndoOperation>
{
	friend class b3UndoBuffer;

	b3_bool          m_Initialized;
	b3_bool          m_Done;

public:
	                 b3UndoOperation();
	virtual         ~b3UndoOperation();

protected:
	virtual void     b3Do();
	virtual void     b3Undo();
	virtual void     b3Redo();
	virtual int      b3GetId() = 0;
	virtual void     b3Delete();
	virtual void     b3Prepare(b3UndoPrepareInfo *info) = 0;

	inline  void     b3Initialize(b3_bool init = true)
	{
		m_Initialized = init;
	}

	inline  void     b3Done(b3_bool done)
	{
		m_Done = done;
	}

	inline  b3_bool  b3IsInitialized()
	{
		return m_Initialized;
	}
	
	inline  b3_bool  b3IsDone()
	{
		return m_Done;
	}
};

class b3UndoBuffer
{
	b3Base<b3UndoOperation>  m_UndoBuffer;
	b3Base<b3UndoOperation>  m_RedoBuffer;

protected:
	b3UndoPrepareInfo       *m_PrepareInfo;

public:
	         b3UndoBuffer();
	virtual ~b3UndoBuffer();

	void     b3Clear();
	b3_bool  b3Do(b3UndoOperation *operation);
	void     b3Undo();
	void     b3Redo();
	b3_bool  b3HasUndo();
	b3_bool  b3HasRedo();

private:
	void     b3Delete(b3UndoOperation *op);
	void     b3Delete(b3Base<b3UndoOperation> *buffer);
};

#endif
