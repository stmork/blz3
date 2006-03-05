/*
**
**	$Filename:	b3FileDialog.cpp $
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

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "b3SystemInclude.h"
#include "blz3/system/b3FileDialog.h"
#include "afxpriv.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2006/03/05 21:22:36  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.2  2002/04/09 07:24:19  sm
**	- Some bugs fixed concerning Windows NT and CB3FileDialog
**	
**	Revision 1.1  2002/04/07 12:59:38  sm
**	- Added support for file dialog with Windows 2000 place bars (Cb3FileDialog)
**	- CB3FileDialog used for CWinApp::OnFileOpen()
**	- Image buttons changed to draw disabled state correctly using
**	  CDC::DrawState()
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CB3FileDialog implementation                  **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNAMIC(CB3FileDialog, CFileDialog)


BEGIN_MESSAGE_MAP(CB3FileDialog, CFileDialog)
	//{{AFX_MSG_MAP(CB3FileDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CB3FileDialog::CB3FileDialog(
	BOOL      bOpenFileDialog,
	LPCTSTR   lpszDefExt,
	LPCTSTR   lpszFileName,
	DWORD     dwFlags,
	LPCTSTR   lpszFilter,
	CWnd     *pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	m_Win2K = IS_WIN2K;
}

int CB3FileDialog::DoModal()
{
	if (!m_Win2K)
	{
		// Less than Windows 2000
		return CFileDialog::DoModal();
	}
	else
	{
		ASSERT_VALID(this);
		ASSERT(m_ofn.Flags & OFN_ENABLEHOOK);
		ASSERT(m_ofn.lpfnHook != NULL); // can still be a user hook

		// zero out the file buffer for consistent parsing later
		ASSERT(AfxIsValidAddress(m_ofn.lpstrFile, m_ofn.nMaxFile));
		DWORD nOffset = lstrlen(m_ofn.lpstrFile)+1;
		ASSERT(nOffset <= m_ofn.nMaxFile);
		memset(m_ofn.lpstrFile+nOffset, 0, (m_ofn.nMaxFile-nOffset)*sizeof(TCHAR));

		// WINBUG: This is a special case for the file open/save dialog,
		//  which sometimes pumps while it is coming up but before it has
		//  disabled the main window.
		HWND hWndFocus = ::GetFocus();
		BOOL bEnableParent = FALSE;
		m_ofn.hwndOwner = PreModal();
		AfxUnhookWindowCreate();
		if (m_ofn.hwndOwner != NULL && ::IsWindowEnabled(m_ofn.hwndOwner))
		{
			bEnableParent = TRUE;
			::EnableWindow(m_ofn.hwndOwner, FALSE);
		}

		_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
		ASSERT(pThreadState->m_pAlternateWndInit == NULL);

		if (m_ofn.Flags & OFN_EXPLORER)
			pThreadState->m_pAlternateWndInit = this;
		else
			AfxHookWindowCreate(this);

		// Copy m_ofn into m_ofnEx
		memset(&m_ofnEx,0,sizeof(m_ofnEx));
		memcpy(&m_ofnEx,&m_ofn,sizeof(m_ofn));
		m_ofnEx.lStructSize = sizeof(m_ofnEx);

		int nResult;
		if (m_bOpenFileDialog)
			nResult = ::GetOpenFileName(&m_ofnEx);
		else
			nResult = ::GetSaveFileName(&m_ofnEx);

		// Copy back m_ofnEx into m_ofn
		memcpy(&m_ofn,&m_ofnEx,sizeof(m_ofn));
		m_ofn.lStructSize = sizeof(m_ofn);
		
		if (nResult)
			ASSERT(pThreadState->m_pAlternateWndInit == NULL);
		pThreadState->m_pAlternateWndInit = NULL;

		// WINBUG: Second part of special case for file open/save dialog.
		if (bEnableParent)
			::EnableWindow(m_ofn.hwndOwner, TRUE);
		if (::IsWindow(hWndFocus))
			::SetFocus(hWndFocus);

		PostModal();
		return nResult ? nResult : IDCANCEL;
	}
}

BOOL CB3FileDialog::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_Win2K)
	{
		memcpy(&m_ofn,&m_ofnEx,sizeof(m_ofn));
		m_ofn.lStructSize = sizeof(m_ofn);
	}
	return CFileDialog::OnNotify(wParam, lParam, pResult);
}

