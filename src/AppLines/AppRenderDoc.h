/*
**
**	$Filename:	AppRenderDoc.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CDocument part for OpenGL Documents (scenes/objects)
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(APPRENDERDOC_H)
#define APPRENDERDOC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/system/b3Document.h"
#include "b3Fulcrum.h"
#include "b3Undo.h"

class CAppRaytraceDoc;
class CDlgHierarchy;
class CB3Action;

class CAppRenderDoc : public CDocument, public b3Document
{
	b3UndoBuffer         *m_UndoBuffer;

protected:
	CDlgHierarchy        *m_DlgHierarchy;
	CAppRaytraceDoc      *m_RaytraceDoc;
	b3Thread             *m_Raytracer;
	b3Display            *m_Display;

public:
	LPARAM                m_FirstVisible;
	LPARAM                m_Selected;
	b3ModellerInfo       *m_Info;
	b3ShapeRenderContext  m_Context;
	b3Fulcrum             m_Fulcrum;
	b3_vector             m_Lower;
	b3_vector             m_Upper;

// Operations
protected: // create from serialization only
	CAppRenderDoc();
	DECLARE_DYNCREATE(CAppRenderDoc)

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppRenderDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
	        b3_vector *b3GetFulcrum();
	        b3_vector *b3GetStepMove();
	        b3_vector *b3GetStepRotate();
	        void       b3DrawFulcrum();
	        void       b3ToggleRaytrace();
	        b3_bool    b3IsRaytracing();
	        void       b3ClearRaytraceDoc();
	virtual void       b3StartRaytrace();
	virtual void       b3StopRaytrace();

	virtual void       b3Prepare(b3_bool geometry_changed,b3_bool structure_changed,b3_bool reorg=false);
	virtual void       b3ComputeBounds();
	virtual void       b3InitTree();
	virtual void       b3SelectItem(b3Item *item);
	virtual void       b3DropBBox(b3BBox *dragBBox,b3BBox *dropBBox);
	virtual void       b3ContextMenu(HTREEITEM item);

	virtual void       b3AddUndoAction(CB3Action *action);
	        void       b3AddOp(b3Operation *op);
			void       b3ClearOp();

	// Drag & drop operations
	virtual void       b3DragBegin();
	virtual HTREEITEM  b3Dragging(HTREEITEM dragitem,HTREEITEM dropitem);
	virtual void       b3Drop    (HTREEITEM dragitem,HTREEITEM dropitem);

	virtual   ~CAppRenderDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAppRenderDoc)
	afx_msg void OnRename();
	afx_msg void OnUpdateRename(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPRENDERDOC_H)
