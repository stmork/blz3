/*
**
**	$Filename:	b3FileDialog.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - New Windows 2000 place bar file dialog
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#if !defined(B3_SYSTEM_FILEDIALOG_H__1D054314_0872_11D2_8A46_0000E81D3D27__INCLUDED_)
#define B3_SYSTEM_FILEDIALOG_H__1D054314_0872_11D2_8A46_0000E81D3D27__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PreviewFileDlg.h : header file
//

#include "blz3/b3Config.h"
#include <dlgs.h>

/////////////////////////////////////////////////////////////////////////////
// CB3ImagePreviewFileDlg dialog

struct OPENFILENAMEEX : public OPENFILENAME
{
	void  *pvReserved;
	DWORD  dwReserved;
	DWORD  FlagsEx;
};

class CB3FileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CB3FileDialog)

	bool m_Win2K;

protected:
	OPENFILENAMEEX m_ofnEx;

public:
	CB3FileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3FileDialog)
	public:
	virtual int DoModal();
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CB3FileDialog)
	//}}AFX_MSG

// Implementation
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(B3_SYSTEM_FILEDIALOG_H__1D054314_0872_11D2_8A46_0000E81D3D27__INCLUDED_)
