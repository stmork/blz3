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
#include "b3Fulcrum.h"

class CAppRaytraceDoc;
class CDlgHierarchy;

class CAppRenderDoc : public CDocument, public b3Document
{
protected:
	CDlgHierarchy        *m_DlgHierarchy;
	CAppRaytraceDoc      *m_RaytraceDoc;
	b3Thread             *m_Raytracer;
	b3Display            *m_Display;

public:
	b3ModellerInfo       *m_Info;
	b3RenderShapeContext  m_Context;
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
	virtual void       b3ComputeBounds();

	virtual   ~CAppRenderDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAppRenderDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPRENDERDOC_H)
