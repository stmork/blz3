/*
**
**	$Filename:	AppLinesDoc.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CDocument part
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(APPLINESDOC_H)
#define APPLINESDOC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "blz3/raytrace/b3Raytrace.h"
#include "b3Fulcrum.h"

class CAppRaytraceDoc;

class CAppLinesDoc : public CDocument, public b3Document
{
protected:
	b3World               m_World;
	b3Thread             *m_Raytracer;
	b3Display            *m_Display;
	CAppRaytraceDoc      *m_RaytraceDoc;
// Attributes
public:
	b3Fulcrum             m_Fulcrum;
	b3Scene              *m_Scene;
	b3ModellerInfo       *m_Info;
	b3RenderShapeContext  m_Context;
	b3_vector             m_Lower;
	b3_vector             m_Upper;

// Operations
protected: // create from serialization only
	CAppLinesDoc();
	DECLARE_DYNCREATE(CAppLinesDoc)

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppLinesDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	b3_vector *b3GetFulcrum();
	void       b3DrawFulcrum();
	void       b3ComputeBounds();
	void       b3ClearRaytraceDoc();
	void       b3StartRaytrace();
	void       b3StopRaytrace();
	void       b3ToggleRaytrace();
	b3_bool    b3IsRaytracing();

	virtual   ~CAppLinesDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	static b3_u32  b3RaytracingThread(void *ptr);

	// Generated message map functions
protected:
	//{{AFX_MSG(CAppLinesDoc)
	afx_msg void OnHierachy();
	afx_msg void OnRaytrace();
	afx_msg void OnDlgScene();
	afx_msg void OnUpdateRaytrace(CCmdUI* pCmdUI);
	afx_msg void OnModellerInfo();
	afx_msg void OnLightNew();
	afx_msg void OnLightDelete();
	afx_msg void OnLightProperties();
	afx_msg void OnLightEnable();
	afx_msg void OnLightSoft();
	afx_msg void OnUpdateLightDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLightEnable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLightSoft(CCmdUI* pCmdUI);
	afx_msg void OnLightLDC();
	afx_msg void OnUpdateLightLDC(CCmdUI* pCmdUI);
	afx_msg void OnLightSpot();
	afx_msg void OnUpdateLightSpot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGlobal(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CAppLinesDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPLINESDOC_H)
