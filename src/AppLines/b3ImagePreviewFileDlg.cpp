/*
**
**	$Filename:	b3ImagePreviewFileDlg.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision: 1447 $
**	$Date: 2006-06-28 18:35:42 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Image file select dialog box
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLinesInclude.h"

#include "b3ImagePreviewFileDlg.h"

#include <dlgs.h>

/*************************************************************************
**                                                                      **
**                        CB3ImagePreviewFileDlg implementation         **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNAMIC(CB3ImagePreviewFileDlg, CB3FileDialog)


BEGIN_MESSAGE_MAP(CB3ImagePreviewFileDlg, CB3FileDialog)
	//{{AFX_MSG_MAP(CB3ImagePreviewFileDlg)
	ON_BN_CLICKED(IDC_PREVIEW, OnPreview)
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CB3ImagePreviewFileDlg::CB3ImagePreviewFileDlg(
	BOOL      bOpenFileDialog,
	LPCTSTR   lpszDefExt,
	LPCTSTR   lpszFileName,
	DWORD     dwFlags,
	LPCTSTR   lpszFilter,
	CWnd     *pParentWnd) :
		CB3FileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	m_ofn.Flags |= (OFN_EXPLORER | OFN_ENABLESIZING | OFN_ENABLETEMPLATE);
	m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_FILEOPENPREVIEW);

	m_bPreview   = TRUE;
	m_Filtered   = true;
	m_KeepAspect = false;
}

void CB3ImagePreviewFileDlg::b3LoadImage(const char *image_name)
{
	if (m_PreviewCtrl.b3Load(image_name))
	{
		m_PreviewCtrl.b3Update(m_Filtered,m_KeepAspect);
	}
}

BOOL CB3ImagePreviewFileDlg::OnInitDialog() 
{
	CWnd *wnd;

	CFileDialog::OnInitDialog();
	
	m_PreviewCtrl.SubclassDlgItem(IDC_IMAGE, this);
	wnd = GetDlgItem(IDC_PREVIEW);
	if (wnd != null)
	{
		wnd->SendMessage(BM_SETCHECK, (m_bPreview) ? 1 : 0);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CB3ImagePreviewFileDlg::OnFileNameChange() 
{
	CFileDialog::OnFileNameChange();
	if (m_bPreview)
	{
		b3LoadImage(GetPathName()); // the control will handle errors
	}
}

void CB3ImagePreviewFileDlg::OnFolderChange() 
{
	CFileDialog::OnFolderChange();
	b3LoadImage(null);
}

void CB3ImagePreviewFileDlg::OnPreview() 
{
	m_bPreview = !m_bPreview;
	if (!m_bPreview)
	{
		b3LoadImage(null); // no preview
	}
	else
	{
		b3LoadImage(GetPathName()); // the control will handle errors
	}
}

BOOL CB3ImagePreviewFileDlg::OnQueryNewPalette() 
{
	m_PreviewCtrl.SendMessage(WM_QUERYNEWPALETTE);	// redo the palette if necessary
	return CFileDialog::OnQueryNewPalette();
}

void CB3ImagePreviewFileDlg::OnPaletteChanged(CWnd* pFocusWnd) 
{
	CFileDialog::OnPaletteChanged(pFocusWnd);
	m_PreviewCtrl.SendMessage(WM_PALETTECHANGED, (WPARAM)pFocusWnd->GetSafeHwnd());	// redo the palette if necessary
}

void CB3ImagePreviewFileDlg::OnSetFocus(CWnd* pOldWnd) 
{
	CFileDialog::OnSetFocus(pOldWnd);
	m_PreviewCtrl.SendMessage(WM_QUERYNEWPALETTE);	// redo the palette if necessary
}
/*************************************************************************
**                                                                      **
**                        CB3ObjectPreviewFileDlg implementation        **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNAMIC(CB3ObjectPreviewFileDlg, CB3ImagePreviewFileDlg)

BEGIN_MESSAGE_MAP(CB3ObjectPreviewFileDlg, CB3ImagePreviewFileDlg)
	//{{AFX_MSG_MAP(CB3ObjectPreviewFileDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CB3ObjectPreviewFileDlg::CB3ObjectPreviewFileDlg(
	BOOL      bOpenFileDialog,
	LPCTSTR   lpszDefExt,
	LPCTSTR   lpszFileName,
	DWORD     dwFlags,
	LPCTSTR   lpszFilter,
	CWnd     *pParentWnd) :
		CB3ImagePreviewFileDlg(
			bOpenFileDialog,
			lpszDefExt,
			lpszFileName,
			dwFlags,
			lpszFilter,
			pParentWnd)
{
}

void CB3ObjectPreviewFileDlg::b3LoadImage(const char *image_name)
{
	if (image_name != null)
	{
		b3Path thumbnail;

		thumbnail.b3RemoveExt(image_name);
		thumbnail.b3Append(".TGA");
		m_PreviewCtrl.b3Load(thumbnail);
	}
	else
	{
		m_PreviewCtrl.b3Load(null);
	}
	m_PreviewCtrl.b3Update(m_Filtered,m_KeepAspect);
}
