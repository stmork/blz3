/*
**
**	$Filename:	b3UndoOperation.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
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

/**
 * This class is a custom information class to pass application
 * specific data into the undo/redo operation.
 *
 * @see b3UndoOperation
 */
class b3UndoPrepareInfo
{
public:
	/**
	 * This method prepares this instance.
	 */
	virtual void b3Prepare()
	{
	}
};

/**
 * This abstract class implements the handling of one undo/redo operation.
 */
class b3UndoOperation : public b3Link<b3UndoOperation>
{
	friend class b3UndoBuffer;

	b3_bool          m_Initialized;
	b3_bool          m_Done;

public:
	/**
	 * The constructor initializes the operation. If the initialization
	 * succeed the constructor must call b3Initialize(true) so that
	 * the b3UndoBuffer handles the operation via b3Do().
	 */
	b3UndoOperation();

	/**
	 * This destructor deinitializes the operation.
	 */
	virtual         ~b3UndoOperation();

protected:
	/**
	 * This method implements the first do operation. The default
	 * implementation calls b3Redo().
	 */
	virtual void     b3Do();

	/**
	 * This method implements the undo operation for undoing an already
	 * done operation.
	 */
	virtual void     b3Undo();

	/**
	 * This method implements further doing operation such redo. In
	 * general this is equal with the do operation.
	 */
	virtual void     b3Redo();

	/**
	 * This method frees this instance depending on the internal state. if this
	 * operation is done the undone information must be deleted otherwise the
	 * done information.
	 */
	virtual void     b3Delete();

	/**
	 * This method is called during b3Do(), b3Redo() and b3Undo() method calls to
	 * prepare the operation.
	 *
	 * @param info Custom application dependend information.
	 */
	virtual void     b3Prepare(b3UndoPrepareInfo *info) = 0;

	/**
	 * This method sets the initialization state. This method must be called during
	 * the constructor phase to allow first doing.
	 *
	 * @param init The new initialization state.
	 */
	inline  void     b3Initialize(b3_bool init = true)
	{
		m_Initialized = init;
	}

	/**
	 * This method sets the done state. If the done state is true the operation
	 * can only be undone otherwise the operation can only be redone.
	 *
	 * @param done The new done state.
	 */
	inline  void     b3Done(b3_bool done)
	{
		m_Done = done;
	}

	/**
	 * This method returns the initialization state.
	 *
	 * @return The initialization state.
	 */
	inline  b3_bool  b3IsInitialized()
	{
		return m_Initialized;
	}

	/**
	 * This method returns the done state. If True the operation
	 * has done or redone otherwise the operations has undone.
	 * The operation may need the done state when calling the
	 * b3Delete() method to determine which memory or instances
	 * may be freed.
	 *
	 * @return True if the operation is in done or redone state.
	 */
	inline  b3_bool  b3IsDone()
	{
		return m_Done;
	}

public:
	virtual int      b3GetId() = 0;
	/**
	 * This method returns a unique ID for each operation implementation. This can be used in GUIs
	 * such MFC to convert this ID into clear text.
	 *
	 * @return An unique ID for each operation implementation.
	 */
};

/**
 * This class handles undo/redo operatations.
 */
class b3UndoBuffer
{
protected:
	b3Base<b3UndoOperation>  m_UndoBuffer;   //!< The list of undo operations.
	b3Base<b3UndoOperation>  m_RedoBuffer;   //!< The list of redo operations.
	b3UndoPrepareInfo       *m_PrepareInfo;  //!< An information class for custom purposes.

public:
	/**
	 * This constructor initializes this undo/redo buffer instance.
	 */
	b3UndoBuffer();

	/**
	 * The destructor calls b3Clear() for deinitializing this instance.
	 */
	virtual ~b3UndoBuffer();

	/**
	 * This method clears both the undo and the redo operation lists.
	 * All operations receive a b3Delete() method call.
	 */
	void     b3Clear();

	/**
	 * This method does a first do on an operation. The operation must be
	 * initialized. First the operations b3Do() method is called and the
	 * operation is marked as done. After that the operations b3Prepare()
	 * method is called. Now all operations in the redo operation list are
	 * deleted. So these operations receive a b3Delete() method call. After
	 * that the new done operation is appended to the undo list. If the
	 * operation was not initialized the operation itself gets not a b3Delete()
	 * method call.
	 *
	 * @param operation The new operation for doing.
	 * @return True if the operation was initialized.
	 */
	b3_bool  b3Do(b3UndoOperation *operation);

	/**
	 * This method removes the last undo operation from the list if any
	 * calls the b3Undo() method in that operation, marks that operation
	 * as not done. After that the operations b3Prepare() method is called
	 * and the operation is put as first element into the redo list.
	 */
	void     b3Undo();

	/**
	 * This method removes the first redo operation from the list if any
	 * calls the b3Redo() method in that operation, marks that operation
	 * as done. After that the operations b3Prepare() method is called
	 * and the operation is put as last element into the undo list.
	 */
	void     b3Redo();

	/**
	 * This method returns true if any operation is in the undo list.
	 *
	 * @return True if the undo operation list is not empty.
	 */
	b3_bool  b3HasUndo();

	/**
	 * This method returns true if any operation is in the redo list.
	 *
	 * @return True if the redo operation list is not empty.
	 */
	b3_bool  b3HasRedo();

private:
	void     b3Delete(b3UndoOperation *op);
	void     b3Delete(b3Base<b3UndoOperation> *buffer);
};

#endif
