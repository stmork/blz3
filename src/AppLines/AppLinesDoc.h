/*
**
**	$Filename:	AppLinesDoc.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CDocument part for scenes
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
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
#include "AppRenderDoc.h"

class CAppLinesDoc : public CAppRenderDoc
{
protected:
	b3World               m_World;
// Attributes
public:
	b3Scene              *m_Scene;

// Operations
protected: // create from serialization only
	CAppLinesDoc();
	DECLARE_DYNCREATE(CAppLinesDoc)

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppLinesDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	void       b3ComputeBounds();
	void       b3StartRaytrace();
	void       b3StopRaytrace();

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
	afx_msg void OnActivate();
	afx_msg void OnDeactivate();
	afx_msg void OnDeactivateRest();
	afx_msg void OnAllDeactivate();
	afx_msg void OnAllActivate();
	afx_msg void OnUpdateSelectedBBox(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditPasteSub();
	afx_msg void OnAllDeactivateRest();
	afx_msg void OnObjectNew();
	afx_msg void OnObjectNewSub();
	afx_msg void OnObjectDelete();
	afx_msg void OnObjectLoad();
	afx_msg void OnObjectSave();
	afx_msg void OnObjectReplace();
	afx_msg void OnObjectCopy();
	afx_msg void OnObjectEdit();
	afx_msg void OnUpdateObjectEdit(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CAppLinesDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
	void           b3FinishEdit(b3BBox *original = null,b3BBox *bbox = null);
	void           b3InitTree();
	void           b3DropBBox(b3BBox *dragBBox,b3BBox *dropBBox);
	void           b3ContextMenu(HTREEITEM item);

private:
	b3_bool        b3WriteBBox(b3BBox *bbox,b3FileAbstract *file);
	b3_bool        b3WriteBBox(b3BBox *bbox,const char *filename);
	b3_bool        b3PutClipboard(b3BBox *bbox);
	void           b3PasteClipboard(b3_bool insert_sub);
	void           b3ObjectCreate(b3_bool insert_sub);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPLINESDOC_H)
