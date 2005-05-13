/*
**
**	$Filename:	b3UndoShapeSurface.h
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Undo/Redo for shape surface handling
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3UNDOSHAPESURFACE_H
#define B3UNDOSHAPESURFACE_H

#include "b3UndoShape.h"

/*************************************************************************
**                                                                      **
**                        Undo/Redo surface list edit                   **
**                                                                      **
*************************************************************************/

class b3OpShapeSurfaceList : public b3OpShape
{
protected:
	b3Shape        *m_Shape;     // This is the properties' shape
	b3Base<b3Item> *m_Base;      // The material base to edit
	b3Base<b3Item>  m_OrigBase;  // The the original properties' base
	b3Base<b3Item>  m_CloneBase; // The the cloned properties' base

protected:
	b3OpShapeSurfaceList(b3BBox *root, CDlgHierarchy *hierarchy);

protected:
	virtual void b3Undo();
	virtual void b3Redo();
	virtual void b3Delete();
};

class b3OpShapeMaterialEditList : public b3OpShapeSurfaceList
{

public:
	b3OpShapeMaterialEditList(CAppObjectDoc *pDoc, b3BBox *root, CDlgHierarchy *hierarchy);

protected:
	void b3Undo();
	void b3Redo();

	inline int  b3GetId()
	{
		return IDS_OP_SHAPE_MATERIAL_MAINTAIN;
	}
};

class b3OpShapeBumpEditList : public b3OpShapeSurfaceList
{
public:
	b3OpShapeBumpEditList(CAppObjectDoc *pDoc, b3BBox *root, CDlgHierarchy *hierarchy);

protected:
	inline int  b3GetId()
	{
		return IDS_OP_SHAPE_BUMP_MAINTAIN;
	}
};

class b3OpShapeConditionEditList : public b3OpShapeSurfaceList
{
public:
	b3OpShapeConditionEditList(CAppObjectDoc *pDoc, b3BBox *root, CDlgHierarchy *hierarchy);

protected:
	void b3Undo();
	void b3Redo();

	inline int  b3GetId()
	{
		return IDS_OP_SHAPE_CONDITION_MAINTAIN;
	}
};

/*************************************************************************
**                                                                      **
**                        Undo/Redo surface item edit                   **
**                                                                      **
*************************************************************************/

class b3OpShapeSurfaceItem : public b3OpShape
{
protected:
	b3Shape        *m_Shape;       // This is the materials' shape
	b3Base<b3Item> *m_Base;        // The material base to edit
	b3Item         *m_Original;    // The original item
	b3Item         *m_Clone;       // The cloned itemd
	b3Item         *m_InsertAfter; // item for reinsertion

protected:
	b3OpShapeSurfaceItem(b3BBox *root, CDlgHierarchy *hierarchy);

protected:
	virtual void b3Undo();
	virtual void b3Redo();
	virtual void b3Delete();
};

class b3OpShapeMaterialEdit : public b3OpShapeSurfaceItem
{

public:
	b3OpShapeMaterialEdit(CAppObjectDoc *pDoc, b3BBox *root, b3Shape *shape, CDlgHierarchy *hierarchy);

protected:
	void b3Undo();
	void b3Redo();

	inline int  b3GetId()
	{
		return IDS_OP_SHAPE_MATERIAL_EDIT;
	}
};

class b3OpShapeBumpEdit : public b3OpShapeSurfaceItem
{
public:
	b3OpShapeBumpEdit(CAppObjectDoc *pDoc, b3BBox *root, b3Shape *shape, CDlgHierarchy *hierarchy);

protected:
	inline int  b3GetId()
	{
		return IDS_OP_SHAPE_BUMP_EDIT;
	}
};

class b3OpShapeConditionEdit : public b3OpShapeSurfaceItem
{
public:
	b3OpShapeConditionEdit(CAppObjectDoc *pDoc, b3BBox *root, b3Shape *shape, CDlgHierarchy *hierarchy);

protected:
	void b3Undo();
	void b3Redo();

	inline int  b3GetId()
	{
		return IDS_OP_SHAPE_CONDITION_EDIT;
	}
};

#endif
