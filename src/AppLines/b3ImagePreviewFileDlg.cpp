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
**	Revision 1.5  2002/08/09 14:05:51  sm
**	- Minor bug fixes.
**
**	Revision 1.4  2002/04/07 12:59:38  sm
**	- Added support for file dialog with Windows 2000 place bars (Cb3FileDialog)
**	- CB3FileDialog used for CWinApp::OnFileOpen()
**	- Image buttons changed to draw disabled state correctly using
**	  CDC::DrawState()
**	
**	Revision 1.3  2002/01/11 16:14:39  sm
**	- Fixed damaged b3Transform() by correcting used parameter vor
**	  b3MatrixMMul and the b3BBox::m_Matrix meber.
**	- Fixed Preview selection dialog.
**	
**	Revision 1.2  2002/01/10 20:18:54  sm
**	- CFileDlg runs but CB3ImagePreviewFileDlg not! I don't know
**	  what to do...
**	
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

	m_bPreview = TRUE;
}

void CB3ImagePreviewFileDlg::b3LoadImage(const char *image_name)
{
	if (m_PreviewCtrl.b3Load(image_name))
	{
		m_PreviewCtrl.b3Update(true);
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
		strcat((char *)thumbnail,".TGA");
		m_PreviewCtrl.b3Load(thumbnail);
	}
	else
	{
		m_PreviewCtrl.b3Load(null);
	}
	m_PreviewCtrl.b3Update(false,false);
}
