/*
**
**	$Filename:	b3ImagePreviewFileDlg.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Image file select dialog box
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(AFX_PREVIEWFILEDLG_H__1D054314_0872_11D2_8A46_0000E81D3D27__INCLUDED_)
#define AFX_PREVIEWFILEDLG_H__1D054314_0872_11D2_8A46_0000E81D3D27__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PreviewFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CB3ImagePreviewFileDlg dialog

#include "blz3/system/b3FileDialog.h"
#include "blz3/system/b3ShowImage.h"

class CB3ImagePreviewFileDlg : public CB3FileDialog
{
	DECLARE_DYNAMIC(CB3ImagePreviewFileDlg)

public:
	CB3ImagePreviewFileDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

// Attributes
public:
	BOOL         m_bPreview;
	CB3ShowImage m_PreviewCtrl;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3ImagePreviewFileDlg)
	protected:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CB3ImagePreviewFileDlg)
	afx_msg void OnPreview();
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	virtual void OnFileNameChange();
	virtual void OnFolderChange();
	virtual void b3LoadImage(const char *image_name);

// Implementation
	DECLARE_MESSAGE_MAP()
};


class CB3ObjectPreviewFileDlg : public CB3ImagePreviewFileDlg
{
	DECLARE_DYNAMIC(CB3ObjectPreviewFileDlg)

public:
	CB3ObjectPreviewFileDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3ObjectPreviewFileDlg)
	//}}AFX_VIRTUAL
protected:
	virtual void b3LoadImage(const char *image_name);

protected:
	//{{AFX_MSG(CB3ObjectPreviewFileDlg)
	//}}AFX_MSG

// Implementation
	DECLARE_MESSAGE_MAP()
};


#endif // !defined(AFX_PREVIEWFILEDLG_H__1D054314_0872_11D2_8A46_0000E81D3D27__INCLUDED_)
