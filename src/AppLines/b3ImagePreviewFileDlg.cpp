/*
**
**	$Filename:	b3ImagePreviewFileDlg.cpp $
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

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLines.h"
#include "b3ImagePreviewFileDlg.h"

#include <dlgs.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2002/01/10 17:31:11  sm
**	- Some minor GUI updates.
**	- b3BBox::b3Transform() changes m_Matrix member.
**	- Added image selection with image preview.
**
**
*/

/*************************************************************************
**                                                                      **
**                        CB3ImagePreviewFileDlg implementation         **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNAMIC(CB3ImagePreviewFileDlg, CFileDialog)

CB3ImagePreviewFileDlg::CB3ImagePreviewFileDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	m_ofn.Flags |= (OFN_EXPLORER | OFN_ENABLETEMPLATE);
	m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_FILEOPENPREVIEW);

  m_bPreview = TRUE;
}


BEGIN_MESSAGE_MAP(CB3ImagePreviewFileDlg, CFileDialog)
	//{{AFX_MSG_MAP(CB3ImagePreviewFileDlg)
	ON_BN_CLICKED(IDC_PREVIEW, OnPreview)
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CB3ImagePreviewFileDlg::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	
	m_DIBStaticCtrl.SubclassDlgItem(IDC_IMAGE, this);
	GetDlgItem(IDC_PREVIEW)->SendMessage(BM_SETCHECK, (m_bPreview) ? 1 : 0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CB3ImagePreviewFileDlg::OnFileNameChange() 
{
	CFileDialog::OnFileNameChange();
	if (m_bPreview)
		m_DIBStaticCtrl.b3Load(GetPathName()); // the control will handle errors
}

void CB3ImagePreviewFileDlg::OnFolderChange() 
{
	CFileDialog::OnFolderChange();
//	m_DIBStaticCtrl.RemoveDib();
}

void CB3ImagePreviewFileDlg::OnPreview() 
{
	m_bPreview = !m_bPreview;
	if (!m_bPreview)
		m_DIBStaticCtrl.b3Load(null); // no preview
	else
		m_DIBStaticCtrl.b3Load(GetPathName()); // the control will handle errors
}

BOOL CB3ImagePreviewFileDlg::OnQueryNewPalette() 
{
	m_DIBStaticCtrl.SendMessage(WM_QUERYNEWPALETTE);	// redo the palette if necessary
	return CFileDialog::OnQueryNewPalette();
}

void CB3ImagePreviewFileDlg::OnPaletteChanged(CWnd* pFocusWnd) 
{
	CFileDialog::OnPaletteChanged(pFocusWnd);
	m_DIBStaticCtrl.SendMessage(WM_PALETTECHANGED, (WPARAM)pFocusWnd->GetSafeHwnd());	// redo the palette if necessary
}

void CB3ImagePreviewFileDlg::OnSetFocus(CWnd* pOldWnd) 
{
	CFileDialog::OnSetFocus(pOldWnd);
	m_DIBStaticCtrl.SendMessage(WM_QUERYNEWPALETTE);	// redo the palette if necessary
}

#ifdef _DEBUG
void CB3ImagePreviewFileDlg::Dump(CDumpContext& dc) const
{
	CFileDialog::Dump(dc);
	if (m_bPreview)
		dc << "preview is enabled\n";
	else
		dc << "preview is disabled\n";
}
#endif //_DEBUG
