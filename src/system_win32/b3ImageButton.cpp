// b3ImageButton.cpp : implementation file
//

#include "stdafx.h"
#include "b3ImageButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CB3ImageButton

CB3ImageButton::CB3ImageButton()
{
	m_Pressed = false;
}

CB3ImageButton::~CB3ImageButton()
{
}


BEGIN_MESSAGE_MAP(CB3ImageButton, CButton)
	//{{AFX_MSG_MAP(CB3ImageButton)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CB3ImageButton message handlers

void CB3ImageButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CButton::OnLButtonDown(nFlags, point);

	if (!m_Pressed)
	{
		int   id  = GetDlgCtrlID( );
		MSG   mess;
		HWND  hWnd = GetSafeHwnd();

		m_Pressed = true;
		while( !PeekMessage( &mess, hWnd, WM_LBUTTONUP, WM_LBUTTONUP, PM_REMOVE ) )
		{
			Sleep(1000);
			SendMessage(BM_CLICK);
		}
		m_Pressed = false;
	}
}
