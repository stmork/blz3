/*
**
**	$Filename:	AppObjectDoc.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CDocument part for objects
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(APPOBJECTDOC_H)
#define APPOBJECTDOC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AppRenderDoc.h"

class CDlgHierarchy;

class CAppObjectDoc : public CAppRenderDoc
{
	CAppLinesDoc         *m_LinesDoc;
	b3BBox               *m_Original;
	b3_matrix             m_OriginalPosition;

protected:
	b3World               m_World;

	// Attributes
public:
	b3BBox               *m_BBox;

// Operations
protected: // create from serialization only
	CAppObjectDoc();
	DECLARE_DYNCREATE(CAppObjectDoc)

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppObjectDoc)
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

	virtual   ~CAppObjectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAppObjectDoc)
	afx_msg void OnObjectNew();
	afx_msg void OnAllActivate();
	afx_msg void OnAllDeactivate();
	afx_msg void OnAllDeactivateRest();
	afx_msg void OnDeactivateRest();
	afx_msg void OnActivate();
	afx_msg void OnDeactivate();
	afx_msg void OnUpdateSelectedItem(CCmdUI* pCmdUI);
	afx_msg void OnObjectDelete();
	afx_msg void OnObjectEdit();
	afx_msg void OnUpdateObjectEdit(CCmdUI* pCmdUI);
	afx_msg void OnEditMaterial();
	afx_msg void OnUpdateEditMaterial(CCmdUI* pCmdUI);
	afx_msg void OnEditBump();
	afx_msg void OnUpdateEditBump(CCmdUI* pCmdUI);
	afx_msg void OnCopyProperties();
	afx_msg void OnUpdateCopyProperties(CCmdUI* pCmdUI);
	afx_msg void OnDeleteTransformHistory();
	afx_msg void OnEditMaterialDirect();
	afx_msg void OnEditBumpDirect();
	afx_msg void OnUpdateEditMaterialDirect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditBumpDirect(CCmdUI* pCmdUI);
	afx_msg void OnCopyMaterialToBump();
	afx_msg void OnUpdateCopyMaterialToBump(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CAppObjectDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
	void        b3EditBBox(CAppLinesDoc *LinesDoc,b3BBox *original);
	void        b3ComputeBounds();
	void        b3InitTree();
	void        b3HierarchySelectionChanged();
	b3Shape    *b3GetSelectedShape();
	void        b3ContextMenu(HTREEITEM item);
	void        b3DropBBox(b3BBox *dragBBox,b3BBox *dropBBox);
	b3_bool     b3IsLinesDoc(CAppLinesDoc *LinesDoc);
	b3_bool     b3IsObjectAlreadyOpen(CAppLinesDoc *LinesDoc,b3BBox *bbox);
	const char *b3GetDocumentName();
	b3Scene    *b3GetParentScene();
	b3_u32      b3GetParentShading();

private:
	void        b3SetBBox(b3BBox *bbox);
	void        b3ActivateItem(b3Item *item,b3_bool activate=true);
	b3Item     *b3FindItem(b3Base<b3Item> *head,b3_u32 class_type);
	b3Item     *b3EnsureSingleItem(b3Base<b3Item> *head,b3_u32 class_type);
	b3_bool     b3CopyMaterialToBump();
	b3_bool     b3CopyBumpToMaterial();
	void        b3UpdateSurface(b3Shape *shape);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(APPOBJECTDOC_H)
