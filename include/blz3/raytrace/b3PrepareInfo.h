/*
**
**	$Filename:	b3PrepareInfo.h $ 
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Base class for multhithreaded scene initialization
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_RAYTRACE_PREPAREINFO_H
#define B3_RAYTRACE_PREPAREINFO_H

#include "blz3/system/b3Thread.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3List.h"

class b3Scene;
class b3BBox;

/**
 * A class which represents onereference to a bounding box.
 *
 * @see b3BBox
 */
class B3_PLUGIN b3BBoxReference : public b3Link<b3BBoxReference>
{
public:
	b3BBox *m_BBox; //!< The bounding box reference.

	/**
	 * This contructor initializes the bounding box reference.
	 */
	b3BBoxReference(b3BBox *bbox = null) : b3Link<b3BBoxReference>(sizeof(b3BBoxReference))
	{
		m_BBox = bbox;
	}
};

/**
 * This enumeration lists error codes which can occur during preparation.
 */
enum b3_error_prepare
{
	B3_PREPARE_ERROR = -1, //!< General error
	B3_PREPARE_OK    =  0, //!< Everything went good.
	B3_PREPARE_NO_MEMORY,  //!< Out of memory error.
	B3_PREPARE_NO_THREAD   //!< No thread available.
};

typedef b3_bool (*b3PrepareProc)(b3BBox *bbox,void *ptr);
typedef b3Exception<b3_error_prepare,'PRP'> b3PrepareException;

#define B3_MIN_BBOXES_FOR_THREADING 50

/**
 * This class handles multi threaded preparation of bounding boxes.
 */
class B3_PLUGIN b3PrepareInfo
{
	b3Thread                *m_Threads;
	b3PrepareProc            m_PrepareProc;
	void                    *m_Ptr;
	b3_count                 m_CPUs;

protected:
	b3Base<b3BBoxReference>  m_BBoxRefList;   //!< A working list of outstanding bounding boxes.
	b3Array<b3BBoxReference> m_BBoxRefArray;  //!< The collected bounding boxes
	b3Mutex                  m_Mutex;         //!< A mutex for synchronized access to the work list.
	b3_count                 m_MinBBoxesForThreading; //!< Minimal amount of bounding boxes for multi threaded processing.

public:
	/**
	 * This constructor initializes this instance.
	 */
	b3PrepareInfo();

	/**
	 * This destructor deinitializes this instance.
	 */
	~b3PrepareInfo();

	/**
	 * This method collects all bounding boxes of a scene.
	 *
	 * @param scene The scene which contains the bounding box.
	 */
	void             b3CollectBBoxes(b3Scene *scene);

	/**
	 * This method collects all sub bounding boxes of the given bounding box.
	 *
	 * @param firstBBox The first bounding box to collect.
	 */
	void             b3CollectBBoxes(b3BBox  *firstBBox);

	/**
	 * This method prepares all collected bounding boxes in multi threaded manner.
	 * The preparation aborts when one bounding box could not beeing initialized.
	 *
	 * @param prepareFunc   The preparation function.
	 * @param ptr           A pointer for custom information.
	 * @param threaded      A flag if the call should be multi threaded on multi CPU machines,
	 * @throws b3PrepareException
	 * @return True on success.
	 */
	b3_bool          b3Prepare(b3PrepareProc prepareFunc,void *ptr = null,b3_bool threaded = true) throw(b3PrepareException);

private:
	b3BBoxReference *b3GetBBoxReference();
	void             b3RebuildListFromArray();
	static b3_u32    b3PrepareThread(void *ptr);
};

#endif
